#include "mesh.h"
#include "transform.h"
#include "entity.h"


void Mesh::on_ready() {
    uint32_t vertexSize = 0;
    if(vertexSize > 0){
        vertexBuffer.AllocateBuffer(VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VMA_MEMORY_USAGE_CPU_TO_GPU, vertices.size() * vertexSize);
    }
    else {
        vertexBuffer.AllocateBuffer(VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VMA_MEMORY_USAGE_CPU_TO_GPU, vertices.size() * sizeof(Vertex));
    }
    indexBuffer.AllocateBuffer(VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VMA_MEMORY_USAGE_CPU_TO_GPU, indicies.size() * sizeof(uint32_t));
    modelBuffer.AllocateBuffer(VK_BUFFER_USAGE_STORAGE_BUFFER_BIT, VMA_MEMORY_USAGE_CPU_TO_GPU, 100 * sizeof(glm::mat4));
    vertexBuffer.UploadBufferData(vertices.data());
    indexBuffer.UploadBufferData(indicies.data());
}


void Mesh::update(const double &delta){
    if(entity != nullptr) {
        if (Transform* trans = entity->GetComponent<Transform>(); trans != nullptr) {
            modelMatrix = trans->GetTransformMatrix();
        }
    }
    //modelBuffer.UploadBufferData(&modelMatrix);
}
