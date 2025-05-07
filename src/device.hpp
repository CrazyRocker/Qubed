#pragma once
#include <vulkan/vulkan.hpp>
#include <optional>

namespace owo {

struct SuitabilityInfo{
    bool suitable = false;
    const char* const reason = "Unknown error :(";
};

SuitabilityInfo isPhysicalDeviceSuitable(vk::PhysicalDevice device, vk::UniqueSurfaceKHR& surface);

vk::PhysicalDevice getPhysicalDevice(vk::UniqueHandle<vk::Instance, vk::detail::DispatchLoaderStatic>& instance, vk::UniqueSurfaceKHR& surface);

std::pair<std::optional<uint32_t>, std::optional<uint32_t>> getGraphicsQueueAndPresentQueue(const vk::PhysicalDevice& physicalDevice, const vk::UniqueSurfaceKHR& surface);

}