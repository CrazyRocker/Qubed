#pragma once
#include <vulkan/vulkan.hpp>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace owo {

VKAPI_ATTR vk::Bool32 VKAPI_CALL debugCallback(vk::DebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    vk::DebugUtilsMessageTypeFlagsEXT messageType,
    const vk::DebugUtilsMessengerCallbackDataEXT* pCallbackData,
    void* pUserData);

vk::UniqueHandle<vk::Instance, vk::detail::DispatchLoaderStatic> createVulkanInstance(const char* NAME);

vk::UniqueSurfaceKHR getUniqueSurface(vk::UniqueInstance& instance, GLFWwindow* window);

}