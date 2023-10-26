#pragma once

#include <string>
#include <optional>

#include <spdlog/spdlog.h>
#include <glad/glad.h>

class Shader {
public:
	Shader() = default;
	void Init();
	void Use();
	bool LoadFromFile(const std::string& vert_file, const std::string& frag_file);

private:
	std::optional<GLuint> CompileShader(int shader_enum, const std::string& shader_file);
	GLuint program_id_;
};