#pragma once

#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Mesh {
public:
	Mesh() = default;
	void LoadFromFile(const std::string& file);
	void Draw() const;
	static std::unordered_set<std::string> loaded_textures_;

private:
	void ProcessNode(const aiNode* node);
	void LoadTextures(const std::string& file);

	class MeshEntry {
	public:
		MeshEntry() = default;
		void Init(const aiMesh* mesh); // generate vao/vbo/ebo
		~MeshEntry();

		int m_vertices = 0;
		int m_indices = 0;
		GLuint m_vao = 0;
		GLuint m_vbo = 0;
		GLuint m_ebo = 0;
		int m_texture_index = -1;
	};

	const aiScene* m_scene;
	std::vector<MeshEntry> m_mesh_entries;
	std::vector<GLuint> m_textures;

	static void ConvertAiMat4ToGLM(glm::mat4& ans, const aiMatrix4x4& ai_mat);
};
