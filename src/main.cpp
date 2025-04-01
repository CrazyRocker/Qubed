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

static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
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
    VkResult err = glfwCreateWindowSurface(*instance, window, nullptr, &surfaceTemp);

    vk::UniqueSurfaceKHR surface(surfaceTemp, *instance);

    std::vector<vk::PhysicalDevice> physicalDevices = instance->enumeratePhysicalDevices();

    std::cout<<"\nList of available devices:";
    for(auto& device : physicalDevices)
        std::cout<<device.getProperties().deviceName<<"\n";

    //Select Physical Device here out of the devices in physicalDevices






        



    while(!glfwWindowShouldClose(window)){
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}