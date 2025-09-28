#version 450

layout(binding = 1) uniform sampler2D texture_sampler[2];
layout(push_constant) uniform PushConstants {
    int selected_texture;
} pushConstants;

layout(location = 0) in vec3 frag_color;
layout(location = 1) in vec2 frag_texture_coordinates;

layout(location = 0) out vec4 out_color;

void main() {
    out_color = texture(texture_sampler[pushConstants.selected_texture], frag_texture_coordinates);
}
