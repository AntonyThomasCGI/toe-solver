#pragma once

#include <vulkan/vulkan.h>

#include <vector>

#include "devices/window.hpp"


class Instance
{
public:
    Instance(Window &window);
    ~Instance();

    static const std::vector<const char*> validationLayers;
    static std::vector<const char*> instanceExtensions;

    const VkInstance &getInstance() const { return instance; }

    bool enableValidationLayers;

private:
    VkInstance instance;

    bool checkValidationLayerSupport();
    void createInstance(Window &window);
};
