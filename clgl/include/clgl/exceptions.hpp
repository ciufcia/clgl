#pragma once

#include <stdexcept>

namespace clgl::exceptions
{
class DrawerNotRegistered : public std::exception
{
public:

    [[nodiscard]] const char *what() { return "Cannot access an unregistered drawer"; }
};

class InvalidPath : public std::exception
{
public:

    [[nodiscard]] const char *what() { return "Invalid path"; }
};

class InvalidParameter : public std::exception
{
    [[nodiscard]] const char *what() { return "Invalid parameter passed"; }
};

namespace winapi
{
class CantFetch : public std::exception
{
public:

    [[nodiscard]] const char *what() { return "Couldn\'t fetch data from winapi"; }
};

class CantSet : public std::exception
{
public:

    [[nodiscard]] const char *what() { return "Couldn\'t set data through winapi"; }
};
}
}