#pragma once
#include <vulkan/vulkan.hpp>
#include <optional>

namespace owo {

struct SuitabilityInfo{
    bool suitable = false;
    std::string reason = "Unknown error :(";
};

constexpr const char* requiredExtensions[] = {"VK_KHR_swapchain"};
constexpr uint8_t requiredExtensionsCount = sizeof(requiredExtensions)/sizeof(requiredExtensions[0]);

SuitabilityInfo isPhysicalDeviceSuitable(vk::PhysicalDevice device, vk::UniqueSurfaceKHR& surface);

std::pair<std::optional<uint32_t>, std::optional<uint32_t>> getGraphicsQueueAndPresentQueue_optional(const vk::PhysicalDevice& physicalDevice, const vk::UniqueSurfaceKHR& surface);

vk::PhysicalDevice getPhysicalDevice(vk::UniqueInstance& instance, vk::UniqueSurfaceKHR& surface);

bool isSwapChainViable(const vk::UniqueSurfaceKHR& surface, const vk::PhysicalDevice& device);

bool doesDeviceSupportRequiredExtensions(vk::PhysicalDevice physicalDevice);

vk::UniqueDevice getUniqueDevice(vk::UniqueInstance& instance, vk::UniqueSurfaceKHR& surface);

}