#pragma once

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
#include <rocket.hpp>

#include <utility>

class Window
{
public:
    Window(unsigned int width, unsigned int height, std::string title);
    ~Window();

    void update();

    std::pair<uint32_t, const char**> getInstanceExtensions();

    VkResult createSurface(VkInstance &instance, VkSurfaceKHR *surface);

    void getFramebufferSize(int &width, int &height);

    rocket::signal<void()> shouldClose;
    rocket::signal<void(int, int)> framebufferResized;
    rocket::signal<void(int, int, int, int)> keyPressed;

private:
    GLFWwindow *window;
    std::string title;

    void createWindow(unsigned int width, unsigned int height);

    friend void framebufferResizeCallback(GLFWwindow* window, int width, int height);
    friend void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
};
