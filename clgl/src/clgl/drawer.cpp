#include "clgl/drawer.hpp"
#include "clgl/constants.hpp"

clgl::CLGLResourceManager * clgl::Drawer::get_resource_manager() const {
    return mp_resource_manager;
}

void clgl::utils::get_ANSI_color(std::wstring &output, Color color) {
    output.reserve(constants::ANSI_color_length);

    output += constants::ESCC;
    output += std::to_wstring(static_cast<U32>(color.r));
    output += L";";
    output += std::to_wstring(static_cast<U32>(color.g));
    output += L";";
    output += std::to_wstring(static_cast<U32>(color.b));
    output += constants::ESCE;
}

void clgl::utils::get_ANSI_color_reset(std::wstring &output) {
    output += L"\x1b[0m";
}

void clgl::utils::color_with_ANSI(std::wstring &output, const std::wstring &character, Color color) {
    output.reserve(constants::ANSI_coloring_output_length);

    get_ANSI_color(output, color);
    
    output += character;
}