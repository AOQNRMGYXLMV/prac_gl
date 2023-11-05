#version 330 core
// 11
in vec3 normal_out;
in vec3 pos_out;
out vec4 FragColor;

uniform vec3 camera_pos;
uniform vec3 light_dir;
uniform vec3 light_color;
uniform vec3 col;

void main() {
	vec3 view = normalize(camera_pos - pos_out);
	vec3 normal = normalize(normal_out);
	vec3 obj_color = vec3(0.0, 1.0, 0.0);
	vec3 ambient = vec3(0.3);
	vec3 diffuse = obj_color * max(dot(normal, -light_dir), 0.0);
	vec3 res = diffuse + ambient;

	FragColor = vec4(res, 1.0);
}

