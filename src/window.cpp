#include "window.hpp"
#include <stdexcept>

namespace owo {

void key_callback(GLFWwindow* window,int key, int scanCode, int action, int mods){
    if(key==GLFW_KEY_ESCAPE && action==GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

GLFWwindow* createWindow(int WIDTH, int HEIGHT, const char* NAME){
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, NAME, nullptr, nullptr);
    
    if(!window)
        throw std::runtime_error("Oops! Window creation failed, hehe :3");
    
    glfwSetKeyCallback(window, owo::key_callback);

    return window;
}

}