#pragma once

#include <iostream>


class Window{
public:
    static void Init();
    static bool Running();
    static void Shutdown();
    static void ResizeFramebuffer(const bool& value) {framebuffer_resized = value;}
    static int GetKEvents(int key);
private:
    static inline void* p_window = nullptr;
    static inline bool framebuffer_resized = false;
};
