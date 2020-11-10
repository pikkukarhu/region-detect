#pragma once

#ifndef __IMGFILE_HPP_SHAPES_PA_FI__
#define __IMGFILE_HPP_SHAPES_PA_FI__ 1

#include <cstdint>

namespace fi {
namespace pa {
namespace shapes {


class image {
public:
    virtual uint32_t* operator[](const int index) = 0;

    virtual const int getWidth() = 0;
    virtual const int getHeight() = 0;
    
    virtual const int getLeft() = 0;
    virtual const int getBottom() = 0;
};

}}}

#endif