#version 460
#ifdef STAGE_VERTEX
#pragma shader_stage(vertex)

layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 vColor;

layout (location = 0) out vec3 outColor;

layout(set=0, binding=0) uniform CameraBuffer {
    mat4 renderMatrix;
}camBuffer;

struct ObjectData{
	mat4 model;
};
layout(std140,set = 1, binding = 0) readonly buffer ObjectBuffer{
    ObjectData objects[];
} objectBuffer;



void main() {
    mat4 modelMatrix = objectBuffer.objects[gl_InstanceIndex].model;
	gl_Position = (camBuffer.renderMatrix * modelMatrix) * vec4(vPosition, 1.0);
	outColor = vColor;
}

#elif defined(STAGE_FRAGMENT)
#pragma shader_stage(fragment)
layout (location = 0) in vec3 inColor;
layout(location = 0) out vec4 outColor;

void main() {
    vec3 resultColor = inColor;
    outColor = vec4(resultColor, 1.0);
}
#endif
