#pragma once

#include <stdexcept>

namespace clgl::exceptions
{
class DrawerNotRegistered : public std::exception
{
public:

    [[nodiscard]] virtual const char *what() const override { return "Cannot access an unregistered drawer"; }
};

class InvalidPath : public std::exception
{
public:

    [[nodiscard]] virtual const char *what() const override { return "Invalid path"; }
};

class InvalidParameter : public std::exception
{
    [[nodiscard]] virtual const char *what() const override { return "Invalid parameter passed"; }
};

namespace winapi
{
class CantFetch : public std::exception
{
public:

    [[nodiscard]] virtual const char *what() const override { return "Couldn\'t fetch data from winapi"; }
};

class CantSet : public std::exception
{
public:

    [[nodiscard]] virtual const char *what() const override { return "Couldn\'t set data through winapi"; }
};
}
}