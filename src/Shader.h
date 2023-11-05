#pragma once

#include <string>
#include <optional>

#include <glm/glm.hpp>
#include <spdlog/spdlog.h>
#include <glad/glad.h>

class Shader {
public:
	Shader() = default;
	void Init();
	void Use() const;
	bool LoadFromFile(const std::string& vert_file, const std::string& frag_file);
	void SetUniformMatrix4fv(const std::string& name, const glm::mat4& mat) const;
	void SetUniform3fv(const std::string& name, const glm::vec3& vec) const;

private:
	std::optional<GLuint> CompileShader(int shader_enum, const std::string& shader_file);
	GLuint program_id_;
};