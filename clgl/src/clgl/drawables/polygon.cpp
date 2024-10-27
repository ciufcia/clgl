#include "clgl/drawables/polygon.hpp"
#include "clgl/drawables/line.hpp"
#include "clgl/utils/utils.hpp"
#include <cmath>
#include <numbers>
#include <stdexcept>

clgl::Polygon::Polygon(U32 point_count, bool filled, const Pixel &pixel) {
    this->filled = filled; this->pixel = pixel;
    set_point_count(point_count);
}

void clgl::Polygon::draw(ScreenBuffer &screen_buffer) {
    std::vector<utils::ClippedPolygonPoint> clipped_polygon_points;

    clip_polygon(*this, clipped_polygon_points, { 0.f, 0.f }, screen_buffer.get_max_float_coordinates());

    if (clipped_polygon_points.size() == 0) return;

    if (filled) {
        I32 min_y = std::numeric_limits<I32>::max();
        I32 max_y = std::numeric_limits<I32>::min();
        for (U32 i = 0u; i < clipped_polygon_points.size(); i++) {
            if (min_y > (I32)clipped_polygon_points[i].coordinates.y) min_y = (I32)clipped_polygon_points[i].coordinates.y;
            if (max_y < (I32)clipped_polygon_points[i].coordinates.y) max_y = (I32)clipped_polygon_points[i].coordinates.y;
        }

        I32 polygon_height = max_y - min_y;

        std::vector<std::vector<utils::EdgeBucket>> p_edge_vectors;
        for (U32 i = 0u; i < polygon_height; ++i) {
            p_edge_vectors.push_back({});
        }

        for (U32 i = 0u; i < clipped_polygon_points.size(); ++i) {
            Vec2F f_start = clipped_polygon_points[i].coordinates;
            Vec2F f_end = (i + 1 != clipped_polygon_points.size()) ? clipped_polygon_points[i + 1].coordinates : clipped_polygon_points[0].coordinates;
            Vec2I i_start = { (I32)f_start.x, (I32)f_start.y };
            Vec2I i_end = { (I32)f_end.x, (I32)f_end.y };

            if (f_start.x > f_end.x) {
                std::swap(f_start, f_end);
                std::swap(i_start, i_end);
            }

            if (i_start.y == i_end.y) continue;

            utils::EdgeBucket edge;
            I32 edge_min_y;

            if (f_start.y > f_end.y) {
                edge.max_y = i_start.y;
                edge_min_y = i_end.y;
                edge.current_x = f_end.x;
            } else {
                edge.max_y = i_end.y;
                edge_min_y = i_start.y;
                edge.current_x = f_start.x;
            }

            edge.inverse_slope = (f_end.x - f_start.x) / (f_end.y - f_start.y);

            p_edge_vectors[edge_min_y - min_y].push_back(edge);
        }

        std::vector<utils::EdgeBucket> active_edges;

        for (I32 y = min_y; y < max_y; ++y) {
            for (U32 i = 0u; i < active_edges.size(); ++i) {
                if (active_edges[i].max_y == y) {
                    active_edges.erase(active_edges.begin() + i);
                    --i;
                }
            }

            for (U32 i = 0u; i < p_edge_vectors[y - min_y].size(); ++i) {
                active_edges.push_back(p_edge_vectors[y - min_y][i]);
            }

            utils::insertion_sort_by_x(active_edges);

            for (U32 e = 0u; e < active_edges.size(); e += 2) {
                utils::EdgeBucket &left_edge = active_edges[e];
                utils::EdgeBucket &right_edge = active_edges[e + 1];

                for (U32 x = static_cast<U32>(utils::round_halfway_toward_zero(left_edge.current_x)); x <= static_cast<U32>(right_edge.current_x); ++x) screen_buffer.set_pixel(Vec2U(x, y), pixel);

                left_edge.current_x += left_edge.inverse_slope;
                right_edge.current_x += right_edge.inverse_slope;
            }
        }
    }

    U32 last_point = clipped_polygon_points.size() - 1u;

    for (U32 i = 0u; i < last_point; ++i) {
        if (!filled && clipped_polygon_points[i].data & clipped_polygon_points[i + 1].data) continue;

        Line line { clipped_polygon_points[i].coordinates, clipped_polygon_points[i + 1].coordinates, pixel };
        line.draw_no_clipping(screen_buffer);
    }

    if (filled || !(clipped_polygon_points[last_point].data & clipped_polygon_points[0u].data)) {
        Line line { clipped_polygon_points[last_point].coordinates, clipped_polygon_points[0u].coordinates, pixel };
        line.draw_no_clipping(screen_buffer);
    }
}

