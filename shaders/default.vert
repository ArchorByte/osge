#version 450

layout(binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 projection;
} object;

layout(location = 0) in vec2 position_input;
layout(location = 1) in vec3 color_input;
layout(location = 0) out vec3 frag_color;

void main() {
    gl_Position = object.projection * object.view * object.model * vec4(position_input, 0.0, 1.0);
    frag_color = color_input;
}
