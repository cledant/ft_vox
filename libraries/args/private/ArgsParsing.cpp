#include "ArgsParsing.hpp"

#include <cstring>
#include <string_view>
#include <sstream>
#include <stdexcept>
#include <limits>

static void
setHelp(Options &opts, std::string_view arg)
{
    (void)arg;
    opts.help = 1;
}

static void
setInvertMouseY(Options &opts, std::string_view arg)
{
    (void)arg;
    opts.invert_y_axis = 1;
}

static void
setSeed(Options &opts, std::string_view arg)
{
    try {
        int64_t seed = std::stol(arg.substr(7).data());
        if (seed < 0 || seed > std::numeric_limits<uint32_t>::max()) {
            throw std::runtime_error("");
        }
        opts.seed = seed;
    } catch (std::exception const &e) {
        std::stringstream err;
        err << "Seed should be a number between: "
            << std::numeric_limits<uint32_t>::min();
        err << " and " << std::numeric_limits<uint32_t>::max();
        throw std::runtime_error(err.str());
    }
}

Options
parseArgs(int32_t argc, const char **argv)
{
    Options opts = {};
    static int32_t const nb_possible_args = 4;
    static char const possible_args[][32] = {
        "-h", "--help", "--invertMouseY", "--seed="
    };
    static void (*func[nb_possible_args])(Options &, std::string_view) = {
        setHelp, setHelp, setInvertMouseY, setSeed
    };

    for (int i = 1; i < argc; ++i) {
        for (int y = 0; y < nb_possible_args; ++y) {
            if (!std::strncmp(
                  possible_args[y], argv[i], std::strlen(possible_args[y]))) {
                (*func[y])(opts, argv[i]);
                break;
            }
        }
    }
    return (opts);
}