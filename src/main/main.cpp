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
    std::vector<Entity> sponza;
    Entity my_entity;
    my_entity.name = "dragon";
    Entity my_entity2;
    my_entity2.name = "suzanne";

    my_entity.AddComponent<Camera>();
    //my_entity.AddComponent<Mesh>();
    my_entity.AddComponent<Transform>();

    my_entity.GetComponent<Transform>()->position = {1.0f, 0.3f, 0.0f};
    my_entity.GetComponent<Transform>()->scale= {1.1f,1.1f,1.1f};

    my_entity2.AddComponent<Mesh>();
    my_entity2.AddComponent<Transform>();
    my_entity2.GetComponent<Transform>()->position = {-1.0f,-0.3f, 0.0f};
    my_entity2.GetComponent<Transform>()->rotation = {0.0f, 5.0f, 45.0f};


    ModelLoader::LoadFile("samples/models/chinesedragon.gltf", chinesedragon);
    ModelLoader::LoadFile("samples/models/sponza.gltf", sponza);


    VulkanDevice::Init();
    Window::Init();
    VulkanDevice::CreateDevice();
    VulkanDevice::CreateSwapchain(1280, 720);
    my_entity.Start();
    PipelineManager::AddPipeline<GeometryPipeline>();
    for(auto& entity : chinesedragon) {
        entity.Start();
        PipelineManager::AddEntityToList<GeometryPipeline>(&entity);
    }
    for(auto& entity : sponza) {
        entity.Start();
        PipelineManager::AddEntityToList<GeometryPipeline>(&entity);
    }
    PipelineManager::GetPipeline<GeometryPipeline>()->Prepare(my_entity.GetComponent<Camera>());

    while ( Window::Running() ) {
        VulkanRBE::PrepareFrame();
        PipelineManager::UpdatePipeline(my_entity.GetComponent<Camera>());
        VulkanRBE::SubmitFrame();
        my_entity.Update(VulkanRBE::frameCount);
        for(auto& entity : chinesedragon) {
            entity.Update(VulkanRBE::frameCount);
        }
        for(auto& entity : sponza) {
            entity.Update(VulkanRBE::frameCount);
        }
    }
    VulkanDevice::Shutdown();
    Window::Shutdown();
    return 0;
}
