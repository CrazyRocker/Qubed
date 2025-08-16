#pragma once
#include <vulkan/vulkan.hpp>
#include <optional>
#include "instance.hpp"

namespace owo {

struct SuitabilityInfo{
    bool suitable = false;
    std::string reason = "Unknown error :(";
};

struct SwapChainSupportDetails {
    vk::SurfaceCapabilitiesKHR capabilities;
    std::vector<vk::SurfaceFormatKHR> formats;
    std::vector<vk::PresentModeKHR> presentModes;
};

constexpr const char* requiredExtensions[] = {"VK_KHR_swapchain"};
constexpr uint8_t requiredExtensionsCount = sizeof(requiredExtensions)/sizeof(requiredExtensions[0]);

SuitabilityInfo isPhysicalDeviceSuitable(vk::PhysicalDevice device, vk::UniqueSurfaceKHR& surface);

vk::PhysicalDevice getPhysicalDevice(vk::UniqueInstance& instance, vk::UniqueSurfaceKHR& surface);

std::pair<std::optional<uint32_t>, std::optional<uint32_t>> getGraphicsQueueAndPresentQueue_optional(const vk::PhysicalDevice& physicalDevice, const vk::UniqueSurfaceKHR& surface);

std::pair<uint32_t, uint32_t> getGraphicsQueueAndPresentQueue(const vk::PhysicalDevice& physicalDevice, const vk::UniqueSurfaceKHR& surface);

vk::UniqueDevice getUniqueDevice(vk::PhysicalDevice& physicalDevice, vk::UniqueSurfaceKHR& surface);

VulkanDevice createVulkanDevice(vk::UniqueInstance& instance, vk::UniqueSurfaceKHR& surface);

vk::Extent2D chooseSwapExtent(const vk::SurfaceCapabilitiesKHR surfaceCapabilities, GLFWwindow* window);

SwapChainSupportDetails querySwapChainSupport(vk::PhysicalDevice physicalDevice, vk::SurfaceKHR& surface, vk::detail::DispatchLoaderStatic dispatch);

VulkanSwapChain createSwapChain(VulkanContext& vulkanContext, GLFWwindow* window, vk::UniqueSurfaceKHR& surface);

}