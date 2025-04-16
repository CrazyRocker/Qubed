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

}