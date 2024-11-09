#pragma once

#include "vec.hpp"

namespace clgl
{
class Transformable
{
public:

    virtual      ~Transformable()           = default;

    virtual void move(const Vec2F &offset) final;

    virtual void set_origin(const Vec2F &origin) final;
    [[nodiscard]] virtual const Vec2F &get_origin() const final;

    virtual void set_position(const Vec2F &position) final;
    [[nodiscard]] virtual const Vec2F &get_position() const final;

    virtual void set_scale(const Vec2F &scale) final;
    [[nodiscard]] virtual const Vec2F &get_scale() const final;
    [[nodiscard]] virtual const Vec2F &get_inverse_scale() const final;

    virtual void set_rotation(F32 rotation) final;
    virtual F32  rotate(F32 angle) final;
    [[nodiscard]] virtual F32 get_rotation() const final;

protected:

    virtual void apply_transform(Vec2F &point, F32 angle_cos, F32 angle_sin) const final;

    virtual void apply_scaling(Vec2F &point) const final;
    virtual void apply_rotation(Vec2F &point, F32 angle_cos, F32 angle_sin) const final;
    virtual void apply_translation(Vec2F &point) const final;

    virtual void apply_inverse_transform(Vec2F &point, F32 angle_cos, F32 angle_sin) const final;

    virtual void apply_inverse_scaling(Vec2F &point) const final;
    virtual void apply_inverse_rotation(Vec2F &point, F32 angle_cos, F32 angle_sin) const final;
    virtual void apply_inverse_translation(Vec2F &point) const final;

protected:

    Vec2F m_origin = { 0.f, 0.f };
    Vec2F m_position = { 0.f, 0.f };
    Vec2F m_scale = { 1.f, 1.f };
    Vec2F m_inverse_scale = { 1.f, 1.f };
    F32   m_rotation = 0.f;
};
}