#pragma once
#include <vulkan/vulkan.hpp>

namespace owo {

VKAPI_ATTR vk::Bool32 VKAPI_CALL debugCallback(vk::DebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    vk::DebugUtilsMessageTypeFlagsEXT messageType,
    const vk::DebugUtilsMessengerCallbackDataEXT* pCallbackData,
    void* pUserData);

vk::UniqueHandle<vk::Instance, vk::detail::DispatchLoaderStatic> createVulkanInstance(const char* NAME);

}