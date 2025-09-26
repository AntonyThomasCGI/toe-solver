#pragma once

#include <memory>
#include <string>
#include <vector>

#include <vulkan/vulkan.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "command_pool.hpp"
#include "logical_device.hpp"
#include "pipeline.hpp"
#include "swap_chain.hpp"
#include "texture.hpp"


class Shader
{
public:
    Shader(Device &device, std::string vertShader, std::string fragShader);
    //Shader(VkDevice &device, CommandPool &commandPool, SwapChain &swapChain, TextureImage &textureImage, std::string vertShader, std::string fragShader);
    ~Shader();

    void bind(VkCommandBuffer &commandBuffer, uint32_t currentFrame);

    //std::unique_ptr<GraphicsPipeline> graphicsPipeline;
    VkShaderModule vertShaderModule;
    VkShaderModule fragShaderModule;

    const std::string getVertShaderPath() { return vertShaderPath; }
    const std::string getFragShaderPath() { return fragShaderPath; }

private:
    Device &device;

    std::string vertShaderPath;
    std::string fragShaderPath;

    //VkDescriptorSetLayout descriptorSetLayout;

    VkShaderModule createShaderModule(const std::vector<char>& code);

    //std::vector<VkBuffer> uniformBuffers;
    //std::vector<VkDeviceMemory> uniformBuffersMemory;
    //std::vector<void*> uniformBuffersMapped;

    //VkDescriptorPool descriptorPool;
    //std::vector<VkDescriptorSet> descriptorSets;

    //void createDescriptorSets(TextureImage &textureImage);
    //void createDescriptorPool();
    //void createUniformBuffers(CommandPool &commandPool);
    //void createDescriptorSetLayout();
};
