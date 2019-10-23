#ifndef FT_VOX_WINDOWMANAGER_HPP
#define FT_VOX_WINDOWMANAGER_HPP

#include <array>
#include <string>
#include <cstdint>

#include "glad/glad.h"
#include "GLFW/glfw3.h"

class WindowManager
{
  public:
    WindowManager();
    virtual ~WindowManager();
    WindowManager(WindowManager const &src) = delete;
    WindowManager &operator=(WindowManager const &rhs) = delete;
    WindowManager(WindowManager &&src) = delete;
    WindowManager &operator=(WindowManager &&rhs) = delete;

    //Constants
    static constexpr int32_t WIN_W = 1280;
    static constexpr int32_t WIN_H = 720;
    static constexpr uint16_t KEYS_BUFF_SIZE = 1024;

    // Window related
    void createWindow(std::string &&name);
    void deleteWindow();
    void toggleFullscreen();

    // Event related
    [[nodiscard]] std::array<uint8_t, KEYS_BUFF_SIZE> const &getKeys() const;
    [[nodiscard]] uint8_t shouldClose() const;
    void triggerClose() const;

    // Render Related
    void render() const;
    void clear() const;

  private:
    // Input
    std::array<uint8_t, 1024> _keys;

    // Window related
    GLFWwindow *_win;
    uint8_t _fullscreen;
    int32_t _w;
    int32_t _h;
    int32_t _w_viewport;
    int32_t _h_viewport;
    std::string _win_name;

    // Callbacks
    void _initCallbacks();
};

#endif // FT_VOX_WINDOWMANAGER_HPP
