#include "device.hpp"
#include "GLFW/glfw3.h"
#include "utils.hpp"
#include <climits>
#include <algorithm>
#include <iostream>

namespace owo {

SuitabilityInfo isPhysicalDeviceSuitable(vk::PhysicalDevice physicalDevice, vk::UniqueSurfaceKHR& surface){
    auto [graphicsQueueFamilyIndex, presentQueueFamilyIndex] = getGraphicsQueueAndPresentQueue_optional(physicalDevice, surface);
    //Checking for Graphics or Queue Families
    if (!graphicsQueueFamilyIndex.has_value()) {
        if(!presentQueueFamilyIndex.has_value()) {
            return {false, "No graphics or present queue family found!"};
        } else {
            return {false, "No graphics queue family found!"};
        }
    } else {
        if(!presentQueueFamilyIndex.has_value()) {
            return {false, "No present Queue family found!"};
        }
    }

    //Checking if there is at least one way to present data and at least one place to present data to (I think, lol)
    std::vector<vk::SurfaceFormatKHR> surfaceFormats = physicalDevice.getSurfaceFormatsKHR(surface.get());
    std::vector<vk::PresentModeKHR> presentModes = physicalDevice.getSurfacePresentModesKHR(surface.get());
    if(surfaceFormats.empty())
        return {false, "Could not find a single surface format to use on the selected GPU"};
    if(presentModes.empty())
        return {false, "Could not find a single present mode to use on the selected GPU"};

    //Checking if all the required extensions are available 
    std::vector<vk::ExtensionProperties> deviceExtensions = physicalDevice.enumerateDeviceExtensionProperties();

    for(int i=0; i < requiredExtensionsCount; i++) {
        bool found = false;
        const char* requiredExtension = requiredExtensions[i];

        for(int j=0; j < deviceExtensions.size();j++)
            if(strcmp(requiredExtension, deviceExtensions[j].extensionName) == 0)
                found = true;

        if(found==false)
            return {false, std::string("Could not find the extension \"") + requiredExtensions[i] + "\" in the list of extensions supported by the selected GPU."};
    }

    return {true, "Supported! :D"};
}


vk::PhysicalDevice getPhysicalDevice(vk::UniqueInstance& instance, vk::UniqueSurfaceKHR& surface) {
    std::vector<vk::PhysicalDevice> physicalDevices = instance->enumeratePhysicalDevices();

    if(physicalDevices.size() == 0){
        throw std::runtime_error("Bro... there are no GPUs 💀\n");
    }
    if(physicalDevices.size() > INT_MAX){
        throw std::runtime_error("Bro... chill 💀 Too many GPUs \n");
    }

    std::cout<<"\nList of available devices:\n";
    for(int i=0; i<physicalDevices.size(); i++){
        auto info = isPhysicalDeviceSuitable(physicalDevices[i], surface);
        if(info.suitable)
            std::cout<<i<<" - "<<physicalDevices[i].getProperties().deviceName<<" (Suitable)\n";
        else
            std::cout<<i<<" - "<<physicalDevices[i].getProperties().deviceName<<" (Unsuitable: "<<info.reason<<")\n";
    }

    while(true){
        uint8_t deviceIndex = owo::input_get_uint8_t("\nPlease enter the device number for the device you want to use (Default: 0): ", 0);
        if (deviceIndex > static_cast<int>(physicalDevices.size() - 1)) {
            std::cout << "The number you have entered is too large! Try again!\n";
        } else if (!isPhysicalDeviceSuitable(physicalDevices[deviceIndex], surface).suitable){
            std::cout << "Unsuitable device selected! Try again!";
        } else{
            return physicalDevices[deviceIndex];
        }
    }
}


std::pair<std::optional<uint32_t>, std::optional<uint32_t>> getGraphicsQueueAndPresentQueue_optional(const vk::PhysicalDevice& physicalDevice, const vk::UniqueSurfaceKHR& surface) {
    std::vector<vk::QueueFamilyProperties> queueFamilyProperties = physicalDevice.getQueueFamilyProperties();
    
    std::optional<uint32_t> graphicsQueueFamilyIndex;
    std::optional<uint32_t> presentQueueFamilyIndex;

    for(int i=0; i<queueFamilyProperties.size(); i++){
        if(queueFamilyProperties[i].queueFlags & vk::QueueFlagBits::eGraphics)
            graphicsQueueFamilyIndex = i;
        if(physicalDevice.getSurfaceSupportKHR(i, surface.get()))
            presentQueueFamilyIndex = i;
        if(graphicsQueueFamilyIndex.has_value() && presentQueueFamilyIndex.has_value())
            break;
    }

    return {graphicsQueueFamilyIndex, presentQueueFamilyIndex};
}


std::pair<uint32_t, uint32_t> getGraphicsQueueAndPresentQueue(const vk::PhysicalDevice& physicalDevice, const vk::UniqueSurfaceKHR& surface) {
    std::vector<vk::QueueFamilyProperties> queueFamilyProperties = physicalDevice.getQueueFamilyProperties();
    
    std::optional<uint32_t> graphicsQueueFamilyIndex;
    std::optional<uint32_t> presentQueueFamilyIndex;

    for(int i=0; i<queueFamilyProperties.size(); i++){
        if(queueFamilyProperties[i].queueFlags & vk::QueueFlagBits::eGraphics)
            graphicsQueueFamilyIndex = i;
        if(physicalDevice.getSurfaceSupportKHR(i, surface.get()))
            presentQueueFamilyIndex = i;
        if(graphicsQueueFamilyIndex.has_value() && presentQueueFamilyIndex.has_value())
            break;
    }

    return {graphicsQueueFamilyIndex.value(), presentQueueFamilyIndex.value()};
}


vk::UniqueDevice getUniqueDevice(vk::PhysicalDevice& physicalDevice, vk::UniqueSurfaceKHR& surface) {
    auto [graphicsQueueFamilyIndex, presentQueueFamilyIndex] = owo::getGraphicsQueueAndPresentQueue(physicalDevice, surface);
    
    float queuePriority = 1.0;
    vk::DeviceQueueCreateInfo queueCreateInfos[] {
        vk::DeviceQueueCreateInfo{vk::DeviceQueueCreateFlags(), static_cast<uint32_t>(graphicsQueueFamilyIndex), 1, &queuePriority},
        vk::DeviceQueueCreateInfo{vk::DeviceQueueCreateFlags(), static_cast<uint32_t>(presentQueueFamilyIndex),  1, &queuePriority}
    };
    uint8_t queueCount = (graphicsQueueFamilyIndex==presentQueueFamilyIndex)?1:2;

    return physicalDevice.createDeviceUnique(vk::DeviceCreateInfo(
        vk::DeviceCreateFlags(), queueCount, queueCreateInfos,
        0u, nullptr,
        static_cast<uint32_t>(requiredExtensionsCount), requiredExtensions));
}


VulkanDevice createVulkanDevice(vk::UniqueInstance& instance, vk::UniqueSurfaceKHR& surface) {
    vk::PhysicalDevice physicalDevice = owo::getPhysicalDevice(instance, surface);
    vk::UniqueDevice logicalDevice = getUniqueDevice(physicalDevice, surface);
    return {physicalDevice, std::move(logicalDevice)};
}


vk::SurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<vk::SurfaceFormatKHR>& availableSurfaceFormats) {
    for(auto surfaceFormat : availableSurfaceFormats) {
        if(surfaceFormat.format == vk::Format::eB8G8R8A8Srgb && surfaceFormat.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear) //Selecting for a standard image format and colourspace. We could do better by checking for, say, 10 bit colour or other, better image formats but that adds unwanted complexity at this stage (26/06/2025) (dd/mm/yyyy)
            return surfaceFormat;
    }
    return availableSurfaceFormats[0];  //In this case, pray to god and hope for the best xD
}