void clgl::Polygon::draw_no_clipping(ScreenBuffer &screen_buffer) {
    if (filled) {
        I32 min_y = std::numeric_limits<I32>::max();
        I32 max_y = std::numeric_limits<I32>::min();
        for (U32 i = 0u; i < get_point_count(); i++) {
            if (min_y > (I32)get_point(i).y) min_y = (I32)get_point(i).y;
            if (max_y < (I32)get_point(i).y) max_y = (I32)get_point(i).y;
        }

        I32 polygon_height = max_y - min_y;

        std::vector<std::vector<utils::EdgeBucket>> p_edge_vectors;
        for (U32 i = 0u; i < polygon_height; ++i) {
            p_edge_vectors.push_back({});
        }

        for (U32 i = 0u; i < get_point_count(); ++i) {
            Vec2F f_start = get_point(i);
            Vec2F f_end = (i + 1 != get_point_count()) ? get_point(i + 1) : get_point(0);
            Vec2I i_start = { (I32)f_start.x, (I32)f_start.y };
            Vec2I i_end = { (I32)f_end.x, (I32)f_end.y };

            if (f_start.x > f_end.x) {
                std::swap(f_start, f_end);
                std::swap(i_start, i_end);
            }

            if (i_start.y == i_end.y) continue;

            utils::EdgeBucket edge;
            I32 edge_min_y;

            if (f_start.y > f_end.y) {
                edge.max_y = i_start.y;
                edge_min_y = i_end.y;
                edge.current_x = f_end.x;
            } else {
                edge.max_y = i_end.y;
                edge_min_y = i_start.y;
                edge.current_x = f_start.x;
            }

            edge.inverse_slope = (f_end.x - f_start.x) / (f_end.y - f_start.y);

            p_edge_vectors[edge_min_y - min_y].push_back(edge);
        }

        std::vector<utils::EdgeBucket> active_edges;

        for (I32 y = min_y; y < max_y; ++y) {
            for (U32 i = 0u; i < active_edges.size(); ++i) {
                if (active_edges[i].max_y == y) {
                    active_edges.erase(active_edges.begin() + i);
                    --i;
                }
            }

            for (U32 i = 0u; i < p_edge_vectors[y - min_y].size(); ++i) {
                active_edges.push_back(p_edge_vectors[y - min_y][i]);
            }

            utils::insertion_sort_by_x(active_edges);

            for (U32 e = 0u; e < active_edges.size(); e += 2) {
                utils::EdgeBucket &left_edge = active_edges[e];
                utils::EdgeBucket &right_edge = active_edges[e + 1];

                for (U32 x = static_cast<U32>(utils::round_halfway_toward_zero(left_edge.current_x)); x <= static_cast<U32>(right_edge.current_x); ++x) {
                    screen_buffer.set_pixel(Vec2U(x, y), pixel);
                }

                left_edge.current_x += left_edge.inverse_slope;
                right_edge.current_x += right_edge.inverse_slope;
            }
        }
    }

    U32 last_point = get_point_count() - 1u;

    for (U32 i = 0u; i < last_point; ++i) {
        Line line { get_point(i), get_point(i + 1), pixel };
        line.draw_no_clipping(screen_buffer);
    }

    Line line { get_point(last_point), get_point(0u), pixel };
    line.draw_no_clipping(screen_buffer);
}

void clgl::Polygon::move(const Vec2F &offset) {
    for (U32 i = 0u; i < get_point_count(); ++i) {
        m_points[i] += offset;
    }
}

void clgl::Polygon::set_rotation(F32 rotation) {
    F32 delta = rotation - m_rotation;

    rotate(delta);
}

clgl::F32 clgl::Polygon::rotate(F32 angle) {
    Vec2F centroid { 0.f, 0.f };

    for (U32 i = 0u; i < get_point_count(); ++i) centroid += get_point(i);

    centroid = { centroid.x / static_cast<F32>(get_point_count()), centroid.y / static_cast<F32>(get_point_count()) };

    F32 angle_in_radians = angle * static_cast<F32>(std::numbers::pi) * utils::inverse_180;

    F32 angle_cos = std::cos(angle_in_radians);
    F32 angle_sin = std::sin(angle_in_radians);

    for (U32 i = 0u; i < get_point_count(); ++i) {
        Vec2F point_around_origin = get_point(i) - centroid;

        set_point(i, Vec2F(
            point_around_origin.x * angle_cos - point_around_origin.y * angle_sin,
            point_around_origin.y *  angle_cos + point_around_origin.x * angle_sin
        ) + centroid);
    }

    m_rotation += angle;

    return m_rotation;
}

