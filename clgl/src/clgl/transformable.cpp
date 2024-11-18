#include "clgl/transformable.hpp"
#include <limits>

void clgl::Transformable::move(const Vec2F &offset) {
    m_position += offset;
}

void clgl::Transformable::set_origin(const Vec2F &origin) {
    m_origin = origin;
}

const clgl::Vec2F &clgl::Transformable::get_origin() const {
    return m_origin;
}

void clgl::Transformable::set_position(const Vec2F &position) {
    m_position = position;
}

const clgl::Vec2F &clgl::Transformable::get_position() const {
    return m_position;
}

void clgl::Transformable::set_scale(const Vec2F &scale) {
    m_scale = scale;
    m_inverse_scale.x = (scale.x != 0.f) ? 1.f / scale.x: std::numeric_limits<F32>::max();
    m_inverse_scale.y = (scale.y != 0.f) ? 1.f / scale.y: std::numeric_limits<F32>::max();
}

const clgl::Vec2F &clgl::Transformable::get_scale() const {
    return m_scale;
}

const clgl::Vec2F &clgl::Transformable::get_inverse_scale() const {
    return m_inverse_scale;
}

void clgl::Transformable::set_rotation(F32 rotation) {
    m_rotation = rotation;
}

clgl::F32 clgl::Transformable::rotate(F32 angle) {
    m_rotation += angle;
    return m_rotation;
}

clgl::F32 clgl::Transformable::get_rotation() const {
    return m_rotation;
}

void clgl::Transformable::apply_transform(Vec2F &point, F32 angle_cos, F32 angle_sin) const {
    apply_scaling(point);
    apply_rotation(point, angle_cos, angle_sin);
    apply_translation(point);
}

void clgl::Transformable::apply_scaling(Vec2F &point) const {
    Vec2F distance_to_origin = point - get_origin();
    point = { get_origin().x + distance_to_origin.x * get_scale().x, get_origin().y + distance_to_origin.y * get_scale().y };
}

void clgl::Transformable::apply_rotation(Vec2F &point, F32 angle_cos, F32 angle_sin) const {
    Vec2F point_around_origin = point - get_origin();
    point = Vec2F(
        point_around_origin.x * angle_cos - point_around_origin.y * angle_sin,
        point_around_origin.y * angle_cos + point_around_origin.x * angle_sin
    ) + get_origin();
}

void clgl::Transformable::apply_translation(Vec2F &point) const {
    point += get_position() - get_origin();
}

void clgl::Transformable::apply_inverse_transform(Vec2F &point, F32 angle_cos, F32 angle_sin) const {
    apply_inverse_scaling(point);
    apply_inverse_rotation(point, angle_cos, angle_sin);
    apply_inverse_translation(point);
}

void clgl::Transformable::apply_inverse_scaling(Vec2F &point) const {
    Vec2F distance_to_origin = point - get_origin();
    point = { get_origin().x + distance_to_origin.x * get_inverse_scale().x, get_origin().y + distance_to_origin.y * get_inverse_scale().y};
}

void clgl::Transformable::apply_inverse_rotation(Vec2F &point, F32 angle_cos, F32 angle_sin) const {
    Vec2F point_around_origin = point - get_origin();
    point = Vec2F(
        point_around_origin.x * angle_cos - point_around_origin.y * (-angle_sin),
        point_around_origin.y * angle_cos + point_around_origin.x * (-angle_sin)
    ) + get_origin();
}

void clgl::Transformable::apply_inverse_translation(Vec2F &point) const {
    point -= get_position() - get_origin();
}