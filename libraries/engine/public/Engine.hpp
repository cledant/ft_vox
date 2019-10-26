#ifndef FT_VOX_ENGINE_HPP
#define FT_VOX_ENGINE_HPP

#include "IOManager.hpp"

class Engine
{
  public:
    Engine();
    virtual ~Engine() = default;
    Engine(Engine const &src) = delete;
    Engine &operator=(Engine const &rhs) = delete;
    Engine(Engine &&src) = delete;
    Engine &operator=(Engine &&rhs) = delete;

    void run();


  private:
    IOManager _iomanager;
};

#endif // FT_VOX_ENGINE_HPP