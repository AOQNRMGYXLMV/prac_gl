#version 330 core

in vec3 normal_out;
in vec3 pos_out;
in vec2 uv_out;

out vec4 FragColor;

uniform vec3 camera_pos;
uniform vec3 light_dir;
uniform vec3 light_color;
uniform vec3 col;
uniform sampler2D diff_tex;

void main() {
	vec3 view = normalize(camera_pos - pos_out);
	vec3 normal = normalize(normal_out);
	vec3 obj_color = texture(diff_tex, uv_out).rgb;
	const float kd = 1.0;
	const float ks = 0.5;
	float ambient = 0.2;
	float diffuse =  kd * max(dot(normal, -light_dir), 0.0);
	vec3 half = normalize(view - light_dir);
	float specular = ks * pow(max(dot(normal, half), 0), 8);

	vec3 res = (diffuse + specular + ambient) * obj_color;

	FragColor = vec4(res, 1.0);
}

