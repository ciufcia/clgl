#pragma once

#include "typedefs.hpp"
#include "constants.hpp"
#include "clgl_resource.hpp"

namespace clgl
{
class Color
{
public:

    U8 r = 0u;
    U8 g = 0u;
    U8 b = 0u;

public:

    constexpr          Color() = default;
    constexpr          Color(U8 r, U8 g, U8 b) : r(r), g(g), b(b) {};
                       Color(U32 hex);

    void               set_color(U8 r, U8 g, U8 b);

    void               set_color(U32 hex);

    [[nodiscard]] U32  get_hex() const;

    /**
     *  Calculates perceived color luminance
     * 
     *  Perceived color weights used in the calculation are specified in the _color_weights namespace
     * 
     *  @returns luminance in range [0, 1] (inclusive)
     */
    [[nodiscard]] F32  calculate_luminance() const;

    [[nodiscard]] bool operator==(const Color &other) const;
    [[nodiscard]] bool operator!=(const Color &other) const;
};

namespace utils
{
[[nodiscard]] U32   rgb_to_hex(U8 r, U8 g, U8 b);
[[nodiscard]] Color hex_to_rgb(U32 hex);
}

/**
 *  Holds perceived color weights
 * 
 *  These weights can be changed, but:
 * 
 *  - They must add up to 1
 *  - The color mappings file should be regenerated
 */
namespace _color_weights
{
constexpr F32 red   = 0.2126f;
constexpr F32 green = 0.7152f;
constexpr F32 blue  = 0.0722f;
}

constexpr F32 _inverse_255 = 1.f / 255.f;

/**
 *  Holds color mapping data for a specific color
 */
struct ColorMapping
{
    /**
     *  Represent windows console character foreground color code
     *  
     *  Possible values can be found in clgl::utils::terminal_16_colors::ForegroundColor
     */
    U8 color_code;

    /**
     *  Represent color luminance
     * 
     *  Can store values in range [0, 15] (inclusive),
     *  where 0 is black and 15 is white
     */
    U8 brightness_value;
};

/**
 *  Stores color mapping data for all 24-bit colors.
 * 
 *  Once loaded this objects takes up a lot of memory, so beware
 */
class ColorMappings : public CLGLResource
{
public:

                               ~ColorMappings();

    /**
     *  Loads the color mapping data from the clgl_resources folder
     */
    void                       load();

    [[nodiscard]] ColorMapping get_color_mapping(Color color) const;
    [[nodiscard]] ColorMapping get_color_mapping(U32 hex) const;

private:

    const std::string m_data_path = constants::resources_folder_path + "color_mappings.clcm";

    U8               *mp_data     = new U8[constants::color_space_size_24bit];
};
}