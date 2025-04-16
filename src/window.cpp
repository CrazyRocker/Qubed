#include "window.hpp"
#include <GLFW/glfw3.h>

namespace owo {

void key_callback(GLFWwindow* window,int key, int scanCode, int action, int mods){
    if(key==GLFW_KEY_ESCAPE && action==GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

GLFWwindow* createWindow(int WIDTH, int HEIGHT, const char* NAME){
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, NAME, nullptr, nullptr);

    glfwSetKeyCallback(window, owo::key_callback);

    return window;
}

}