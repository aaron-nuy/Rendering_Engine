#pragma once
#include <vector>
#include "mesh.h"
#include "../engine_abstractions/shader.h"
#include "../rtre_base.h"

namespace rtre {
    


    class AModel {
    protected:
        std::shared_ptr<RenderShader> m_Shader;

        mat4 m_Rotate = mat4(1.f);
        vec4 m_Scale = vec4(1);
        vec3 m_Position = vec3(0);
        virtual void draw() = 0;
        
    public:
        // Angle must be in radians
        virtual void rotate(GLfloat angle, vec3 axis = vec3(0.f, 1.f, 0.f)) final {
            m_Rotate = glm::rotate(angle, axis);
        }
        virtual void scale(GLfloat scale) final {
            m_Scale = vec4(scale, scale, scale,1);
        }
        virtual void scale(glm::vec3 scalev) final {
            m_Scale = vec4(scalev, 1);
        }
        virtual void setPosition(vec3 coords) final {
            m_Position = coords;
        }
    };

    class Model : public AModel {


        std::vector<std::shared_ptr<Mesh>> m_Meshes;
        std::vector<std::shared_ptr<Sampler2D>> loaded_Textures;
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

                if(mesh->HasNormals()){
                    vertex.normal = vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
                }
                else {
                    vertex.normal = vec3(1);
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
            const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices);

            if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
            {
                std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
                return;
            }
            directory = path.substr(0, path.find_last_of('/'));


            processNode(scene->mRootNode, scene);

        }

