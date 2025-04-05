#include <iostream>
#include <stdexcept>
#include <vulkan/vulkan.hpp>
#define GLFW_INCLUDE_VULKAN //MUST define this before including GLFW/glfw3.h so it knows to use Vulkan
#include <GLFW/glfw3.h>

//These aren't modified ever in the program, so I've declared as constant. Feel free to change this as needed
const uint32_t WIDTH = 800;
const uint32_t HEIGHT = 600;
const char NAME[] = "Vulkan App Test";

void key_callback(GLFWwindow* window,int key, int scanCode, int action, int mods){
    if(key==GLFW_KEY_ESCAPE && action==GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

static VKAPI_ATTR vk::Bool32 VKAPI_CALL debugCallback(vk::DebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    vk::DebugUtilsMessageTypeFlagsEXT messageType,
    const vk::DebugUtilsMessengerCallbackDataEXT* pCallbackData,
    void* pUserData) {
        std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;
        return VK_FALSE;
}

int main() {
    
    if(!glfwInit())
        throw std::runtime_error("Error while initializing GLFW!");
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, NAME, nullptr, nullptr);
    if(!window)
        throw std::runtime_error("Failed to create GLFW window!");

    glfwSetKeyCallback(window, key_callback);

    vk::ApplicationInfo appInfo(NAME, VK_MAKE_VERSION(0, 1, 0), "No Engine",
                                VK_MAKE_VERSION(0, 1, 0), VK_API_VERSION_1_4);
    
    uint32_t glfwExtensionCount =0;
    const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
    std::vector<const char*> glfwExtensionsVector(glfwExtensions, glfwExtensions + glfwExtensionCount);
    glfwExtensionsVector.push_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
    #ifndef NDEBUG
    glfwExtensionsVector.push_back("VK_EXT_debug_utils");
    std::vector<const char*> layers = std::vector<const char*>{"VK_LAYER_KHRONOS_validation"};
    #endif
    
    auto instance = vk::createInstanceUnique(vk::InstanceCreateInfo{
        vk::InstanceCreateFlags{ VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR }, &appInfo,
        #ifndef NDEBUG
        static_cast<uint32_t>(layers.size()), layers.data(),
        #else
        0, nullptr,
        #endif
        static_cast<uint32_t>(glfwExtensionsVector.size()), glfwExtensionsVector.data()
    });

    vk::detail::DispatchLoaderDynamic dld(instance.get(), vkGetInstanceProcAddr);

    auto messenger = instance->createDebugUtilsMessengerEXTUnique(
    vk::DebugUtilsMessengerCreateInfoEXT( 
        {},
        vk::DebugUtilsMessageSeverityFlagBitsEXT::eError | vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning | vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose | vk::DebugUtilsMessageSeverityFlagBitsEXT::eInfo,
        vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral | vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation | vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance,
        debugCallback,
        nullptr),
    nullptr, dld);
    
    VkSurfaceKHR surfaceTemp;
    if(VK_SUCCESS != glfwCreateWindowSurface(*instance, window, nullptr, &surfaceTemp))
        throw std::runtime_error("Failed to create a Window Surface in GLFW");

    vk::UniqueSurfaceKHR surface(surfaceTemp, *instance);

    std::vector<vk::PhysicalDevice> physicalDevices = instance->enumeratePhysicalDevices();

    std::cout<<"\nList of available devices:\n";
    for(int i=0; i<physicalDevices.size(); i++)
        std::cout<<i<<" - "<<physicalDevices[i].getProperties().deviceName<<"\n";

    std::cout<<"\nPlease enter the device number for the device you want to use (Default: 0): \n";
    char input = std::cin.get();

    vk::PhysicalDevice physicalDevice;
    if(std::isdigit(input))
        physicalDevice = physicalDevices[static_cast<int>(input - '0')];
    else
        physicalDevice = physicalDevices[0];
    
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
        if(presentQueueFamilyIndex == UINT32_MAX)
            throw std::runtime_error("Couldn't find a suitable graphics queue family and a present queue family");
        else
            throw std::runtime_error("Couldn't find a suitable graphics queue family!");
    } else {
        if(presentQueueFamilyIndex == UINT32_MAX)
            throw std::runtime_error("Couldn't find a suitable present Queue family");
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