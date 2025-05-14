#include "device.hpp"
#include "utils.hpp"
#include <climits>
#include <iostream>

namespace owo {


SuitabilityInfo isPhysicalDeviceSuitable(vk::PhysicalDevice device, vk::UniqueSurfaceKHR& surface){
    auto [graphicsQueueFamilyIndex, presentQueueFamilyIndex] = getGraphicsQueueAndPresentQueue_optional(device, surface);
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

}