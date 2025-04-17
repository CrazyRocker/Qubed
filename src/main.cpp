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
    try{

    if(!glfwInit()){
        throw std::runtime_error("Error while initializing GLFW!");
    }

    GLFWwindow* window = owo::createWindow(WIDTH, HEIGHT, NAME);        //creates a window and sets a key callback to exit the window when escape is pressed
    if(window == nullptr){
        throw std::runtime_error("Failed to create GLFW window!");
    }
    
    auto instance = owo::createVulkanInstance(NAME);    //Also creates a debug callback (for vulkan)
    
    VkSurfaceKHR surfaceTemp;
    if(VK_SUCCESS != glfwCreateWindowSurface(*instance, window, nullptr, &surfaceTemp)) {
        throw std::runtime_error("Failed to create a Window Surface in GLFW");
    }
    vk::UniqueSurfaceKHR surface(surfaceTemp, *instance);

    vk::PhysicalDevice physicalDevice = owo::getPhysicalDevice(instance);
    
    std::vector<vk::QueueFamilyProperties> queueFamilyProperties = physicalDevice.getQueueFamilyProperties();

    auto [graphicsQueueFamilyIndex, presentQueueFamilyIndex] = owo::getGraphicsQueueAndPresentQueue(physicalDevice, surface);

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

    } catch (std::exception& exception){
        std::cerr<<exception.what();
        return -1;
    }
}