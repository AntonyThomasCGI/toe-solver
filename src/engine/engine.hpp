#pragma once

#include "devices/window.hpp"
#include "graphics/graphics.hpp"


class Engine
{
public:
    Engine(Window &window);
    ~Engine();

    void update();

    std::unique_ptr<VulkanGraphics> graphics;

private:
    Window &window;

    rocket::scoped_connection_container connections;
};
