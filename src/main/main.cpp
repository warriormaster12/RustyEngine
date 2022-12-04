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
    Entity my_entity;
    Entity my_entity2;
    Entity my_entity3;
    my_entity.AddComponent<Camera>();
    my_entity.AddComponent<Mesh>();
    my_entity.AddComponent<Transform>();

    my_entity.GetComponent<Transform>()->position = {1.0f, 0.3f, 0.0f};
    my_entity.GetComponent<Transform>()->scale= {1.1f,1.1f,1.1f};

    my_entity2.AddComponent<Mesh>();
    my_entity2.AddComponent<Transform>();
    my_entity2.GetComponent<Transform>()->position = {-1.0f,-0.3f, 0.0f};
    my_entity2.GetComponent<Transform>()->rotation = {45.0f, 0.0f, 0.0f};


    ModelLoader::LoadFile("samples/models/suzanne.gltf", my_entity3);


    VulkanDevice::Init();
    Window::Init();
    VulkanDevice::CreateDevice();
    VulkanDevice::CreateSwapchain(1280, 720);

    my_entity.Start();
    my_entity2.Start();
    my_entity3.Start();

    PipelineManager::AddPipeline<GeometryPipeline>();
    PipelineManager::AddEntityToList<GeometryPipeline>(&my_entity);
    PipelineManager::AddEntityToList<GeometryPipeline>(&my_entity2);
    PipelineManager::AddEntityToList<GeometryPipeline>(&my_entity3);
    PipelineManager::GetPipeline<GeometryPipeline>()->Prepare(my_entity.GetComponent<Camera>());

    while ( Window::Running() ) {
        VulkanRBE::PrepareFrame();
        PipelineManager::UpdatePipeline(my_entity.GetComponent<Camera>());
        VulkanRBE::SubmitFrame();
        my_entity.Update(VulkanRBE::frameCount);
        my_entity2.Update(VulkanRBE::frameCount);
        my_entity3.Update(VulkanRBE::frameCount);
    }
    VulkanDevice::Shutdown();
    Window::Shutdown();
    return 0;
}