vk::PresentModeKHR chooseSwapPresentMode(const std::vector<vk::PresentModeKHR> availablePresentModes) {
    for(const auto& presentMode : availablePresentModes) {
        if(presentMode == vk::PresentModeKHR::eMailbox) //Preferred presentmode
            return presentMode;
    }
    return vk::PresentModeKHR::eFifo;
}


vk::Extent2D chooseSwapExtent(const vk::SurfaceCapabilitiesKHR surfaceCapabilities, GLFWwindow* window) {
    if(surfaceCapabilities.currentExtent.width != UINT32_MAX) {     //The surfaceCapabilities.extent being equalt to UINT32_MAX is the window manager signaling that the screen resolution is allowed to, and may be different from the framebuffer resolution. 
        return surfaceCapabilities.currentExtent;                   //If it is not equal to UINT32_MAX, we can simply treat both forms of resolution as equal
    } else {
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        
        vk::Extent2D actualExtent{                  //Struct which will containt the actual framebuffer resolution
            static_cast<uint32_t>(width),
            static_cast<uint32_t>(height)
        };
        
        actualExtent.width  =  std::clamp(actualExtent.width,  surfaceCapabilities.minImageExtent.width,  surfaceCapabilities.maxImageExtent.width);
        actualExtent.height =  std::clamp(actualExtent.height, surfaceCapabilities.minImageExtent.height, surfaceCapabilities.maxImageExtent.height);

        return actualExtent;
    }
}


