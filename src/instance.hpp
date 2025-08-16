#pragma once
#include <vulkan/vulkan.hpp>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace owo {

struct VulkanInstance{
    vk::UniqueInstance instance;
    vk::UniqueHandle<vk::DebugUtilsMessengerEXT, vk::detail::DispatchLoaderDynamic> debugMessenger;
};

struct VulkanDevice{
    vk::PhysicalDevice physicalDevice;
    vk::UniqueDevice logicalDevice;
};

struct VulkanSwapChain {
    vk::UniqueSwapchainKHR swapchain;
};

struct VulkanContext {
    VulkanInstance vkInstance;
    VulkanDevice vkDevices;
    VulkanSwapChain vkSwapchain;
};

VKAPI_ATTR vk::Bool32 VKAPI_CALL debugCallback(vk::DebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    vk::DebugUtilsMessageTypeFlagsEXT messageType,
    const vk::DebugUtilsMessengerCallbackDataEXT* pCallbackData,
    void* pUserData);

VulkanInstance createVulkanInstance(const char* NAME);    //Creates Vulkan instance and debug messenger

vk::UniqueSurfaceKHR getUniqueSurface(vk::UniqueInstance& instance, GLFWwindow* window);

}