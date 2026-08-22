//
// Created by yigudove on 2026/8/18.
//

#include "main.h"
#include "window_util.h"

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#ifdef _WIN32
#include <windows.h>
#endif

using namespace std;

///<summary> 输入处理 </summary>
void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

float vertices[] = {
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f
};

int main()
{
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif
    cout << "程序入口" << endl;

    // 创建窗口（内部已完成 GLFW 初始化、上下文创建、GLAD 加载等）
    GLFWwindow* window = window_util::createWindow(800, 600, "LearnOpenGL");
    if (window == nullptr)
    {
        return -1;
    }

    glViewport(0, 0, 800, 600);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    // 注册调整窗口大小的函数
    window_util::setupFramebufferSizeCallback(window);

    while(!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClear(GL_COLOR_BUFFER_BIT);


        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    return 0;
}