SwapChainSupportDetails querySwapChainSupport(vk::PhysicalDevice physicalDevice, vk::SurfaceKHR& surface, vk::detail::DispatchLoaderStatic dispatch) {
    SwapChainSupportDetails details;

    details.capabilities = physicalDevice.getSurfaceCapabilitiesKHR(surface, dispatch);

    details.formats = physicalDevice.getSurfaceFormatsKHR(surface);

    details.presentModes = physicalDevice.getSurfacePresentModesKHR(surface);

    return details;
}


VulkanSwapChain createSwapChain(VulkanContext& vulkanContext, GLFWwindow* window, vk::UniqueSurfaceKHR& surface) {
    SwapChainSupportDetails swapChainDetails = querySwapChainSupport(vulkanContext.vkDevices.physicalDevice, surface.get(), vulkanContext.vkInstance.instance.getDispatch());

    vk::SurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainDetails.formats);
    vk::SurfacePresentModeEXT presentMode = chooseSwapPresentMode(swapChainDetails.presentModes);
    vk::Extent2D extent = chooseSwapExtent(swapChainDetails.capabilities, window);

    uint8_t imageCount = swapChainDetails.capabilities.minImageCount + 1;                                           //We try to request for one more image in the swapchain than the minimum
    if(swapChainDetails.capabilities.maxImageCount > 0 && imageCount > swapChainDetails.capabilities.maxImageCount) //So as to not have to wait to start writing the images so as to not
        imageCount = swapChainDetails.capabilities.maxImageCount;                                                   //have to wait for the diver to start writing the images. However, we
                                                                                                                    //still allow a lower number for better compatibility
    vk::SwapchainCreateInfoKHR createInfo{
        {},
        surface.get(),
        imageCount,
        surfaceFormat.format,
        surfaceFormat.colorSpace,
        extent,
        1,
        vk::ImageUsageFlagBits::eColorAttachment
    };

    auto [graphicsQueueFamilyIndex, presentQueueFamilyIndex] = owo::getGraphicsQueueAndPresentQueue(vulkanContext.vkDevices.physicalDevice, surface);
    uint32_t queueFamilyIndicies[] = {graphicsQueueFamilyIndex, presentQueueFamilyIndex};

    if(graphicsQueueFamilyIndex == presentQueueFamilyIndex) {
        createInfo.imageSharingMode = vk::SharingMode::eExclusive;
        createInfo.queueFamilyIndexCount = 0;
        createInfo.pQueueFamilyIndices = nullptr;
    } else {
        createInfo.imageSharingMode = vk::SharingMode::eConcurrent;
        createInfo.queueFamilyIndexCount = 2;
        createInfo.pQueueFamilyIndices = queueFamilyIndicies;
    }

    createInfo.compositeAlpha = vk::CompositeAlphaFlagBitsKHR::eOpaque;
    createInfo.presentMode = presentMode.presentMode;
    createInfo.oldSwapchain = nullptr;

    auto swapchain = vulkanContext.vkDevices.logicalDevice->createSwapchainKHRUnique(createInfo);

    return {std::move(swapchain)};
}

}