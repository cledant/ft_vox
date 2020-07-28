#ifndef FT_VOX_ARGSPARSING_HPP
#define FT_VOX_ARGSPARSING_HPP

#include <optional>
#include <cstdint>

typedef struct Options
{
    std::optional<uint64_t> seed;
    uint8_t help;
    uint8_t invert_y_axis;
} Options;

Options parseArgs(int32_t argc, char const **argv);

#endif // FT_VOX_ARGSPARSING_HPP
