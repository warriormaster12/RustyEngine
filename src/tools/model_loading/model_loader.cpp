#include "model_loader.h"
#include "tiny_gltf.h"
#include "logger.h"
#include "components/mesh.h"





void ModelLoader::LoadFile(const std::string &file, Entity& outEntity) {
    tinygltf::TinyGLTF loader;
    std::string err;
    std::string warn;
    tinygltf::Model model;
    bool ret = loader.LoadASCIIFromFile(&model, &err, &warn, file);

    if (!warn.empty()) {
        ENGINE_WARN("Warn: {0}", warn.c_str());
    }

    if (!err.empty()) {
        ENGINE_ERROR("Warn: {0}", err.c_str());
    }

    if (!ret) {
        ENGINE_ERROR("Failed to parse glTF");
    }
    for(auto mesh : model.meshes) {
        for(auto primitive : mesh.primitives) {
            outEntity.AddComponent<Mesh>();
            if(primitive.attributes.find("POSITION") != primitive.attributes.end())  {
                const tinygltf::Accessor& accessor = model.accessors[primitive.attributes["POSITION"]];
                const tinygltf::BufferView& bufferView = model.bufferViews[accessor.bufferView];

                const tinygltf::Buffer& buffer = model.buffers[bufferView.buffer];
                const float* positions = reinterpret_cast<const float*>(&buffer.data[bufferView.byteOffset + accessor.byteOffset]);
                outEntity.GetComponent<Mesh>()->vertices = {};
                outEntity.GetComponent<Mesh>()->vertices.resize(accessor.count);
                for (size_t i = 0; i < accessor.count; ++i) {
                   outEntity.GetComponent<Mesh>()->vertices[i].position = glm::vec3(positions[i * 3 + 0], positions[i * 3 + 1], positions[i * 3 + 2]);
                }
            }
            if(primitive.indices > 0) {
                const tinygltf::Accessor& accessor = model.accessors[primitive.indices];
                const tinygltf::BufferView& bufferView = model.bufferViews[accessor.bufferView];

                const tinygltf::Buffer& buffer = model.buffers[bufferView.buffer];
                const uint32_t* indices = reinterpret_cast<const uint32_t*>(&buffer.data[bufferView.byteOffset + accessor.byteOffset]);
                outEntity.GetComponent<Mesh>()->indicies = {};
                // outEntity.GetComponent<Mesh>()->indicies.resize(accessor.count);
                // for (size_t i = 0; i < accessor.count; ++i) {
                //     outEntity.GetComponent<Mesh>()->indicies[i] = indices[i * 3];
                // }
            }
        }
    }
}