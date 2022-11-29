#pragma once

#include <iostream>
#include <memory>
#include <unordered_map>
#include "components/camera.h"
#include "entity.h"
#include "VkInit.h"


class RenderPipeline {
public:
    virtual void Prepare(Camera* camera = nullptr)=0;
    virtual void Update(Camera* camera)=0;
    std::vector<Entity*> entities;
};


class PipelineManager {
public:
    template<typename T>
    static void AddPipeline() {
        if (pipelines.find(typeid(T).name()) == pipelines.end()) {
            T pipeline;
            pipelines[typeid(T).name()] = std::make_shared<T>(std::move(pipeline));
        }
    }
    template<typename T>
    static T* GetPipeline() {
        if (pipelines.find(typeid(T).name()) != pipelines.end()) {
            return static_cast<T*>(pipelines.find(typeid(T).name())->second.get());
        }
        return nullptr;
    }
    template<typename T>
    static void AddEntityToList(Entity* entity) {
        if (pipelines.find(typeid(T).name()) != pipelines.end()) {
            pipelines.find(typeid(T).name())->second->entities.push_back(entity);
        }
    }
    static void UpdatePipeline(Camera* camera = nullptr) {
        for (auto pipeline : pipelines){
            pipeline.second->Update(camera);
        }
    }
    template<typename T>
    static void RemovePipeline() {
        if (pipelines.find(typeid(T).name()) != pipelines.end()) {

        }
    }
private:
    static inline std::unordered_map<const char*, std::shared_ptr<RenderPipeline>> pipelines;
};
