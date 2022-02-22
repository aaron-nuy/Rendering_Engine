#pragma once
#include <vector>
#include "mesh.h"
#include "../engine_abstractions/shader.h"
#include "../rtre_base.h"

namespace rtre {

    class Model {

        std::vector<std::shared_ptr<Mesh>> m_Meshes;
        std::vector<std::shared_ptr<Sampler2D>> loaded_Textures;
        std::shared_ptr<RenderShader> m_Shader;
        std::string directory;
        GLuint slotCounter = 0;

        void processNode(aiNode* node, const aiScene* scene)
        {
            for (unsigned int i = 0; i < node->mNumMeshes; i++)
            {
                aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
                processMesh(mesh, scene);
            }
            for (unsigned int i = 0; i < node->mNumChildren; i++)
            {
                processNode(node->mChildren[i], scene);
            }
        }
        void processMesh(aiMesh* mesh, const aiScene* scene)
        {
            std::vector<Vertex3> vertices;
            std::vector<GLuint> indices;
            std::vector<std::shared_ptr<Sampler2D>> textures;

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

                vertices.emplace_back(vertex);
            }
            for (unsigned int i = 0; i < mesh->mNumFaces; i++)
            {
                aiFace face = mesh->mFaces[i];

                for (unsigned int j = 0; j < face.mNumIndices; j++)
                    indices.push_back(face.mIndices[j]);
            }
            aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

            std::vector<std::shared_ptr<Sampler2D>> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, rTdiffuse);
            textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
            std::vector<std::shared_ptr<Sampler2D>> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, rTspecular);
            textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
            std::vector<std::shared_ptr<Sampler2D>> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, rTnormal);
            textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
            std::vector<std::shared_ptr<Sampler2D>> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, rTheight);
            textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());
            

            m_Meshes.emplace_back(std::make_shared<Mesh>(vertices, indices, textures));
        }

        std::vector<std::shared_ptr<Sampler2D>> loadMaterialTextures(aiMaterial* mat, aiTextureType type, Senum textureType)
        {
            std::vector<std::shared_ptr<Sampler2D>> textures;
            for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
            {
                aiString str;
                mat->GetTexture(type, i, &str);

                bool skip = false;
                for (unsigned int j = 0; j < loaded_Textures.size(); j++)
                {
                    if (std::strcmp(loaded_Textures[j]->path().data(), str.C_Str()) == 0)
                    {
                        textures.push_back(loaded_Textures[j]);
                        skip = true;
                        break;
                    }
                }
                if (!skip)
                {
                    std::string crntpath = directory + "/" + str.C_Str();
                    std::shared_ptr<Sampler2D> texture = std::make_shared<Sampler2D>(crntpath.c_str(), slotCounter++);
                    texture->setType(textureType);
                    texture->setPath(str.C_Str());
                    textures.push_back(texture);
                    loaded_Textures.push_back(texture);
                }
            }
            return textures;
        }

    public:

        Model() {
            m_Shader = d3Shader;
        }
        Model(std::shared_ptr<RenderShader> shader)
        {
            m_Shader = shader;
        }
        Model(const std::string& path, std::shared_ptr<RenderShader> shader)
        {
            m_Shader = shader;
            loadModel(path);
        }
        Model(const std::string& path)
        {
            m_Shader = d3Shader;
           
            loadModel(path);

        }
        Model(const std::string& path,const char* vertexShaderPath, const char* fragShaderPath,
            const char* geometryShaderPath = nullptr)
        {
            m_Shader = std::make_shared<RenderShader>(vertexShaderPath, fragShaderPath, geometryShaderPath);
            loadModel(path);
        }
        Model(const char* vertexShaderPath, const char* fragShaderPath,
            const char* geometryShaderPath = nullptr)
        {
            m_Shader = std::make_shared<RenderShader>(vertexShaderPath, fragShaderPath, geometryShaderPath);
        }


        void loadModel(const std::string& path)
        {
            m_Meshes.clear();
            Assimp::Importer import;
            const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

            if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
            {
                std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
                return;
            }
            directory = path.substr(0, path.find_last_of('/'));


            processNode(scene->mRootNode, scene);

        }

        void draw() {

            glm::mat4 identity = glm::mat4(1.0f);
            m_Shader->activate();

            m_Shader->SetUniform(m_Shader->getUnifromID("perspectiveM"),camera.perspective() );
            m_Shader->SetUniform(m_Shader->getUnifromID("transformM"), identity);
            m_Shader->SetUniform(m_Shader->getUnifromID("rotateM"), identity);
            m_Shader->SetUniform(m_Shader->getUnifromID("scaleV"), 1.1f);
            m_Shader->SetUniform(m_Shader->getUnifromID("aspectRatioV"), 1.f);


            for (auto& mesh : m_Meshes)
                mesh->draw(rtre::d3Shader);
        }
    };
}