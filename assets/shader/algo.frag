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
	vpbOut = vec4(1.0, 0.0, 0.0, 1.0); // Red
    vdbOut = vec4(0.0, 1.0, 0.0, 1.0); // Green
    lbOut = vec4(0.0, 0.0, 1.0, 1.0); // Blue
    ldbOut = vec4(1.0, 1.0, 0.0, 1.0); // Yellow
    cbOut = vec4(0.0, 1.0, 1.0, 1.0); // Cyan
    mbOut = vec4(1.0, 0.0, 1.0, 1.0); // Magenta
}