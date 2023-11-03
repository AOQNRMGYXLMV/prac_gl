#include "Mesh.h"

void Mesh::ConvertAiMat4ToGLM(glm::mat4& ans, const aiMatrix4x4& ai_mat) {
	for (int r = 0; r < 4; r++)
		for (int c = 0; c < 4; c++)
			ans[c][r] = ai_mat[r][c];
}

void Mesh::Load(const aiMesh* mesh, const aiMatrix4x4& ai_transform) {
	ConvertAiMat4ToGLM(transform_, ai_transform);

	glGenVertexArrays(1, &vao_);
	glGenBuffers(1, &vbo_);
	glGenBuffers(1, &ebo_);

	glBindVertexArray(vao_);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_);
	glBufferData(GL_ARRAY_BUFFER, sizeof(aiVector3D) * (mesh->mNumVertices), mesh->mVertices, GL_STATIC_DRAW);

	std::vector<unsigned int> indices;
	for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
		auto face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++) {
			indices.push_back(face.mIndices[j]);
		}
	}
	num_indices_ = indices.size();
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), indices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);
	glEnableVertexAttribArray(0);
}

void Mesh::Draw() const {
	glBindVertexArray(vao_);
	glDrawElements(GL_TRIANGLES, num_indices_, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}
