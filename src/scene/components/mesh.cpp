#include "mesh.h"
#include "transform.h"


void Mesh::on_ready() {
    vertexBuffer.AllocateBuffer(VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VMA_MEMORY_USAGE_CPU_TO_GPU, vertices.size(), sizeof(Vertex));
    indexBuffer.AllocateBuffer(VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VMA_MEMORY_USAGE_CPU_TO_GPU, indicies.size(),sizeof(uint32_t));
    modelBuffer.AllocateBuffer(VK_BUFFER_USAGE_STORAGE_BUFFER_BIT, VMA_MEMORY_USAGE_CPU_TO_GPU, 100, sizeof(glm::mat4));
    vertexBuffer.UploadBufferData(vertices.data());
    indexBuffer.UploadBufferData(indicies.data());
}


void Mesh::update(const double &delta){
    //modelBuffer.UploadBufferData(&modelMatrix);
}
