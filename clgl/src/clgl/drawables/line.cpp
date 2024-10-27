#include "clgl/drawables/line.hpp"

clgl::Line::Line(const Vec2F &end1, const Vec2F &end2, const Pixel &pixel) {
    this->end1 = end1; this->end2 = end2; this->pixel = pixel;
}

void clgl::Line::draw(ScreenBuffer &screen_buffer) {
    Vec2F start = end1;
    Vec2F end   = end2;

    utils::LineClippingData clipping_data = utils::clip_line(start, end, {0.f, 0.f}, screen_buffer.get_max_float_coordinates());

    if (!clipping_data.visible) return;

    draw(screen_buffer, clipping_data.end1, clipping_data.end2);
}

void clgl::Line::move(const Vec2F &offset) {
    end1 += offset;
    end2 += offset;
}

void clgl::Line::draw_no_clipping(ScreenBuffer &screen_buffer) {
    draw(screen_buffer, end1, end2);
}

void clgl::Line::draw(ScreenBuffer &screen_buffer, Vec2F start, Vec2F end) {
    bool steep  = false;

    if (abs(end.x - start.x) < abs(end.y - start.y)) {
        std::swap(start.x, start.y);
        std::swap(end.x, end.y);
        steep = true;
    }

    if (start.x > end.x) std::swap(start, end);

    F32 dx    = end.x - start.x;
    F32 dy    = end.y - start.y;
    F32 slope = (dx != 0.f) ? dy / dx : slope = screen_buffer.get_max_float_coordinate_offset();
    F32 b     = start.y - start.x * slope;

    F32 cy;
    I32 draw_start_x;
    I32 draw_end_x;

    {
        F32 start_pixel_left  = floor(start.x);
        F32 start_pixel_right = start_pixel_left + 1.f;
        F32 distance_to_edge  = start_pixel_right - start.x;
        cy                    = (start_pixel_left + 0.5f) * slope + b;

        if (distance_to_edge >= 0.5f) {
            if (steep) screen_buffer.set_pixel(Vec2U(cy, start_pixel_left), pixel);
            else       screen_buffer.set_pixel(Vec2U(start_pixel_left, cy), pixel);
        }

        draw_start_x = static_cast<I32>(start_pixel_right);
    }

    {
        F32 end_pixel_left   = floor(end.x);
        F32 distance_to_edge = end.x - end_pixel_left;

        if (distance_to_edge >= 0.5f) {
            F32 y = (end_pixel_left + 0.5f) * slope + b;

            if (steep) screen_buffer.set_pixel(Vec2U(y, end_pixel_left), pixel);
            else       screen_buffer.set_pixel(Vec2U(end_pixel_left, y), pixel);
        }
        
        draw_end_x = static_cast<I32>(end_pixel_left);
    }

    for (I32 cx = draw_start_x; cx < draw_end_x; cx++) {
        cy += slope;

        if (steep) screen_buffer.set_pixel(Vec2U(cy, cx), pixel);
        else       screen_buffer.set_pixel(Vec2U(cx, cy), pixel);
    }
}

clgl::utils::LineClippingData clgl::utils::clip_line(const Vec2F &end1, const Vec2F &end2, const Vec2F &top_left, const Vec2F &bottom_right) {
    U8 region_code_1 = assign_cohen_sutherland_region_codes(end1, top_left, bottom_right);
    U8 region_code_2 = assign_cohen_sutherland_region_codes(end2, top_left, bottom_right);

    LineClippingData clipping_data;

    clipping_data.end1 = end1;
    clipping_data.end2 = end2;

    F32 dx = end2.x - end1.x;
    F32 dy = end2.y - end1.y;

    F32 slope         = (dx == 0.f) ? std::numeric_limits<F32>::max() : dy / dx;
    F32 inverse_slope = (dy == 0.f) ? 0.f                             : dx / dy;

    while (true) {
        if ((region_code_1 | region_code_2) == cohen_sutherland_inside_region) break; // line doesn't require clipping
        if ((region_code_1 & region_code_2) != 0b00000000) {
            clipping_data.visible = false;
            return clipping_data;
        }

        U8 current_point_code;
        if (region_code_1 != 0b00000000) current_point_code = region_code_1;
        else                             current_point_code = region_code_2;

        Vec2F clipped { 0.f, 0.f };

        if (current_point_code & cohen_sutherland_left_region) {
            clipped.x = top_left.x;
            clipped.y = end1.y + slope * (top_left.x - end1.x);
        } else if (current_point_code & cohen_sutherland_top_region) {
            clipped.x = end1.x + inverse_slope * (top_left.y - end1.y);
            clipped.y = top_left.y;
        } else if (current_point_code & cohen_sutherland_right_region) {
            clipped.x = bottom_right.x;
            clipped.y = end1.y + slope * (bottom_right.x - end1.x);
        } else if (current_point_code & cohen_sutherland_bottom_region) {
            clipped.x = end1.x + inverse_slope * (bottom_right.y - end1.y);
            clipped.y = bottom_right.y;
        }

        if (current_point_code == region_code_1) {
            clipping_data.end1 = clipped;
            region_code_1      = assign_cohen_sutherland_region_codes(clipped, top_left, bottom_right);
        } else {
            clipping_data.end2 = clipped;
            region_code_2      = assign_cohen_sutherland_region_codes(clipped, top_left, bottom_right);
        }
    }

    return clipping_data;
}

clgl::U8 clgl::utils::assign_cohen_sutherland_region_codes(const Vec2F &point, const Vec2F &top_left, const Vec2F &bottom_right) {
    U8 region_code = cohen_sutherland_inside_region;

    if      (point.x < top_left.x)     region_code |= cohen_sutherland_left_region;
    else if (point.x > bottom_right.x) region_code |= cohen_sutherland_right_region;

    if      (point.y < top_left.y)     region_code |= cohen_sutherland_top_region;
    else if (point.y > bottom_right.y) region_code |= cohen_sutherland_bottom_region;

    return region_code;
}