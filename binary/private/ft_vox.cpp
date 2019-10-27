#include <iostream>

#include "Engine.hpp"

int
main()
{
    try {
        Engine engine;

        engine.init();
        engine.run();
    } catch (std::exception const &e) {
        std::cout << "Catched exception: " << e.what() << std::endl;
    }
    return (0);
}