#version 450

layout(location = 0) in vec2 texCoords;

layout(binding = 1) uniform sampler2DArray vpb;
layout(binding = 2) uniform sampler2DArray vdb;
layout(binding = 3) uniform sampler2DArray lb;
layout(binding = 4) uniform sampler2DArray ldb;
layout(binding = 5) uniform sampler2DArray cb;
layout(binding = 6) uniform sampler2DArray mb;

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

layout(location = 0) out vec4 outColor;

void main() {

	vec4 teste = texture(cb, vec3(texCoords, 0.0f));

    outColor = teste;
}