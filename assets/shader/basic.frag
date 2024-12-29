#version 450

layout(location = 0) in vec2 texCoords;

layout(rgba32f, binding = 1) uniform image2DArray vpb;
layout(rgba32f, binding = 2) uniform image2DArray vdb;
layout(rgba32f, binding = 3) uniform image2DArray lb;
layout(rgba32f, binding = 4) uniform image2DArray ldb;
layout(rgba32f, binding = 5) uniform image2DArray cb;
layout(rgba32f, binding = 6) uniform image2DArray mb;

layout(push_constant) uniform PerPlanePush {
	int layer;
	float currentZVS;
} push;

layout(location = 0) out vec4 outColor;

void main() {

	ivec3 pixel_coords = ivec3(1.0, 1.0, 1.0); 
	
	imageStore(cb, pixel_coords, vec4(push.layer / 5.0f, 0.5, 1.0, 1.0));

    outColor = vec4(texCoords, push.layer / 5.0f, 1.0f);
}