#pragma once
#include <vulkan/vulkan.hpp>

namespace owo {

    vk::PhysicalDevice getPhysicalDevice(vk::UniqueHandle<vk::Instance, vk::detail::DispatchLoaderStatic>& instance);

}