#version 450

layout(set = 0, binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 proj;
} ubo;

layout(std430, set = 1, binding = 0) readonly buffer Transforms {
	mat4 transforms[];
};


layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in uint inID;

layout(location = 0) out vec3 fragColor;

void main() {
    //gl_Position = ubo.proj * ubo.view * ubo.model * vec4(inPosition, 1.0);
    gl_Position = ubo.proj * ubo.view * transforms[inID] * vec4(inPosition, 1.0);
    //gl_Position =  transforms[inID] * vec4(inPosition, 1.0);
    fragColor = inColor;
}