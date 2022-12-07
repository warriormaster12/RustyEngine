#include <iostream>
#include "render_pipelines.h"
#include "geometry_pipeline.h"
#include "window.h"
#include <vector>
#include "components/camera.h"
#include "components/mesh.h"
#include "components/transform.h"

#include "logger.h"
#include "model_loader.h"


int main(int argc, char* argv[]) {
    Logger::Init();

    std::vector<Entity> chinesedragon;
    std::vector<Entity> suzanne;
    Entity camera;
    camera.name = "camera";

    camera.AddComponent<Camera>();
    camera.AddComponent<Transform>();
    camera.GetComponent<Transform>()->SetPosition(glm::vec3(0.0f, 1.0f, 0.0f));



    chinesedragon = ModelLoader::LoadFile("samples/models/chinesedragon.gltf");
    //chinesedragon[0].GetComponent<Transform>()->SetRotation(glm::vec3(0.0f, 0.0f, 0.0f));
    suzanne = ModelLoader::LoadFile("samples/models/sponza.gltf");


    VulkanDevice::Init();
    Window::Init();
    VulkanDevice::CreateDevice();
    VulkanDevice::CreateSwapchain(1280, 720);
    PipelineManager::AddPipeline<GeometryPipeline>();
    for(auto& entity : chinesedragon) {
        entity.Start();
        PipelineManager::AddEntityToList<GeometryPipeline>(&entity);
    }
    for(auto& entity : suzanne) {
        entity.Start();
        PipelineManager::AddEntityToList<GeometryPipeline>(&entity);
    }
    camera.Start();
    PipelineManager::AddEntityToList<GeometryPipeline>(&camera);
    PipelineManager::GetPipeline<GeometryPipeline>()->Prepare();

    while ( Window::Running() ) {
        camera.Update(VulkanRBE::frameCount);
        for(auto& entity : chinesedragon) {
            entity.Update(VulkanRBE::frameCount);
        }
        for(auto& entity : suzanne) {
            entity.Update(VulkanRBE::frameCount);
        }
        VulkanRBE::PrepareFrame();
        PipelineManager::UpdatePipeline();
        VulkanRBE::SubmitFrame();
    }
    VulkanDevice::Shutdown();
    Window::Shutdown();
    return 0;
}
