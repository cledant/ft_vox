#include "WindowManager.hpp"

#include <iostream>
#include <stdexcept>

#define THIS_WIN_PTR static_cast<WindowManager *>(glfwGetWindowUserPointer(win))

WindowManager::WindowManager()
{
    if (!glfwInit()) {
        throw std::runtime_error("Glfw : failed to init");
    }
}

WindowManager::~WindowManager()
{
    glfwTerminate();
}

// Window related
void
WindowManager::createWindow(std::string &&name)
{
    if (!_win) {
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
        glfwWindowHint(GLFW_RED_BITS, 8);
        glfwWindowHint(GLFW_GREEN_BITS, 8);
        glfwWindowHint(GLFW_BLUE_BITS, 8);
        glfwWindowHint(GLFW_ALPHA_BITS, 8);
#ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
        glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, GLFW_FALSE);
#endif
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        _win_name = name;
        _win = glfwCreateWindow(640, 480, _win_name.c_str(), nullptr, nullptr);
        if (!_win) {
            throw std::runtime_error("Glfw : failed to create window");
        }
        glfwSetInputMode(_win, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
        glfwSetWindowUserPointer(_win, this);
        _initCallbacks();
        glfwMakeContextCurrent(_win);
        glfwSwapInterval(0);
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            throw std::runtime_error("GLAD not loaded");
        }
        glfwSetWindowSize(_win, WIN_W, WIN_H);
    }
}

void
WindowManager::deleteWindow()
{
    if (!_win) {
        glfwDestroyWindow(_win);
        _win = nullptr;
    }
}

void
WindowManager::toggleFullscreen()
{}

// Event related
uint8_t
WindowManager::shouldClose() const
{
    return (glfwWindowShouldClose(_win));
}

void
WindowManager::triggerClose() const
{
    glfwSetWindowShouldClose(_win, 1);
}

std::array<uint8_t, WindowManager::KEYS_BUFF_SIZE> const &
WindowManager::getKeys() const
{
    glfwPollEvents();
    return (_keys);
}

// Render related
void
WindowManager::render() const
{
    glfwSwapBuffers(_win);
}

void
WindowManager::clear() const
{
    glClearColor(0.086f, 0.317f, 0.427f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

// Callbacks
void
WindowManager::_initCallbacks()
{
    // Input
    auto input_callback =
      [](GLFWwindow *win, int key, int scancode, int action, int mods) {
          static_cast<void>(scancode);
          static_cast<void>(mods);
          if (key >= 0 && key < 1024) {
              if (action == GLFW_PRESS) {
                  THIS_WIN_PTR->_keys[key] = 1;
              } else if (action == GLFW_RELEASE) {
                  THIS_WIN_PTR->_keys[key] = 0;
              }
          }
      };
    glfwSetKeyCallback(_win, input_callback);

    // Close
    auto close_callback = [](GLFWwindow *win) {
        glfwSetWindowShouldClose(win, GLFW_TRUE);
    };
    glfwSetWindowCloseCallback(_win, close_callback);

    // Window
    auto window_size_callback = [](GLFWwindow *win, int w, int h) {
        THIS_WIN_PTR->_h = h;
        THIS_WIN_PTR->_w = w;
    };
    glfwSetWindowSizeCallback(_win, window_size_callback);

    // Framebuffer
    auto framebuffer_size_callback = [](GLFWwindow *win, int w, int h) {
        THIS_WIN_PTR->_w_viewport = w;
        THIS_WIN_PTR->_h_viewport = h;
        glViewport(0, 0, w, h);
    };
    glfwSetFramebufferSizeCallback(_win, framebuffer_size_callback);
}