#pragma once

#include <optional>

#include "vulkan/vulkan.h"

#include "devices/window.hpp"
#include "instance.hpp"


struct QueueFamilyIndices {
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;

    bool isComplete() {
        return graphicsFamily.has_value() && presentFamily.has_value();
    }
};

struct SwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};



class Surface {

public:
    Surface(Instance &instance, Window &window);
    ~Surface();

    const VkSurfaceKHR &getSurface() const { return surface; }

    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
    SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);

private:
    Instance &instance;
    VkSurfaceKHR surface;
};
