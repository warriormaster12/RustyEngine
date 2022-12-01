#include "window.h"
#define GLFW_INCLUDE_VULKAN
#define VK_NO_PROTOTYPES
#include "GLFW/glfw3.h"
#include "VkInit.h"


void FramebufferResizeCallback(GLFWwindow* window, int width, int height) {
    Window::ResizeFramebuffer(true);
}

void Window::Init() {
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    GLFWwindow* glfw_p_window=nullptr;
    glfw_p_window = glfwCreateWindow(1280,720, "Rusty Engine", nullptr, nullptr);

    p_window = static_cast<void*>(glfw_p_window);

    glfwSetFramebufferSizeCallback(static_cast<GLFWwindow*>(p_window), FramebufferResizeCallback);

    if ( VulkanDevice::GetGlobalInstance() != VK_NULL_HANDLE) {
       glfwCreateWindowSurface(VulkanDevice::GetGlobalInstance(), static_cast<GLFWwindow*>(p_window), nullptr, &VulkanDevice::GetGlobalSurface());
    }
    ENGINE_INFO("Window created");
}

bool Window::Running() {
    if (!glfwWindowShouldClose(static_cast<GLFWwindow*>(p_window))) {
        glfwPollEvents();
        return true;
    }
    return false;
}

void Window::Shutdown() {
    glfwDestroyWindow(static_cast<GLFWwindow*>(p_window));
    glfwTerminate();
}
