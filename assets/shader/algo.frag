#version 450

layout(location = 0) in vec2 texCoords;

layout(binding = 1) uniform sampler2D vpb;
layout(binding = 2) uniform sampler2D vdb;
layout(binding = 3) uniform sampler2D lb;
layout(binding = 4) uniform sampler2D ldb;
layout(binding = 5) uniform sampler2D cb;
layout(binding = 6) uniform sampler2D mb;

layout(binding = 7) uniform AlgoUniformBufferObject {
    mat4 inverseViewMatrix;
	mat4 viewMatrix;
	float planeDistance;
	vec3 middleOfPlaneVS;
	float sphereRadius;
	vec2 planeSides;
	ivec2 dims;
	vec3 refractionPos;
	vec4 refractionValue;
	float voxelDepth;
	float planeWidth;
	float planeHeight;
	vec3 lightVSPos;
	vec3 lightColor;
} ubo;

layout(binding = 8) uniform sampler3D volume;

layout(push_constant) uniform PerPlanePush {
	int layer;
	float currentZVS;
} push;

layout(location = 0) out vec4 vpbOut;
layout(location = 1) out vec4 vdbOut;
layout(location = 2) out vec4 lbOut;
layout(location = 3) out vec4 ldbOut;
layout(location = 4) out vec4 cbOut;
layout(location = 5) out vec4 mbOut;

void main() {	
	float currentZVS = push.currentZVS;
	int gl_Layer = push.layer;

	vec3 texOffset = vec3(0.5);//mat3(inverseViewMatrix) * vec3(planeSides, 0.5);	
	vec2 texFactor2 = vec2(planeSides/0.5);

	int readLayer = 1 - gl_Layer;
	int writeLayer = gl_Layer;

	// LIGHT
	vec4 ldi = texture(ldb, vec3(TexCoords, readLayer));

	
	vec2 lpi_1 = intersectPlane(
		vec3(0,0,-1),
		vec3(0.5, 0.5, currentZVS + planeDistance),
		-ldi.xyz,
		vec3(TexCoords, currentZVS)
	).xy;

	//TODO worldpos geometry shader way or this way??
	vec3 volumePosLPI_1 = (inverseViewMatrix * vec4(texFactor2 * (middleOfPlaneVS.xy + lpi_1 - 0.5), currentZVS + planeDistance, 1.0)).xyz + texOffset;
	vec3 WorldPos = (inverseViewMatrix * vec4(texFactor2 * (middleOfPlaneVS.xy + TexCoords - 0.5), currentZVS, 1.0)).xyz + texOffset;

	vec4 Li_1 = texture(lb, vec3(lpi_1, readLayer));	
	
	vec4 ldi_1 = texture(ldb, vec3(lpi_1, readLayer));
	ldi_1 += texture(ldb, vec3(lpi_1 + vec2(1,0)/dims, readLayer)) * 0.25;
	ldi_1 += texture(ldb, vec3(lpi_1 + vec2(-1,0)/dims, readLayer))* 0.25;
	ldi_1 += texture(ldb, vec3(lpi_1 + vec2(0, -1)/dims, readLayer))* 0.25;
	ldi_1 += texture(ldb, vec3(lpi_1 + vec2(0, 1)/dims, readLayer))* 0.25;
	ldi_1 = normalize(ldi_1 / 2.0);

	float Si = abs(dFdxFine(TexCoords.x)) * abs(dFdyFine(TexCoords.y));  // DOT CORRECT?
	float Si_1 = abs(dFdxFine(lpi_1.x)) *  abs(dFdyFine(lpi_1.y));
	
	float Ii = Si_1/Si; //IC

	if (!useIi) Ii = 1.0;

	// INTEGRATION TABLE LIGHT
	float volumeX = sampleVolTexture(WorldPos);
	float volumeLPI_1 = sampleVolTexture(volumePosLPI_1);
	vec4 transfer = texture(volumeTransfer, vec2(volumeX, volumeLPI_1));
	float alphaL = transfer.w;
	vec4 mL = texture(mediumTransfer, vec2(volumeX, volumeLPI_1));//1.0 - transfer.xyz * 0.05; // transfer medium lpi_1
	//mL.xyz = mL.xyz * mL.w + vec3(1.0) * (1-mL.w);
	
	//mL = vec3(1.0);
	vec4 Li = Li_1 * Ii *  abs(1.0 - alphaL) * vec4( vec3(0.99) + mL.xyz * 0.01,1);

	precise vec3 ref = getRefractionGradient((WorldPos+volumePosLPI_1)/2.0) * planeDistance;
	ldi = normalize(ldi_1 + (vec4(ref, 0.0)));
	
	lbOut = vec4(Li.xyz, 1.0);
	ldbOut = ldi;			

	// VIEW

	vec4 vpi = texture(vpb, vec3(TexCoords, readLayer));
	vec4 vpiWorldPos = inverseViewMatrix * vpi;
	vec4 vdi = texture(vdb, vec3(TexCoords, readLayer));
	vec4 c = texture(cb, vec3(TexCoords, readLayer));
	vec3 Ci_1 = c.xyz;
	float Ai_1 = c.w;
	vec4 Mi_1 = texture(mb, vec3(TexCoords, readLayer));
	vec4 id = texture(lb, vec3(TexCoords, readLayer));
	

	// INTEGRATION TABLE VIEW
	//vec4 vpi_1 = vpi - vdi * planeDistance;
	vec4 vpi_1 = vec4(intersectPlane(
		vec3(0,0,-1),
		vec3(0, 0, currentZVS + planeDistance),
		-vdi.xyz,
		vpi.xyz
	), 1.0);

	vec4 vpi_1WorldPos = inverseViewMatrix * vpi_1;
	float volumeVPI =  sampleVolTexture(vec3(vpiWorldPos) + texOffset);		
	float volumeVPI_1 =  sampleVolTexture(vec3(vpi_1WorldPos) + texOffset);
	vec4 transferV =  texture(volumeTransfer, vec2(volumeVPI, volumeVPI_1)) ;

	vec3 refractionView = getRefractionGradient((vpiWorldPos.xyz + vpi_1WorldPos.xyz)/2.0 + texOffset);

	vec3 is = vec3(0);


	vec3 cV = transferV.xyz;
	float alphaV = transferV.w;
	vec4 mV = texture(mediumTransfer, vec2(volumeVPI, volumeVPI_1));//vec3(1.0) - cV * 0.1; // transfer medium vpi_1	

	vec3 Ci = Ci_1  + (1.0 - Ai_1)  * Mi_1.xyz * ( alphaV *  cV  * id.xyz + is.xyz ); //TODO: add + is.xyz
	float Ai = Ai_1  + (1.0 - Ai_1) * alphaV;
	vec3 Mi = Mi_1.xyz * (1.0-mV.w) + (Mi_1.xyz * mV.xyz * mV.w);

	vec4 vdi_P1 = vdi;
	if (useVRayRefraction)
	{
		vdi_P1 = normalize(vdi + planeDistance * vec4(refractionView, 0.0));
	}
	
	
	//vec4 vpi_P2 = vpi + vdi_P1 * planeDistance;
	vec4 vpi_P1 = vec4(intersectPlane(
		vec3(0,0,-1),
		vec3(0, 0, currentZVS - planeDistance),
		vdi_P1.xyz,
		vpi.xyz
	), 1.0);

	vpbOut = vpi_P1;
	vdbOut = vdi_P1;
	cbOut = vec4(Ci, Ai);
	mbOut = vec4(Mi, 1.0);
}