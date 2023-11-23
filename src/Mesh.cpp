#include "Mesh.h"

#include <cassert>
#include <spdlog/spdlog.h>
#include <stb_image.h>

std::unordered_set<std::string> Mesh::loaded_textures_{};

Mesh::MeshEntry::~MeshEntry() {
	if (m_vao != 0) glDeleteVertexArrays(1, &m_vao);
	if (m_vbo != 0) glDeleteBuffers(1, &m_vbo);
	if (m_ebo != 0) glDeleteBuffers(1, &m_ebo);
}

void Mesh::LoadTextures(const std::string& file) {
	auto num_textures = m_scene->mNumMaterials;
	for (unsigned int i = 0; i < num_textures; i++) {		
		auto p_mat = m_scene->mMaterials[i];
		if (0 == p_mat->GetTextureCount(aiTextureType_DIFFUSE)) continue;
		
		aiString path;
		if (p_mat->GetTexture(aiTextureType_DIFFUSE, 0, &path) != AI_SUCCESS) continue;
		spdlog::info("the {} texture path: {}.", i, path.C_Str());

		// load embedded texture
		if (path.C_Str()[0] == '*') {
			const auto texture = m_scene->GetEmbeddedTexture(path.C_Str());
			spdlog::debug("Get embedded texture, width = {}, height = {}, format hint = {}", texture->mWidth, texture->mHeight, texture->achFormatHint);
			if (std::string("png") == texture->achFormatHint) {
				int x, y, channels;
				auto data = stbi_load_from_memory(reinterpret_cast<const stbi_uc*>(texture->pcData), texture->mWidth, &x, &y, &channels, 0);
				spdlog::info("load png texture, width: {}, height: {}, channels: {}", x, y, channels);

				auto& texture = m_textures[i];
				glGenTextures(1, &texture);
				glBindTexture(GL_TEXTURE_2D, texture);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, x, y, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
				glGenerateMipmap(GL_TEXTURE_2D);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

				stbi_image_free(data);
			}
			continue;
		}

		spdlog::warn("Load texture from file is not implemented.");
		continue;
		auto slash_id = file.find_last_of('/');
		if (slash_id == std::string::npos) slash_id = file.find_last_of('\\');

		std::string base;
		if (slash_id == std::string::npos) {
			base = ".";
		}
		else if (slash_id == 0) {
			base = "/";
		}
		else {
			base = file.substr(0, slash_id);
		}

	}
}

void Mesh::LoadFromFile(const std::string& file) {
	Assimp::Importer importer;
	m_scene = importer.ReadFile(file, aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_SortByPType | aiProcess_MakeLeftHanded);

	if (nullptr == m_scene) {
		spdlog::error("Load 3D model failed, path: {}", file);
		return;
	}

	m_mesh_entries.resize(m_scene->mNumMeshes);
	m_textures.resize(m_scene->mNumMaterials);

	ProcessNode(m_scene->mRootNode);

	LoadTextures(file);
}

void Mesh::ProcessNode(const aiNode* node) {
	for (unsigned int i = 0; i < node->mNumMeshes; i++) {
		auto mesh_id = node->mMeshes[i];
		m_mesh_entries[mesh_id].Init(m_scene->mMeshes[mesh_id]);
	}
	for (unsigned int i = 0; i < node->mNumChildren; i++) {
		ProcessNode(node->mChildren[i]);
	}
}

void Mesh::ConvertAiMat4ToGLM(glm::mat4& ans, const aiMatrix4x4& ai_mat) {
	for (int r = 0; r < 4; r++)
		for (int c = 0; c < 4; c++)
			ans[c][r] = ai_mat[r][c];
}

void Mesh::MeshEntry::Init(const aiMesh* mesh) {
	glGenVertexArrays(1, &m_vao);
	glGenBuffers(1, &m_vbo);
	glGenBuffers(1, &m_ebo);
	m_vertices = mesh->mNumVertices;

	glBindVertexArray(m_vao);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	// pos: 3d, normal: 3d, uv: 2d
	glBufferData(GL_ARRAY_BUFFER, sizeof(ai_real) * 8 * m_vertices, nullptr, GL_STATIC_DRAW);

	GLintptr offset = 0;

	// setup position data
	GLintptr segment_length = sizeof(aiVector3D) * m_vertices;
	glBufferSubData(GL_ARRAY_BUFFER, offset, segment_length, mesh->mVertices);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(aiVector3D), 0);
	glEnableVertexAttribArray(0);
	offset += segment_length;

	// setup normal vectors data
	assert(mesh->HasNormals());
	segment_length = sizeof(aiVector3D) * m_vertices;
	glBufferSubData(GL_ARRAY_BUFFER, offset, segment_length, mesh->mNormals);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(aiVector3D), reinterpret_cast<const void*>(offset));
	glEnableVertexAttribArray(1);
	offset += segment_length;

	// setup uv data
	assert(mesh->HasTextureCoords(0));
	segment_length = sizeof(aiVector2D) * m_vertices;
	glBufferSubData(GL_ARRAY_BUFFER, offset, segment_length, mesh->mTextureCoords[0]);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(aiVector2D), reinterpret_cast<const void*>(offset));
	glEnableVertexAttribArray(2);

	// setup vertex indices data
	std::vector<unsigned int> indices;
	for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
		auto face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++) {
			indices.push_back(face.mIndices[j]);
		}
	}
	m_indices = indices.size();
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), indices.data(), GL_STATIC_DRAW);

	m_texture_index = mesh->mMaterialIndex;
}

void Mesh::Draw() const {
	glActiveTexture(GL_TEXTURE0);
	for (const auto& entry : m_mesh_entries) {
		glBindVertexArray(entry.m_vao);
		glBindTexture(GL_TEXTURE_2D, m_textures[entry.m_texture_index]);
		glDrawElements(GL_TRIANGLES, entry.m_indices, GL_UNSIGNED_INT, 0);
	}

	glBindVertexArray(0);
}
