#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <memory>

#include "component.h"

class Entity {
public:
    std::string name;

    void Start() {
        for(auto& component : component_list) {
            component->on_ready();
        }
    }

    void Update(const double& deltaT) {
         for(auto& component : component_list) {
            component->update(deltaT);
        }
    }
    template<typename T>
    void AddComponent() {
        if ( component_list.size() > 0) {
            for(int i=0; i < component_list.size(); i++) {
                if (component_list[i]->type_name == typeid(T).name()){
                    break;
                }
                else {
                    std::unique_ptr<T> component = std::make_unique<T>();
                    component->type_name = typeid(T).name();
                    if (component->entity == nullptr) {
                        component->entity = this;
                    }
                    component_list.push_back(std::move(component));
                    break;
                }
            }
        }
        else {
            std::unique_ptr<T> component = std::make_unique<T>();
            component->type_name = typeid(T).name();
            if (component->entity == nullptr) {
                component->entity = this;
            }
            component_list.push_back(std::move(component));
        }
    }
    template<typename T>
    T* GetComponent() {
        for(int i=0; i < component_list.size(); i++) {
            if (component_list[i]->type_name == typeid(T).name()){
                return static_cast<T*>(component_list[i].get());
            }
        }
        return nullptr;
    }
    template<typename T>
    void RemoveComponent() {
        for(int i=0; i < component_list.size(); i++) {
            if (component_list[i]->type_name == typeid(T).name()){
                component_list.erase(component_list.begin() + i);
                component_list.shrink_to_fit();
            }
        }
    }
private:
    std::vector<std::shared_ptr<Component>> component_list = {};
};
