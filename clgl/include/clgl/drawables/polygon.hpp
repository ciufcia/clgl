#pragma once

#include "clgl/drawable.hpp"
#include "clgl/transformable.hpp"
#include <vector>

namespace clgl
{
class Polygon : public Drawable, public Transformable
{
public:

                              Polygon() = default;
                              Polygon(U32 point_count, bool filled, const Pixel &pixel);

    void                      draw(ScreenBuffer &screen_buffer) override;
    void                      draw_no_clipping(ScreenBuffer &screen_buffer) override;

    void                      move(const Vec2F &offset) override;

    void                      set_rotation(F32 rotation) override;
    F32                       rotate(F32 angle) override;

    void                      set_point_count(U32 point_count);
    U32                       get_point_count() const;
    void                      set_point(U32 index, const Vec2F &point);
    Vec2F                     get_point(U32 index) const;
    const std::vector<Vec2F> &get_points() const;

public:

    Pixel pixel;
    bool  filled = false;

private:

    std::vector<Vec2F> m_points;
};

namespace utils
{
typedef U8 ClippedPointBoundData;

struct ClippedPolygonPoint
{
    Vec2F coordinates          { 0.f, 0.f };
    ClippedPointBoundData data = 0u;
};

void clip_polygon(const Polygon &target, std::vector<ClippedPolygonPoint> &output, const Vec2F &top_left, const Vec2F &bottom_right);

struct EdgeBucket
{
    F32 inverse_slope = 0.f;
    I32 max_y         = 0;
    F32 current_x     = 0;
};

enum class BoundPosition
{
    Left,
    Top,
    Right,
    Bottom
};

struct BoundEdgeData
{
    BoundPosition position;
    F32           value;
};

void insertion_sort_by_x(std::vector<EdgeBucket> &vec);

void clip_polygon_by_bound(const BoundEdgeData &bound, const std::vector<ClippedPolygonPoint> &old_points, std::vector<ClippedPolygonPoint> &new_points);
void clip_edge_by_bound(const BoundEdgeData &bound, const ClippedPolygonPoint &start, const ClippedPolygonPoint &end, std::vector<ClippedPolygonPoint> &new_points);

template<typename TVec2NumberType>
void clip_polygon(const std::vector<clgl::Vec2<TVec2NumberType>> &target, std::vector<ClippedPolygonPoint> &output, const Vec2F &top_left, const Vec2F &bottom_right) {
    output.reserve(target.size());

    std::vector<ClippedPolygonPoint> old_points;
    old_points.reserve(target.size());
    for (U32 i = 0u; i < target.size(); ++i) {
        old_points.push_back({Vec2F(target[i]), false});
    }

    clip_polygon_by_bound({BoundPosition::Left, top_left.x}, old_points, output);
    old_points = std::move(output); output.clear();
    clip_polygon_by_bound({BoundPosition::Top, top_left.y}, old_points, output);
    old_points = std::move(output); output.clear();
    clip_polygon_by_bound({BoundPosition::Right, bottom_right.x}, old_points, output);
    old_points = std::move(output); output.clear();
    clip_polygon_by_bound({BoundPosition::Bottom, bottom_right.y}, old_points, output);
}
}
};