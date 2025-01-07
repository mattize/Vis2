#version 450

layout(binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 proj;
} ubo;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec2 inTex;

layout(location = 0) out vec2 texCoords;
layout(location = 1) out vec3 position_model;
layout(location = 3) out vec3 FragPos;


void main() {
	
	texCoords = inTex;
	position_model = inPosition;
	FragPos = vec3(ubo.model * vec4(inPosition, 1.0));
	gl_Position = ubo.proj * ubo.view * ubo.model* vec4(inPosition, 1);
}