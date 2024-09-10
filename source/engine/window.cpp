#include "window.h"
#include "imgui_layer.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

GLFWwindow* window;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void process_input(GLFWwindow* window);

bool init_window()
{
#if _DEBUG
    glfwInitHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
#endif
    if (!glfwInit())
        return false;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);

    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(window);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    glfwSwapInterval(0);

    imgui_init(window);

    return true;
}

bool update_window()
{
    return !glfwWindowShouldClose(window);    
}

void terminate_window()
{
    imgui_terminate();
    glfwTerminate();
}

swap_buffers get_swap_buffers_func()
{
    auto glfw_swap_buffers = [](void* window){
        glfwSwapBuffers(static_cast<GLFWwindow*>(window));
    };
    return (swap_buffers)glfw_swap_buffers;
}

void* get_window()
{
    return static_cast<void*>(window);
}
