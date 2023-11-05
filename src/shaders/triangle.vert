#version 330 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 normal;
// layout (location = 2) in vec2 uv;

out vec3 normal_out;
out vec3 pos_out;

uniform mat4 view;
uniform mat4 projection;

void main() {
	normal_out = normal;
	pos_out = pos;
	gl_Position = projection * view * vec4(pos, 1.0);
}

