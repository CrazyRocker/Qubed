#include "instance.hpp"
#include <iostream>

namespace owo {

VKAPI_ATTR vk::Bool32 VKAPI_CALL debugCallback(vk::DebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    vk::DebugUtilsMessageTypeFlagsEXT messageType,
    const vk::DebugUtilsMessengerCallbackDataEXT* pCallbackData,
    void* pUserData) {
        std::cout << "validation layer: " << pCallbackData->pMessage << std::endl;
        return VK_FALSE;
}


std::vector<const char*> getInstanceExtensionsVector() {
    uint32_t glfwExtensionCount = 0;
    const char** instanceExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    std::vector<const char*> instanceExtensionsVector;
    instanceExtensionsVector.reserve(glfwExtensionCount + 1 
    #ifndef NDEBUG
        + 1     //If debug is enabled, we want to reserve one more space for the VK_EXT_debug_utils extension
    #endif
    );

    for(int i=0; i<glfwExtensionCount; i++)
        instanceExtensionsVector.push_back(instanceExtensions[i]);
    instanceExtensionsVector.push_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);

    return instanceExtensionsVector;
}


VulkanInstance createVulkanInstance(const char* NAME) {
    auto instanceExtensionsVector = getInstanceExtensionsVector();
    
    #ifndef NDEBUG
    instanceExtensionsVector.push_back("VK_EXT_debug_utils");   //TODO: CHECK IF THESE ARE AVAILABLE THEN REQUEST THEM.
    const char* layers[] = {"VK_LAYER_KHRONOS_validation"};   //Even in a debug build, I'd rather let people use the software without the validation layers
    #endif
    
    vk::ApplicationInfo appInfo{NAME, VK_MAKE_VERSION(0, 1, 0),
                                "No Engine", VK_MAKE_VERSION(0, 1, 0),
                                VK_API_VERSION_1_4};

    auto instance = vk::createInstanceUnique(vk::InstanceCreateInfo{
        vk::InstanceCreateFlags{ VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR }, &appInfo,
        #ifndef NDEBUG
        static_cast<uint32_t>(sizeof(layers)/sizeof(layers[1])), layers,
        #else
        0, nullptr,
        #endif
        static_cast<uint32_t>(instanceExtensionsVector.size()), instanceExtensionsVector.data()
    });
    
    vk::detail::DispatchLoaderDynamic dld(instance.get(), vkGetInstanceProcAddr);
    
    #ifndef NDEBUG
    auto debugMessenger = instance->createDebugUtilsMessengerEXTUnique(
    vk::DebugUtilsMessengerCreateInfoEXT( 
        {},
        vk::DebugUtilsMessageSeverityFlagBitsEXT::eError | vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning | vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose | vk::DebugUtilsMessageSeverityFlagBitsEXT::eInfo,
        vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral | vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation | vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance,
        owo::debugCallback,
        nullptr),
    nullptr, dld);
    #else
    auto debugMessenger = nullptr;
    #endif
    return {std::move(instance), std::move(debugMessenger)};
}


vk::UniqueSurfaceKHR getUniqueSurface(vk::UniqueInstance& instance, GLFWwindow* window) {
    VkSurfaceKHR surfaceTemp;
    if(VK_SUCCESS != glfwCreateWindowSurface(*instance, window, nullptr, &surfaceTemp)) {
        throw std::runtime_error("Failed to create a Window Surface in GLFW");
    }
    return vk::UniqueSurfaceKHR(surfaceTemp, *instance);
}

}