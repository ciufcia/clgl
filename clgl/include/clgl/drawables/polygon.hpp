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

    void                                    draw(ScreenBuffer &screen_buffer) override;
    void                                    draw_no_clipping(ScreenBuffer &screen_buffer) override;

    void                                    set_point_count(U32 point_count);
    [[nodiscard]] U32                       get_point_count() const;
    void                                    set_point(U32 index, const Vec2F &point);
    [[nodiscard]] Vec2F                     get_point(U32 index) const;
    [[nodiscard]] const std::vector<Vec2F> &get_points() const;

    [[nodiscard]] Vec2F                     calculate_centroid() const;

private:

    void transform_points(std::vector<Vec2F> &output) const;

    void draw_with_overdraw(ScreenBuffer &screen_buffer);
    void draw_no_clipping_with_overdraw(ScreenBuffer &screen_buffer);
    void draw_without_overdraw(ScreenBuffer &screen_buffer);
    void draw_no_clipping_without_overdraw(ScreenBuffer &screen_buffer);

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

void clip_polygon(const std::vector<Vec2F> &target, std::vector<ClippedPolygonPoint> &output, const Vec2F &top_left, const Vec2F &bottom_right);
}
};