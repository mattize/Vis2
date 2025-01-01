#version 450

layout(location = 0) in vec2 texCoords;

layout(rgba32f, binding = 1) uniform image2DArray vpb;
layout(rgba32f, binding = 2) uniform image2DArray vdb;
layout(rgba32f, binding = 3) uniform image2DArray lb;
layout(rgba32f, binding = 4) uniform image2DArray ldb;
layout(rgba32f, binding = 5) uniform image2DArray cb;
layout(rgba32f, binding = 6) uniform image2DArray mb;

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


void main() {
	ivec2 imageDimensions = ivec2(512, 512);
	
	vec2 scaledCoords = texCoords * vec2(imageDimensions);
	
	ivec2 texelCoords = ivec2(clamp(texCoords, vec2(0), vec2(imageDimensions - 1)));
	ivec3 texelArrayCoords = ivec3(texelCoords, 0);
	
	vec4 texelValue = imageLoad(vpb, texelArrayCoords);
	
	imageStore(cb, texelArrayCoords, texelValue); 
	
	//outColor = vec4(texCoords, push.layer / 5.0f, 1.0f);
}