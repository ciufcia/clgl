#pragma once

#include "vec.hpp"

namespace clgl
{
class Transformable
{
public:

    virtual      ~Transformable()           = default;

    virtual void move(const Vec2F &offset)  = 0;

    virtual void set_rotation(F32 rotation) = 0;
    virtual F32  rotate(F32 angle)          = 0;

protected:

    F32  m_rotation = 0.f;
};
}