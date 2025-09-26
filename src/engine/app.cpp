
#include <chrono>

#include "app.hpp"

AppBase::AppBase(unsigned int width, unsigned int height, int argc, char **argv)
{
    window = new Window(width, height);
    engine = new Engine(*window);
    connections += {
        window->shouldClose.connect<&AppBase::exitApp>(this),
        window->keyPressed.connect<&AppBase::_processInput>(this)
    };
}


AppBase::~AppBase()
{
    delete engine;
    delete window;
}


void AppBase::exitApp()
{
    running = false;
}


void AppBase::_processInput(int key, int scancode, int action, int mode)
{
    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)
            keys[key] = true;
        else if (action == GLFW_RELEASE)
        {
            keys[key] = false;
            keysProcessed[key] = false;
        }
    }
}


void AppBase::mainLoop()
{
    float deltaTime = 0.0f;
    auto lastFrame = std::chrono::high_resolution_clock::now();

    while (running) {
        auto currentFrame = std::chrono::high_resolution_clock::now();
        std::chrono::duration<float> diff = currentFrame - lastFrame;
        deltaTime = diff.count();
        lastFrame = currentFrame;

        processInput(deltaTime);
        update(deltaTime);
        window->update();
        engine->update();
    }
}
