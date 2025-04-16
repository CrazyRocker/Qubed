#pragma once
#include <GLFW/glfw3.h>
namespace owo {

void key_callback(GLFWwindow* window,int key, int scanCode, int action, int mods);
GLFWwindow* createWindow(int WIDTH, int HEIGHT, const char* NAME);

}