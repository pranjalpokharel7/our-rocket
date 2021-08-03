#include "./renderer.h"
#include "./mesh.hpp"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Model{
    private:
        std::vector<Texture> textures_loaded;
        std::vector<Mesh> meshes;
        std::string directory;

        void load_model(std::string path);
        void process_node(aiNode *node, const aiScene *scene);
        Mesh process_mesh(aiMesh *mesh, const aiScene *scene, unsigned int mesh_index);
        std::vector<Texture> load_material_textures(aiMaterial *mat,
                                                    aiTextureType type,
                                                    std::string type_name);
        unsigned int texture_from_file(const char* file_name);
    public:
        Model();
        Model(const char *path);
        void draw_model(ShaderProgram &shader_program);
};
