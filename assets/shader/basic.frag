#version 450

vec3 sampleOffset[20] = vec3[]
(
   vec3( 1,  1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1,  1,  1), 
   vec3( 1,  1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1,  1, -1),
   vec3( 1,  1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1,  1,  0),
   vec3( 1,  0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1,  0, -1),
   vec3( 0,  1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0,  1, -1)
);

layout(location = 0) in vec2 texCoords;
layout(location = 1) in vec3 position_model;
layout(location = 2) in vec3 FragPos;

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

layout(location = 0) out vec4 outColor;

void main() {


    outColor = texture(cb, texCoords);
}