void clgl::Polygon::set_point_count(U32 point_count) {
    m_points.resize(point_count, { 0.f, 0.f });
}

clgl::U32 clgl::Polygon::get_point_count() const {
    return m_points.size();
}

void clgl::Polygon::set_point(U32 index, const Vec2F &point) {
    m_points[index] = point;
}

clgl::Vec2F clgl::Polygon::get_point(U32 index) const {
    return m_points[index];
}

const std::vector<clgl::Vec2F> &clgl::Polygon::get_points() const {
    return m_points;
}

void clgl::utils::clip_polygon(const Polygon &target, std::vector<ClippedPolygonPoint> &output, const Vec2F &top_left, const Vec2F &bottom_right) {
    clip_polygon<F32>(target.get_points(), output, top_left, bottom_right);
}

void clgl::utils::insertion_sort_by_x(std::vector<EdgeBucket> &vec) {
    for (U32 i = 1; i < vec.size(); ++i) {
        utils::EdgeBucket bucket = vec[i];
        I32 j = i - 1;
        while (j >= 0 && vec[j].current_x > bucket.current_x) {
            vec[j + 1] = vec[j];
            --j;
        }
        vec[j + 1] = bucket;
    }
}

void clgl::utils::clip_polygon_by_bound(const BoundEdgeData &bound, const std::vector<ClippedPolygonPoint> &old_points, std::vector<ClippedPolygonPoint> &new_points) {
    for (I32 i = 0; i < static_cast<I32>(old_points.size()) - 1; ++i) {
        ClippedPolygonPoint start = old_points[i];
        ClippedPolygonPoint end   = old_points[i + 1];

        utils::clip_edge_by_bound(bound, start, end, new_points);
    }

    if (old_points.size() != 0) utils::clip_edge_by_bound(bound, old_points[old_points.size() - 1], old_points[0], new_points);
}

void clgl::utils::clip_edge_by_bound(const BoundEdgeData &bound, const ClippedPolygonPoint &start, const ClippedPolygonPoint &end, std::vector<ClippedPolygonPoint> &new_points) {
    bool first_point_in;
    bool second_point_in;

    switch (bound.position) {
        case BoundPosition::Left:
            first_point_in = start.coordinates.x >= bound.value;
            second_point_in = end.coordinates.x >= bound.value;
            break;
        case BoundPosition::Top:
            first_point_in  = start.coordinates.y >= bound.value;
            second_point_in = end.coordinates.y >= bound.value;
            break;
        case BoundPosition::Right:
            first_point_in  = start.coordinates.x <= bound.value;
            second_point_in = end.coordinates.x <= bound.value;
            break;
        case BoundPosition::Bottom:
            first_point_in  = start.coordinates.y <= bound.value;
            second_point_in = end.coordinates.y <= bound.value;
            break;
        default:
            throw std::runtime_error("Invalid bound position");
    }

    ClippedPointBoundData data = 0u;
    data = _CLGL_SET_BIT(data, (static_cast<int>(bound.position)));

    if (first_point_in) {
        data |= end.data;
        if (second_point_in) {
            new_points.push_back(end);
        } else {
            if (bound.position == BoundPosition::Left || bound.position == BoundPosition::Right) {
                F32 slope = (end.coordinates.y - start.coordinates.y) / (end.coordinates.x - start.coordinates.x);
                ClippedPolygonPoint point { {bound.value, start.coordinates.y + slope * (bound.value - start.coordinates.x)}, data };
                new_points.push_back(point);
            } else {
                F32 inverse_slope = (end.coordinates.x - start.coordinates.x) / (end.coordinates.y - start.coordinates.y);
                ClippedPolygonPoint point { {start.coordinates.x + inverse_slope * (bound.value - start.coordinates.y), bound.value}, data };
                new_points.push_back(point);
            }
        }
    } else {
        data |= start.data;
        if (second_point_in) {
            if (bound.position == BoundPosition::Left || bound.position == BoundPosition::Right) {
                F32 slope = (end.coordinates.y - start.coordinates.y) / (end.coordinates.x - start.coordinates.x);
                ClippedPolygonPoint point { {bound.value, start.coordinates.y + slope * (bound.value - start.coordinates.x)}, data };
                new_points.push_back(point);
            } else {
                F32 inverse_slope = (end.coordinates.x - start.coordinates.x) / (end.coordinates.y - start.coordinates.y);
                ClippedPolygonPoint point { {start.coordinates.x + inverse_slope * (bound.value - start.coordinates.y), bound.value}, data };
                new_points.push_back(point);
            }
            new_points.push_back(end);
        }
    }
}