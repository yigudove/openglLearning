//
// Created by yigudove on 2026/8/22.
//

#ifndef OPENGLLEARNING_WINDOW_UTIL_H
#define OPENGLLEARNING_WINDOW_UTIL_H

// 前向声明，避免在头文件里引入 GLFW 的具体定义
struct GLFWwindow;

class window_util
{
public:
    // 初始化 GLFW 和 GLAD，并创建并返回一个 OpenGL 3.3 Core 模式的窗口。
    // 创建失败时返回 nullptr。
    static GLFWwindow* createWindow(int width, int height, const char* title);

    // 注销窗口大小变化时的回调，自动更新视口（viewport）
    static void setupFramebufferSizeCallback(GLFWwindow* window);

private:
    // 内部：GLFW 回调函数（必须是静态/全局函数，不能是成员函数）
    static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
};


#endif //OPENGLLEARNING_WINDOW_UTIL_H
