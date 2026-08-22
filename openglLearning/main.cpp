//
// Created by yigudove on 2026/8/18.
//

#include "main.h"
#include "window_util.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#ifdef _WIN32
#include <windows.h>
#endif

using namespace std;


int renderContent = 0;

///<summary> 生成着色器文件的绝对路径（基于可执行文件位置，向上查找源码目录） </summary>
std::string getShaderPath(const char* fileName)
{
    // 获取 exe 所在目录
    char exePath[MAX_PATH];
    GetModuleFileNameA(NULL, exePath, MAX_PATH);

    std::string dir(exePath);
    // 去掉文件名，得到 exe 所在目录（如 .../cmake-build-debug/openglLearning/）
    size_t pos = dir.find_last_of("\\/");
    if (pos != std::string::npos)
        dir = dir.substr(0, pos);

    // 从 exe 目录向上逐级查找，尝试多个可能的源码目录布局
    while (true)
    {
        // 情况1：当前目录下直接有 GLSL/（源码目录内）
        std::string candidate = dir + "/GLSL/" + fileName;
        std::ifstream test(candidate.c_str());
        if (test.is_open())
            return candidate;

        // 情况2：当前目录下是 openglLearning/GLSL/（源码项目根目录）
        candidate = dir + "/openglLearning/GLSL/" + fileName;
        test.open(candidate.c_str());
        if (test.is_open())
            return candidate;

        // 无法继续上溯（到达盘符根目录）则退出
        size_t sep = dir.find_last_of("\\/");
        if (sep == std::string::npos || dir.size() <= 3)
            break;
        dir = dir.substr(0, sep);
    }
    return "";
}

///<summary> 读取文本文件内容 </summary>
std::string readFile(const char* filePath)
{
    std::ifstream file(filePath);
    if (!file.is_open())
    {
        std::cerr << "无法打开文件: " << filePath << std::endl;
        return "";
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

///<summary> 输入处理 </summary>
void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if(glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    {
        renderContent = 1;
        return;
    }
}

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



    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f
    };

    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // 顶点
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);

    std::string vsPath = getShaderPath("basic.vert");
    std::string vsCode = readFile(vsPath.c_str());
    const char* vertexShaderSource = vsCode.c_str();

    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    // 顶点着色器编译错误检查
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cerr << "顶点着色器编译失败:\n" << infoLog << std::endl;
    }

    // 着色器
    std::string fragPath = getShaderPath("basicFrag.vert");
    std::string vsCode_frag = readFile(fragPath.c_str());
    const char* fragmentShaderSource = vsCode_frag.c_str();

    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // 片元着色器编译错误检查
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cerr << "片元着色器编译失败:\n" << infoLog << std::endl;
    }

    // shader程序
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // 链接错误检查
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cerr << "着色器程序链接失败:\n" << infoLog << std::endl;
    }

    glUseProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // VAO
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);

    // 1. 绑定VAO
    glBindVertexArray(VAO);
    // 2. 把顶点数组复制到缓冲中供OpenGL使用
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // 3. 设置顶点属性指针
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    while(!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClear(GL_COLOR_BUFFER_BIT);

        if (renderContent == 1)
        {
            glUseProgram(shaderProgram);
            glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLES, 0, 3);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    return 0;
}
