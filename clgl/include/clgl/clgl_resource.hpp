#pragma once

namespace clgl
{
class CLGLResource
{
public:

    virtual ~CLGLResource() = default;

    virtual void on_load() {};
    virtual void on_deload() {};
};
}