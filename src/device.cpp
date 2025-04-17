#include "device.hpp"
#include "utils.hpp"
#include <climits>
#include <iostream>

namespace owo {

vk::PhysicalDevice getPhysicalDevice(vk::UniqueHandle<vk::Instance, vk::detail::DispatchLoaderStatic>& instance) {
    std::vector<vk::PhysicalDevice> physicalDevices = instance->enumeratePhysicalDevices();

    if(physicalDevices.size() > INT_MAX){
        std::cout<<"Bro... chill 💀 Too many GPUs \n"<<std::flush;
        
    }

    std::cout<<"\nList of available devices:\n";
    for(int i=0; i<physicalDevices.size(); i++)
        std::cout<<i<<" - "<<physicalDevices[i].getProperties().deviceName<<"\n";

    while(true){
        uint8_t deviceIndex = owo::input_get_uint8_t("\nPlease enter the device number for the device you want to use (Default: 0): ", 0);
        if (deviceIndex > static_cast<int>(physicalDevices.size() - 1)) {
            std::cout << "The number you have entered is too large! Try again!\n";
        } else{
            return physicalDevices[deviceIndex];
        }
    }
}

std::pair<uint32_t, uint32_t> getGraphicsQueueAndPresentQueue(const vk::PhysicalDevice& physicalDevice, const vk::UniqueSurfaceKHR& surface) {
    std::vector<vk::QueueFamilyProperties> queueFamilyProperties = physicalDevice.getQueueFamilyProperties();
    
    uint32_t graphicsQueueFamilyIndex = UINT32_MAX;
    uint32_t presentQueueFamilyIndex = UINT32_MAX;
    for(int i=0; i<queueFamilyProperties.size(); i++){
        if(queueFamilyProperties[i].queueFlags & vk::QueueFlagBits::eGraphics)
            graphicsQueueFamilyIndex = i;
        if(physicalDevice.getSurfaceSupportKHR(i, surface.get()))
            presentQueueFamilyIndex = i;
    }

    if (graphicsQueueFamilyIndex == UINT32_MAX) {
        if(presentQueueFamilyIndex == UINT32_MAX) {
            throw std::runtime_error("Couldn't find a suitable graphics queue family and a present queue family");
        } else {
            throw std::runtime_error("Couldn't find a suitable graphics queue family!");
        }
    } else {
        if(presentQueueFamilyIndex == UINT32_MAX) {
            throw std::runtime_error("Couldn't find a suitable present Queue family");
        }
    }

    return {graphicsQueueFamilyIndex, presentQueueFamilyIndex};
}

}