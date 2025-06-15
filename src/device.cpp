#include "device.hpp"
#include "utils.hpp"
#include <climits>
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


vk::PhysicalDevice getPhysicalDevice(vk::UniqueHandle<vk::Instance, vk::detail::DispatchLoaderStatic>& instance, vk::UniqueSurfaceKHR& surface) {
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
    }

    return {graphicsQueueFamilyIndex, presentQueueFamilyIndex};
}


std::pair<uint32_t, uint32_t> getGraphicsQueueAndPresentQueue(const vk::PhysicalDevice& physicalDevice, const vk::UniqueSurfaceKHR& surface) {
    std::vector<vk::QueueFamilyProperties> queueFamilyProperties = physicalDevice.getQueueFamilyProperties();
    
    uint32_t graphicsQueueFamilyIndex;
    uint32_t presentQueueFamilyIndex;

    for(int i=0; i<queueFamilyProperties.size(); i++){
        if(queueFamilyProperties[i].queueFlags & vk::QueueFlagBits::eGraphics)
            graphicsQueueFamilyIndex = i;
        if(physicalDevice.getSurfaceSupportKHR(i, surface.get()))
            presentQueueFamilyIndex = i;
    }

    return {graphicsQueueFamilyIndex, presentQueueFamilyIndex};
}


vk::UniqueDevice getUniqueDevice(vk::UniqueInstance& instance, vk::UniqueSurfaceKHR& surface) {
    vk::PhysicalDevice physicalDevice = owo::getPhysicalDevice(instance, surface);
    
    //Getting the QueueFamilies
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



}