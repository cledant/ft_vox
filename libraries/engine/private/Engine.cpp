#include "Engine.hpp"

Engine::Engine()
  : _iomanager()
{
}

void
Engine::run()
{
    _iomanager.createWindow("ft_vox");
    while (!_iomanager.shouldClose()) {
        _iomanager.render();
    }
}