#include "clgl/screen_writer.hpp"
#include "clgl/constants.hpp"

clgl::ScreenWriter::~ScreenWriter() {
    delete[] p_char_info_buffer;
}

void clgl::ScreenWriter::write_char_info_buffer() {
    WriteConsoleOutputW(
        m_output_handle,
        p_char_info_buffer,
        m_buffer_size,
        { 0u, 0u },
        &m_write_region
    );
}

void clgl::ScreenWriter::write_string_buffer() {
    SetConsoleCursorPosition(
        m_output_handle,
        { 0u, 0u }
    );

    DWORD characters_written;

    WriteConsoleW(
        m_output_handle,
        string_buffer.data(),
        static_cast<DWORD>(string_buffer.size()),
        &characters_written,
        NULL
    );

    SetConsoleCursorPosition(
        m_output_handle,
        { 0u, 0u }
    );
}

void clgl::ScreenWriter::clear_string_buffer() {
    string_buffer.clear();
}

void clgl::ScreenWriter::set_size(const Vec2U &size) {
    m_buffer_size = COORD{ static_cast<SHORT>(size.x), static_cast<SHORT>(size.y) };
    m_write_region = { 0u, 0u, static_cast<SHORT>(m_buffer_size.X - 1), static_cast<SHORT>(m_buffer_size.Y - 1) };

    delete[] p_char_info_buffer;
    p_char_info_buffer = new CHAR_INFO[static_cast<U32>(m_buffer_size.X) * static_cast<U32>(m_buffer_size.Y)];

    string_buffer.reserve(size.x * size.y * constants::ANSI_coloring_output_length);
}