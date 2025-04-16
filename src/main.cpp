#include <iostream>
#include <vulkan/vulkan.hpp>
#define GLFW_INCLUDE_VULKAN //MUST define this before including GLFW/glfw3.h so it knows to use Vulkan
#include <GLFW/glfw3.h>

#include "window.hpp"
#include "instance.hpp"
#include "device.hpp"

//These aren't modified ever in the program, so I've declared as constant. Feel free to change this as needed
constexpr uint32_t WIDTH = 800;
constexpr uint32_t HEIGHT = 600;
constexpr char NAME[] = "Vulkan App Test";

int main() {
    
    if(!glfwInit()){
        std::cout << "Error while initializing GLFW!" << std::endl;
        return -1;
    }

    GLFWwindow* window = owo::createWindow(WIDTH, HEIGHT, NAME);
    if(window == nullptr){
        std::cout<<"Failed to create GLFW window!"<<std::endl;
        return -1;
    }
    
    auto instance = owo::createVulkanInstance(NAME);    //Also creates a debug callback (for vulkan)
    
    VkSurfaceKHR surfaceTemp;
    if(VK_SUCCESS != glfwCreateWindowSurface(*instance, window, nullptr, &surfaceTemp)) {
        std::cout << "Failed to create a Window Surface in GLFW" << std::endl;
        return -1;
    }
    vk::UniqueSurfaceKHR surface(surfaceTemp, *instance);

    vk::PhysicalDevice physicalDevice = owo::getPhysicalDevice(instance);
    
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
            std::cout << "Couldn't find a suitable graphics queue family and a present queue family" << std::endl;
            return -1;
        } else {
            std::cout << "Couldn't find a suitable graphics queue family!" << std::endl;
            return -1;
        }
    } else {
        if(presentQueueFamilyIndex == UINT32_MAX) {
            std::cout << "Couldn't find a suitable present Queue family" << std::endl;
            return -1;
        }
    }

    std::cout << "Graphics Queue Family Index: " << graphicsQueueFamilyIndex << "\n";
    std::cout << "Present Queue Family Index: " << presentQueueFamilyIndex << "\n";

























    std::cout<<std::flush;      //Flushing the output buffer to ensure the std::cout messages appear immediately.
                                //Otherwise, the messages may just stay in the output buffer and only get printed later
                                //after execution completes or something triggers a flush

    while(!glfwWindowShouldClose(window)){
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}