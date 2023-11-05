#include "Shader.h"

#include <fstream>
#include <sstream>
#include <glm/gtc/type_ptr.hpp>

void Shader::Init() {
	program_id_ = glCreateProgram();
}

std::optional<GLuint> Shader::CompileShader(int shader_enum, const std::string& shader_file) {
	std::ifstream file_stream;
	file_stream.open(shader_file);
	if (!file_stream.is_open()) {
		spdlog::error("Open vertex shader file {} failed.", shader_file);
		return std::nullopt;
	}

	std::stringstream ss;
	ss << file_stream.rdbuf();
	file_stream.close();
	auto shader = glCreateShader(shader_enum);
	auto shader_str = ss.str();
	auto shader_cstr = shader_str.c_str();
	glShaderSource(shader, 1, &shader_cstr, nullptr);
	glCompileShader(shader);

	int success;
	char info_log[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(shader, sizeof(info_log), nullptr, info_log);
		spdlog::error("Compile shader {} failed: {}", shader_file, info_log);
		glDeleteShader(shader);
		return std::nullopt;
	}

	return shader;
}

bool Shader::LoadFromFile(const std::string& vert_file, const std::string& frag_file) {
	auto vert_shader = CompileShader(GL_VERTEX_SHADER, vert_file);
	if (!vert_shader.has_value()) return false;
	auto frag_shader = CompileShader(GL_FRAGMENT_SHADER, frag_file);
	if (!frag_shader.has_value()) return false;

	glAttachShader(program_id_, vert_shader.value());
	glAttachShader(program_id_, frag_shader.value());
	glLinkProgram(program_id_);

	int success;
	char log_info[512];
	glGetProgramiv(program_id_, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(program_id_, sizeof(log_info), nullptr, log_info);
		spdlog::error("Link shader program error: {}", log_info);
	}

	glDeleteShader(vert_shader.value());
	glDeleteShader(frag_shader.value());

	return static_cast<bool>(success);
}

void Shader::Use() const {
	glUseProgram(program_id_);
}

void Shader::SetUniformMatrix4fv(const std::string& name, const glm::mat4& mat) const {
	auto location = glGetUniformLocation(program_id_, name.c_str());
	if (location >= 0) {
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(mat));
	}
	else {
		spdlog::warn("Get uniform \"{}\" location failed.", name);
	}
}

void Shader::SetUniform3fv(const std::string& name, const glm::vec3& vec) const {
	auto location = glGetUniformLocation(program_id_, name.c_str());
	if (location >= 0) {
		glUniform3fv(location, 1, glm::value_ptr(vec));
	}
	else {
		spdlog::warn("Get uniform \"{}\" location  failed.", name);
	}
}
