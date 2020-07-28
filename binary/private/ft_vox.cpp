#include <iostream>

#include "ArgsParsing.hpp"
#include "Engine.hpp"

static void
displayHelp()
{
    std::cout << "TODO help" << std::endl;
}

int
main(int argc, char const **argv)
{
    try {
        Options opt = parseArgs(argc, argv);
        if (opt.help) {
            displayHelp();
            exit(0);
        }

        Engine engine;
        engine.init(opt);
        engine.run();
    } catch (std::exception const &e) {
        std::cout << "Catched exception: " << e.what() << std::endl;
    }
    return (0);
}