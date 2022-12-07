#include <iostream>
#include "render_pipelines.h"
#include "geometry_pipeline.h"
#include "window.h"
#include <vector>
#include "components/camera.h"
#include "components/mesh.h"
#include "components/transform.h"
#include "components/script.h"

#include "logger.h"
#include "model_loader.h"



double deltaTime = 0.0f;
std::chrono::time_point<std::chrono::high_resolution_clock> tStart;
std::chrono::time_point<std::chrono::high_resolution_clock> tEnd;

int main(int argc, char* argv[]) {
    Logger::Init();

    std::vector<Entity> chinesedragon;
    std::vector<Entity> suzanne;
    Entity camera;
    camera.name = "camera";

    camera.AddComponent<Camera>();
    camera.AddComponent<Transform>();
    camera.AddComponent<Script>();
    camera.GetComponent<Transform>()->SetPosition(glm::vec3(0.0f, 5.0f, 0.0f));
    camera.GetComponent<Transform>()->SetRotation(glm::vec3(90.0f, 0.0f, 0.0f));



    chinesedragon = ModelLoader::LoadFile("samples/models/chinesedragon.gltf");
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
        tStart = std::chrono::high_resolution_clock::now();
        camera.Update(deltaTime);
        for(auto& entity : chinesedragon) {
            entity.Update(deltaTime);
        }
        for(auto& entity : suzanne) {
            entity.Update(deltaTime);
        }
        VulkanRBE::PrepareFrame();
        PipelineManager::UpdatePipeline();
        VulkanRBE::SubmitFrame();

        tEnd = std::chrono::high_resolution_clock::now();
        auto tDiff = std::chrono::duration<double, std::milli>(tEnd - tStart).count();
        deltaTime = tDiff / 1000.0f;
    }
    VulkanDevice::Shutdown();
    Window::Shutdown();
    return 0;
}
