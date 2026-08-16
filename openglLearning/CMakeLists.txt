# CMakeList.txt: openglLearning 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#

# 将 GLFW 源码作为子项目引入（会编译静态库 glfw）
add_subdirectory ("external-source/glfw-3.5.1" glfw-build)

# 将源代码添加到此项目的可执行文件。
add_executable (openglLearning "openglLearning.cpp" "openglLearning.h")

# 链接 GLFW 静态库，并自动获得 include 目录
target_link_libraries (openglLearning PRIVATE glfw)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET openglLearning PROPERTY CXX_STANDARD 20)
endif()

# TODO: 如有需要，请添加测试并安装目标。
