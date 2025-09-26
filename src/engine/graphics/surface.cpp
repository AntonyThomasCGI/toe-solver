
#include <stdexcept>
#include <iostream>

#include <vulkan/vk_enum_string_helper.h>

#include "surface.hpp"


Surface::Surface(Instance &instance, Window &window) : instance(instance)
{
    VkInstance inst = instance.getInstance();
    if (VkResult result = window.createSurface(inst, &surface); result != VK_SUCCESS) {
        std::cerr << string_VkResult(result) << std::endl;
        throw std::runtime_error("failed to create window surface!");
    }
}


Surface::~Surface()
{
    VkInstance inst = instance.getInstance();
    vkDestroySurfaceKHR(inst, surface, nullptr);
}


QueueFamilyIndices Surface::findQueueFamilies(VkPhysicalDevice device) {
    QueueFamilyIndices indicies;

    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

    int i = 0;
    for (const auto& queueFamily : queueFamilies) {
        // Does device support graphics rendering.
        if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            indicies.graphicsFamily = i;
        }
        // Does the device support our window surface.
        VkBool32 presentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);
        if (presentSupport) {
            indicies.presentFamily = i;
        }

        if (indicies.isComplete()) {
            break;
        }
        i++;
    }

    return indicies;
}


SwapChainSupportDetails Surface::querySwapChainSupport(VkPhysicalDevice device) {
    SwapChainSupportDetails details;

    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);

    uint32_t formatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);
    if (formatCount != 0) {
        details.formats.resize(formatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.formats.data());
    }

    uint32_t presentModeCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);
    if (presentModeCount != 0) {
        details.presentModes.resize(presentModeCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, details.presentModes.data());
    }

    return details;
}

