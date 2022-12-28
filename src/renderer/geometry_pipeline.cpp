#include "geometry_pipeline.h"
#include "components/mesh.h"
#include "components/transform.h"
#include "components/camera.h"
#include "frustum.h"



Pipeline testPipeline;


void GeometryPipeline::Prepare() {
    testPipeline.depthEnabled = true;
    PipelineBuilder::BuildGraphicsPipeline({"shaders/simple_triangle.vert.spv","shaders/simple_triangle.frag.spv"}, {VK_SHADER_STAGE_VERTEX_BIT, VK_SHADER_STAGE_FRAGMENT_BIT}, testPipeline);
    for(auto entity: entities){
        if(entity->GetComponent<Camera>() != nullptr) {
            testPipeline.descriptorSets[0].GenerateDescriptorBuffers(entity->GetComponent<Camera>()->cameraBuffer.descBuffInfo);
        }

    }
    for(auto entity: entities){
        if(entity->GetComponent<Mesh>()!= nullptr){
            testPipeline.descriptorSets[1].GenerateDescriptorBuffers(entity->GetComponent<Mesh>()->modelBuffer.descBuffInfo);
            break;
        }
    }
}

void GeometryPipeline::Update() {
    VulkanRBE::currentPipeline = &testPipeline;
    VulkanRBE::clearColor[0] = 0.1f;
    VulkanRBE::clearColor[1] = 0.1f;
    VulkanRBE::clearColor[2] = 0.1f;
    VulkanRBE::clearColor[3] = 1.0f;
    VulkanRBE::BeginRenderLayer();
    std::vector<glm::mat4> modelMatricies;
    Camera* camera = nullptr;
    Frustum cameraFrustum;
    for(auto entity : entities) {
        if(entity->GetComponent<Camera>() != nullptr){
            camera = entity->GetComponent<Camera>();
            cameraFrustum = cameraFrustum.createFrustumFromCamera(*entity);
            break;
        }
    }
    for(int i=0; i < entities.size(); i++) {
        auto entity = entities[i];
        if(entity->GetComponent<Mesh>() != nullptr && entity->GetComponent<Transform>() != nullptr) {
            AABB aabb(AABB::generateAABB(*entity));
            if(aabb.isOnFrustum(cameraFrustum, *entity)) {
                modelMatricies.push_back(entity->GetComponent<Mesh>()->modelMatrix);
                entities[0]->GetComponent<Mesh>()->modelBuffer.UploadBufferData(modelMatricies.data());
                if(camera != nullptr){
                    camera->renderMatrix = camera->GetProjectionMatrix() * camera->GetViewMatrix();
                }
                if(entity->GetComponent<Mesh>()->vertices.size() > 0) {
                    VulkanRBE::BindVertexBuffer(0, 1, entity->GetComponent<Mesh>()->vertexBuffer);
                    if(entity->GetComponent<Mesh>()->indicies.size() > 0) {
                        VulkanRBE::BindIndexBuffer(entity->GetComponent<Mesh>()->indexBuffer);
                        VulkanRBE::DrawIndexed(entity->GetComponent<Mesh>()->indicies.size(),1,0, 0, i);
                    }else {
                        VulkanRBE::Draw(entity->GetComponent<Mesh>()->vertices.size(),1,0,i);
                    }
                }
            }
        }
    }
    VulkanRBE::EndRenderLayer();
}
