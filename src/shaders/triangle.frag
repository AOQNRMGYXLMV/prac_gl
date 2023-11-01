#version 330 core

in vec3 oCol;

out vec4 FragColor;

void main() {
	// FragColor = vec4(0.0f, 1.0f, 0.0f, 1.0f);
	FragColor = vec4(oCol, 1.0f);
}

