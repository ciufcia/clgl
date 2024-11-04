#pragma once

#include "vec.hpp"
#include "pixel.hpp"
#include <memory>

namespace clgl
{
class Texture
{
public:

    ~Texture();

    void load(const std::string &filename, Character character = L'@');

    // size of "characters" must match the pixel count of the texture
    void load_with_custom_characters(const std::string &filename, const std::string &characters);

    [[nodiscard]] const Vec2U &get_size() const;
    [[nodiscard]] U32          get_pixel_count() const;

    [[nodiscard]] const Pixel *get_pixel_data() const;

    [[nodiscard]] static std::shared_ptr<Texture> create(const std::string &filename, Character character = L'@');
    // size of "characters" must match the pixel count of the texture
    [[nodiscard]] static std::shared_ptr<Texture> create_with_custom_characters(const std::string &filename, const std::string &characters);

private:

    Vec2U  m_size;
    U32    m_pixel_count;

    Pixel *mp_pixels = nullptr;
};
}