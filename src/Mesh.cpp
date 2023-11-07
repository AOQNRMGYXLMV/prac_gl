#include "Mesh.h"

#include <spdlog/spdlog.h>
#include <stb_image.h>

std::unordered_set<std::string> Mesh::loaded_textures_{};
unsigned int Mesh::texture_{ 0 };

void Mesh::ConvertAiMat4ToGLM(glm::mat4& ans, const aiMatrix4x4& ai_mat) {
	for (int r = 0; r < 4; r++)
		for (int c = 0; c < 4; c++)
			ans[c][r] = ai_mat[r][c];
}

void Mesh::Load(const aiScene* scene, const aiMesh* mesh, const aiMatrix4x4& ai_transform) {
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

	if (mesh->HasNormals()) {
		unsigned int normal_vbo;
		glGenBuffers(1, &normal_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, normal_vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(aiVector3D) * mesh->mNumVertices, mesh->mNormals, GL_STATIC_DRAW);
	
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);
		glEnableVertexAttribArray(1);
	}

	// load texture
	auto material = scene->mMaterials[mesh->mMaterialIndex];
	auto diff_count = material->GetTextureCount(aiTextureType_DIFFUSE);
	spdlog::info("diffuse textures count: {}.", diff_count);
	for (unsigned int i = 0; i < diff_count; i++) {
		aiString path;
		if (material->GetTexture(aiTextureType_DIFFUSE, i, &path) != AI_SUCCESS) {
			spdlog::error("Get the {}-th texture failed.", i);
			break;
		}

		if (loaded_textures_.count(std::string(path.C_Str()))) continue;

		spdlog::info("the {} texture path: {}.", i, path.C_Str());
		if (path.C_Str()[0] == '*') {
			const auto texture = scene->GetEmbeddedTexture(path.C_Str());
			spdlog::info("Get embedded texture, width = {}, height = {}, format hint = {}", texture->mWidth, texture->mHeight, texture->achFormatHint);
			if (std::string("png") == texture->achFormatHint) {
				int x, y, channels;
				auto data = stbi_load_from_memory(reinterpret_cast<const stbi_uc*>(texture->pcData), texture->mWidth, &x, &y, &channels, 0);
				spdlog::info("load png texture, width: {}, height: {}, channels: {}", x, y, channels);

				glGenTextures(1, &texture_);
				glBindTexture(GL_TEXTURE_2D, texture_);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, x, y, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
				glGenerateMipmap(GL_TEXTURE_2D);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

				stbi_image_free(data);
				loaded_textures_.emplace(path.C_Str());
			}
		}
	}

	if (mesh->HasTextureCoords(0)) {
		unsigned int uv_vbo;
		glGenBuffers(1, &uv_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, uv_vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(aiVector3D) * mesh->mNumVertices, mesh->mTextureCoords[0], GL_STATIC_DRAW);

		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);
		glEnableVertexAttribArray(2);
	}
}

void Mesh::Draw() const {
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_);
	glBindVertexArray(vao_);
	glDrawElements(GL_TRIANGLES, num_indices_, GL_UNSIGNED_INT, 0);
	
	glBindVertexArray(0);
}
