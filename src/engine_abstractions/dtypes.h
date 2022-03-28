#pragma once
#include "glad/glad.h"
#include "glm/glm.hpp"

namespace rtre {

	using glm::vec4;
	using glm::vec3;
	using glm::vec2;
	using glm::mat4;
	using glm::mat3;
	using glm::mat2;


	struct Vertex2
	{
		vec2 position;
		vec2 txtCoord;

		Vertex2()
			:
			position(vec2(0, 0)),
			txtCoord(vec2(0, 0))
		{}
		Vertex2(const vec2& pos ,const vec2& txtcoord = vec2(0,0))
			:
			position(pos),
			txtCoord(txtcoord)
		{}

	};

	struct Vertex3
	{
		vec3 position;
		vec2 txtCoord;
		vec3 normal;

		Vertex3() 
			:
			position(vec3(0, 0, 0)),
			txtCoord(vec2(0, 0)),
			normal(vec3(1,1,1))

		{}
		Vertex3(const vec3& pos, const vec2& txtcoord = vec2(0, 0),const vec3& norm = vec3(1,1,1))
			:
			position(pos),
			txtCoord(txtcoord),
			normal(norm)
		{}

	};


	class PointLight {
	public:
		vec3 position = vec3(0, 0, 0);

		vec3 diffuse = vec3(1, 1, 1);
		vec3 specular = vec3(1, 1, 1);

		GLfloat constant = 0.8;
		GLfloat linear = 0.02f;
		GLfloat quadratic = 0.032f;

		PointLight(vec3 pos, vec3 dif, vec3 spec = vec3(1.f, 1.f, 1.f),
			GLfloat cons = 0.8, GLfloat lin = 0.02f, GLfloat quad = 0.032f);

		~PointLight();

		void passStruct(std::shared_ptr<RenderShader> shader, GLuint index) {
			std::string uniformName = "pointLights[" + std::to_string(index) + "]";
			shader->SetUniform(shader->getUnifromID((uniformName + "." + "position").c_str()), position);
			shader->SetUniform(shader->getUnifromID((uniformName + "." + "diffuse").c_str()), diffuse);
			shader->SetUniform(shader->getUnifromID((uniformName + "." + "specular").c_str()), specular);
			shader->SetUniform(shader->getUnifromID((uniformName + "." + "constant").c_str()), constant);
			shader->SetUniform(shader->getUnifromID((uniformName + "." + "linear").c_str()), linear);
			shader->SetUniform(shader->getUnifromID((uniformName + "." + "quadratic").c_str()), quadratic);
		}

	};

	class BasicMesh {
		std::vector<Vertex3> vertices;
		std::vector<GLuint> indices;

	public:
		BasicMesh() {}

		BasicMesh(std::vector<Vertex3>& verts, std::vector<GLuint>& inds)
			:
			vertices(verts),
			indices(inds)
		{
		}

		std::vector<Vertex3> getVertices() {
			return vertices;
		}

		std::vector<GLuint> getIndices() {
			return indices;
		}


		static BasicMesh getVertices(const std::string& path) {
			Assimp::Importer import;
			const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices);

			if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
			{
				throw  std::exception("ERROR::ASSIMP:: Could not load basic sphere mesh.\n");
			}

			aiMesh* mesh = scene->mMeshes[scene->mRootNode->mChildren[0]->mMeshes[0]];

			return processBasicMesh(mesh, scene);
		}

		static BasicMesh processBasicMesh(aiMesh* mesh, const aiScene* scene)
		{
			std::vector<Vertex3> t_vertices;
			std::vector<GLuint> t_indices;

			for (unsigned int i = 0; i < mesh->mNumVertices; i++)
			{
				Vertex3 vertex;
				vertex.position = vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
				if (mesh->mTextureCoords[0])
				{
					vertex.txtCoord = vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
				}
				else {
					vertex.txtCoord = glm::vec2(0.f);
				}

				if (mesh->HasNormals()) {
					vertex.normal = vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
				}
				else {
					vertex.normal = vec3(1);
				}

				t_vertices.push_back(vertex);
			}
			for (unsigned int i = 0; i < mesh->mNumFaces; i++)
			{
				aiFace face = mesh->mFaces[i];

				for (unsigned int j = 0; j < face.mNumIndices; j++)
					t_indices.push_back(face.mIndices[j]);
			}

			BasicMesh t_Mesh(t_vertices, t_indices);

			return t_Mesh;
		}

	};
}