        void draw() override {

            m_Shader->activate();

            m_Shader->SetUniform(m_Shader->getUnifromID("perspectiveM"),camera.perspective() );
            m_Shader->SetUniform(m_Shader->getUnifromID("rotateM"), m_Rotate);
            m_Shader->SetUniform(m_Shader->getUnifromID("scaleV"), m_Scale);
            m_Shader->SetUniform(m_Shader->getUnifromID("positionV"), m_Position);
            m_Shader->SetUniform(m_Shader->getUnifromID("aspectRatioV"), aspectRatio);

            sendLights(m_Shader);

            for (auto& mesh : m_Meshes)
                mesh->draw(m_Shader);
        }
    };

    class BModel : public AModel {
    protected:
        std::shared_ptr<Mesh> m_Mesh;
        std::shared_ptr<RenderShader> m_Shader;
        vec3 m_Proportions = vec3(0);

        BModel(){
            m_Mesh = std::make_shared<Mesh>();
        }
    public:
        virtual void draw() final override {

            m_Shader->activate();

            m_Shader->SetUniform(m_Shader->getUnifromID("perspectiveM"), camera.perspective());
            m_Shader->SetUniform(m_Shader->getUnifromID("rotateM"), m_Rotate);
            m_Shader->SetUniform(m_Shader->getUnifromID("scaleV"), m_Scale);
            m_Shader->SetUniform(m_Shader->getUnifromID("positionV"), m_Position);
            m_Shader->SetUniform(m_Shader->getUnifromID("aspectRatioV"), aspectRatio);

            sendLights(m_Shader);

            m_Mesh->draw(m_Shader);
        }
    };

    class Cube : public BModel {

        static const std::vector<Vertex3> s_Vertices;

        static const std::vector<GLuint> s_Indices;

    public:


        Cube() {
            m_Shader = d3Shader;
            m_Mesh->load(s_Vertices, s_Indices, std::vector<std::shared_ptr<Sampler2D>> {texturePlaceholder});
        }

        Cube(const vec3& position, const vec3& scale = vec3(1))
        {
            m_Position = position;
            m_Scale = vec4(scale,1);

            m_Shader = d3Shader;
            m_Mesh->load(s_Vertices, s_Indices, std::vector<std::shared_ptr<Sampler2D>> {texturePlaceholder});
            
        }
        Cube(std::shared_ptr<RenderShader> shader, 
            const vec3& position = vec3(0), const vec3& scale = vec3(1))
        {
            m_Position = position;
            m_Scale = vec4(scale, 1);

            m_Mesh->load(s_Vertices, s_Indices, std::vector<std::shared_ptr<Sampler2D>> {texturePlaceholder});
            m_Shader = shader;
        }
        Cube(const std::string& texture, std::shared_ptr<RenderShader> shader, 
            const vec3& position = vec3(0), const vec3& scale = vec3(1))
        {
            m_Position = position;
            m_Scale = vec4(scale, 1);

            m_Shader = shader;
            m_Mesh->load(s_Vertices, s_Indices, std::vector<std::shared_ptr<Sampler2D>> {std::make_shared<Sampler2D>(texture.c_str(), 1)});

        }
        Cube(const std::string& texture,const vec3& position = vec3(0), const vec3& scale = vec3(1))
        {
            m_Shader = d3Shader;
            m_Position = position;
            m_Scale = vec4(scale, 1);
            m_Mesh->load(s_Vertices, s_Indices, std::vector<std::shared_ptr<Sampler2D>> {std::make_shared<Sampler2D>(texture.c_str(), 1)});

        }
        Cube(const std::string& texture, const char* vertexShaderPath, const char* fragShaderPath,
            const char* geometryShaderPath = nullptr, const vec3& position = vec3(0), const vec3& scale = vec3(1))
        {
            m_Position = position;
            m_Scale = vec4(scale, 1);
            m_Shader = std::make_shared<RenderShader>(vertexShaderPath, fragShaderPath, geometryShaderPath);
            m_Mesh->load(s_Vertices, s_Indices, std::vector<std::shared_ptr<Sampler2D>> {std::make_shared<Sampler2D>(texture.c_str(), 1)});
        }
        Cube(const char* vertexShaderPath, const char* fragShaderPath,
            const char* geometryShaderPath = nullptr, const vec3& position = vec3(0), const vec3& scale = vec3(1))
        {
            m_Position = position;
            m_Scale = vec4(scale, 1);
            m_Shader = std::make_shared<RenderShader>(vertexShaderPath, fragShaderPath, geometryShaderPath);
            m_Mesh->load(s_Vertices, s_Indices, std::vector<std::shared_ptr<Sampler2D>> {texturePlaceholder});
        }

    };

    class Sphere : public BModel{

        std::vector<Vertex3> s_Vertices = sphereVertices.getVertices();
        std::vector<GLuint> s_Indices = sphereVertices.getIndices();

    public:

        Sphere() {
            m_Shader = d3Shader;
            m_Mesh->load(s_Vertices, s_Indices, std::vector<std::shared_ptr<Sampler2D>> {texturePlaceholder});
        }
        Sphere(const vec3& position, const vec3& scale = vec3(1))
        {
            m_Position = position;
            m_Scale = vec4(scale, 1);

            m_Shader = d3Shader;
            m_Mesh->load(s_Vertices, s_Indices, std::vector<std::shared_ptr<Sampler2D>> {texturePlaceholder});

        }
        Sphere(std::shared_ptr<RenderShader> shader,
            const vec3& position = vec3(0), const vec3& scale = vec3(1))
        {
            m_Position = position;
            m_Scale = vec4(scale, 1);

            m_Mesh->load(s_Vertices, s_Indices, std::vector<std::shared_ptr<Sampler2D>> {texturePlaceholder});
            m_Shader = shader;
        }
        Sphere(const std::string& texture, std::shared_ptr<RenderShader> shader,
            const vec3& position = vec3(0), const vec3& scale = vec3(1))
        {
            m_Position = position;
            m_Scale = vec4(scale, 1);

            m_Shader = shader;
            m_Mesh->load(s_Vertices, s_Indices, std::vector<std::shared_ptr<Sampler2D>> {std::make_shared<Sampler2D>(texture.c_str(), 1)});

        }
        Sphere(const std::string& texture, const vec3& position = vec3(0), const vec3& scale = vec3(1))
        {
            m_Shader = d3Shader;
            m_Position = position;
            m_Scale = vec4(scale, 1);
            m_Mesh->load(s_Vertices, s_Indices, std::vector<std::shared_ptr<Sampler2D>> {std::make_shared<Sampler2D>(texture.c_str(), 1)});

        }
        Sphere(const std::string& texture, const char* vertexShaderPath, const char* fragShaderPath,
            const char* geometryShaderPath = nullptr, const vec3& position = vec3(0), const vec3& scale = vec3(1))
        {
            m_Position = position;
            m_Scale = vec4(scale, 1);
            m_Shader = std::make_shared<RenderShader>(vertexShaderPath, fragShaderPath, geometryShaderPath);
            m_Mesh->load(s_Vertices, s_Indices, std::vector<std::shared_ptr<Sampler2D>> {std::make_shared<Sampler2D>(texture.c_str(), 1)});
        }
        Sphere(const char* vertexShaderPath, const char* fragShaderPath,
            const char* geometryShaderPath = nullptr, const vec3& position = vec3(0), const vec3& scale = vec3(1))
        {
            m_Position = position;
            m_Scale = vec4(scale, 1);
            m_Shader = std::make_shared<RenderShader>(vertexShaderPath, fragShaderPath, geometryShaderPath);
            m_Mesh->load(s_Vertices, s_Indices, std::vector<std::shared_ptr<Sampler2D>> {texturePlaceholder});
        }


    };

    const std::vector<Vertex3> Cube::s_Vertices = {
        Vertex3{glm::vec3(-0.5f,-0.5f, 0.5f), glm::vec2(1, 1), glm::vec3(0.f,-1.f,0.f)},	   //0  
        Vertex3{glm::vec3( 0.5f,-0.5f, 0.5f), glm::vec2(0, 1), glm::vec3(0.f,-1.f,0.f)},    //1 
        Vertex3{glm::vec3( 0.5f,-0.5f,-0.5f), glm::vec2(0, 0), glm::vec3(0.f,-1.f,0.f)},    //2  Bottom
        Vertex3{glm::vec3(-0.5f,-0.5f,-0.5f), glm::vec2(1, 0), glm::vec3(0.f,-1.f,0.f)},    //3 
                                                      
        Vertex3{glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec2(0, 1), glm::vec3(-1.f,0.f,0.f)},     //0 Left 4
        Vertex3{glm::vec3(-0.5f,-0.5f, 0.5f), glm::vec2(0, 0), glm::vec3(-1.f,0.f,0.f)},		//1 5
        Vertex3{glm::vec3(-0.5f,-0.5f,-0.5f), glm::vec2(1, 0), glm::vec3(-1.f,0.f,0.f)},		//2 6
        Vertex3{glm::vec3(-0.5f, 0.5f,-0.5f), glm::vec2(1, 1), glm::vec3(-1.f,0.f,0.f)},		//3 7
                                                           
        Vertex3{glm::vec3(-0.5f,-0.5f,-0.5f), glm::vec2(1, 0), glm::vec3(0.f,0.f,-1.f)}, 	//0 8
        Vertex3{glm::vec3( 0.5f,-0.5f,-0.5f), glm::vec2(0, 0), glm::vec3(0.f,0.f,-1.f)},		//1 9
        Vertex3{glm::vec3( 0.5f, 0.5f,-0.5f), glm::vec2(0, 1), glm::vec3(0.f,0.f,-1.f)},		//2 Back 10
        Vertex3{glm::vec3(-0.5f, 0.5f,-0.5f), glm::vec2(1, 1), glm::vec3(0.f,0.f,-1.f)},		//3 11
                                                  
        Vertex3{glm::vec3( 0.5f, 0.5f,-0.5f), glm::vec2(1, 1), glm::vec3(1.f,0.f,0.f)}, 	//0 12
        Vertex3{glm::vec3( 0.5f,-0.5f,-0.5f), glm::vec2(1, 0), glm::vec3(1.f,0.f,0.f)},		//1 13
        Vertex3{glm::vec3( 0.5f,-0.5f, 0.5f), glm::vec2(0, 0), glm::vec3(1.f,0.f,0.f)},		//2 14
        Vertex3{glm::vec3( 0.5f, 0.5f, 0.5f), glm::vec2(0, 1), glm::vec3(1.f,0.f,0.f)},		//3 Right 15
                                                    
        Vertex3{glm::vec3( 0.5f, 0.5f, 0.5f), glm::vec2(1, 1), glm::vec3(0.f,0.f,1.f)},     //0	16	  																										  
        Vertex3{glm::vec3( 0.5f,-0.5f, 0.5f), glm::vec2(1, 0), glm::vec3(0.f,0.f,1.f)},		//1 17
        Vertex3{glm::vec3(-0.5f,-0.5f, 0.5f), glm::vec2(0, 0), glm::vec3(0.f,0.f,1.f)},		//2 18
        Vertex3{glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec2(0, 1), glm::vec3(0.f,0.f,1.f)},		//3 Front 19
                                                        
        Vertex3{glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec2(0, 0), glm::vec3(0.f,1.f,0.f)},		//0 Top 20
        Vertex3{glm::vec3(-0.5f, 0.5f,-0.5f), glm::vec2(0, 1), glm::vec3(0.f,1.f,0.f)},		//1 21
        Vertex3{glm::vec3( 0.5f, 0.5f,-0.5f), glm::vec2(1, 1), glm::vec3(0.f,1.f,0.f)},		//2 22
        Vertex3{glm::vec3( 0.5f, 0.5f, 0.5f), glm::vec2(1, 0), glm::vec3(0.f,1.f,0.f)}		//3 23
    };
    const std::vector<GLuint> Cube::s_Indices = {
        2,1,0,
        0,3,2, // Bottom

        6,5,4,
        4,7,6, // Left

        10,9,8,
        8,11,10, // Back

        14,13,12,
        12,15,14, // Right

        18,17,16,
        16,19,18, // Front

        22,21,20,
        20,23,22  // Top

    };

}