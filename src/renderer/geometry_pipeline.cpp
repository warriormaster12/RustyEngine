#include "geometry_pipeline.h"
#include "glm/gtc/matrix_transform.hpp"
#include "components/mesh.h"
#include "components/transform.h"



Pipeline testPipeline;


void GeometryPipeline::Prepare(Camera* camera /*= nullptr*/) {
    testPipeline.depthEnabled = true;
    PipelineBuilder::BuildGraphicsPipeline({"shaders/simple_triangle.vert.spv","shaders/simple_triangle.frag.spv"}, {VK_SHADER_STAGE_VERTEX_BIT, VK_SHADER_STAGE_FRAGMENT_BIT}, testPipeline);
    if(camera != nullptr) {
        testPipeline.descriptorSets[0].GenerateDescriptorBuffers(camera->cameraBuffer.descBuffInfo);
    }
    testPipeline.descriptorSets[1].GenerateDescriptorBuffers(entities[0]->GetComponent<Mesh>()->modelBuffer.descBuffInfo);
}

void GeometryPipeline::Update(Camera* camera) {
    VulkanRBE::currentPipeline = &testPipeline;
    VulkanRBE::clearColor[0] = 0.1f;
    VulkanRBE::clearColor[1] = 0.1f;
    VulkanRBE::clearColor[2] = 0.1f;
    VulkanRBE::clearColor[3] = 1.0f;
    VulkanRBE::BeginRenderLayer();
    std::vector<glm::mat4> modelMatricies;
    for(int i=0; i < entities.size(); i++) {
        auto entity = entities[i];
        if (entity->GetComponent<Transform>() != nullptr) {
            modelMatricies.push_back(entity->GetComponent<Transform>()->GetModelMatrix());
        }
        entities[0]->GetComponent<Mesh>()->modelBuffer.UploadBufferData(modelMatricies.data());
        camera->renderMatrix = camera->GetProjectionMatrix() * camera->GetViewMatrix();
        VulkanRBE::BindVertexBuffer(0, 1, entity->GetComponent<Mesh>()->vertexBuffer);
        VulkanRBE::BindIndexBuffer(entity->GetComponent<Mesh>()->indexBuffer);
        VulkanRBE::DrawIndexed(entity->GetComponent<Mesh>()->indicies.size(),1,0, 0, i);
    }
    VulkanRBE::EndRenderLayer();
}
