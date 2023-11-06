#pragma once

#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <assimp/scene.h>

class Mesh {
public:
	Mesh() = default;
	void Load(const aiScene* scene, const aiMesh* mesh, const aiMatrix4x4& ai_transform);
	void Draw() const;

private:
	static void ConvertAiMat4ToGLM(glm::mat4& ans, const aiMatrix4x4& ai_mat);
	glm::mat4 transform_;
	GLuint vao_;
	GLuint vbo_;
	GLuint ebo_;
	int num_indices_;
};
