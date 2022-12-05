#include "model_loader.h"
#include "tiny_gltf.h"
#include "logger.h"
#include "components/mesh.h"
#include "components/transform.h"




void ModelLoader::LoadFile(const std::string &file, std::vector<Entity>& outEntities) {
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
    for(auto node : model.nodes) {
        Entity entity;
        if(node.mesh != -1) {
            entity.name = node.name;
            entity.AddComponent<Transform>();
            entity.AddComponent<Mesh>();
            auto* transform = entity.GetComponent<Transform>();
            transform->position = glm::vec3(node.translation[0], node.translation[1], node.translation[2]);
            transform->scale = glm::vec3(node.scale[0], node.scale[1], node.scale[2]);
            transform->rotation = glm::vec3(node.rotation[0], node.rotation[1], node.rotation[2]);

            for(auto primitive : model.meshes[node.mesh].primitives) {
                auto& verticies = entity.GetComponent<Mesh>()->vertices;
                auto& indicies = entity.GetComponent<Mesh>()->indicies;
                if(primitive.attributes.find("POSITION") != primitive.attributes.end())  {
                    const tinygltf::Accessor& accessor = model.accessors[primitive.attributes.find("POSITION")->second];
                    const tinygltf::BufferView& bufferView = model.bufferViews[accessor.bufferView];

                    const tinygltf::Buffer& buffer = model.buffers[bufferView.buffer];
                    const float* positions = reinterpret_cast<const float*>(&buffer.data[bufferView.byteOffset + accessor.byteOffset]);
                    verticies.resize(accessor.count);
                    for (size_t i = 0; i < accessor.count; ++i) {
                    verticies[i].position = glm::vec3(positions[i * 3 + 0], positions[i * 3 + 1], positions[i * 3 + 2]);
                    }
                }
                if (primitive.attributes.find("NORMAL") != primitive.attributes.end()) {
                    const tinygltf::Accessor& accessor = model.accessors[primitive.attributes.find("NORMAL")->second];
                    const tinygltf::BufferView& bufferView = model.bufferViews[accessor.bufferView];

                    const tinygltf::Buffer& buffer = model.buffers[bufferView.buffer];
                    const float* normals = reinterpret_cast<const float*>(&buffer.data[bufferView.byteOffset + accessor.byteOffset]);
                    for (size_t i = 0; i < accessor.count; ++i) {
                    verticies[i].normals = glm::vec3(normals[i * 3 + 0], normals[i * 3 + 1], normals[i * 3 + 2]);
                    }
                }
                if(primitive.indices > 0) {
                    const tinygltf::Accessor& accessor = model.accessors[primitive.indices];
                    const tinygltf::BufferView& bufferView = model.bufferViews[accessor.bufferView];

                    const tinygltf::Buffer& buffer = model.buffers[bufferView.buffer];
                    const uint32_t* indices = reinterpret_cast<const uint32_t*>(&buffer.data[bufferView.byteOffset + accessor.byteOffset]);
                    indicies = {};
                    indicies.resize(accessor.count);

                    switch (accessor.componentType) {
                        case TINYGLTF_PARAMETER_TYPE_UNSIGNED_INT: {
                            uint32_t *buf = new uint32_t[accessor.count];
                            memcpy(buf, &buffer.data[accessor.byteOffset + bufferView.byteOffset], accessor.count * sizeof(uint32_t));
                            for (size_t index = 0; index < accessor.count; index++) {
                                indicies[index] = buf[index];
                            }
                            delete[] buf;
                            break;
                        }
                        case TINYGLTF_PARAMETER_TYPE_UNSIGNED_SHORT: {
                            uint16_t *buf = new uint16_t[accessor.count];
                            memcpy(buf, &buffer.data[accessor.byteOffset + bufferView.byteOffset], accessor.count * sizeof(uint16_t));
                            for (size_t index = 0; index < accessor.count; index++) {
                                indicies[index] = buf[index];
                            }
                            delete[] buf;
                            break;
                        }
                        case TINYGLTF_PARAMETER_TYPE_UNSIGNED_BYTE: {
                            uint8_t *buf = new uint8_t[accessor.count];
                            memcpy(buf, &buffer.data[accessor.byteOffset + bufferView.byteOffset], accessor.count * sizeof(uint8_t));
                            for (size_t index = 0; index < accessor.count; index++) {
                                indicies[index] = buf[index];
                            }
                            delete[] buf;
                            break;
                        }
                        default:
                            std::cerr << "Index component type " << accessor.componentType << " not supported!" << std::endl;
                            return;
                    }
                }
            }
            outEntities.push_back(entity);
        }
    }
}
