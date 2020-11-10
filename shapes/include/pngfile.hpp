#pragma once

#ifndef __PNGFILE_HPP_SHAPES_PA_FI__
#define __PNGFILE_HPP_SHAPES_PA_FI__ 1

#include "image.hpp"
#include <png.h>

namespace fi {
namespace pa {
namespace shapes {

class pngfile : public image {
private:
   int width;
   int height;
   png_byte color_type;
   png_byte bit_depth;

   png_bytep *row_pointers = NULL;

public:
    pngfile(char* filename);
    virtual ~pngfile();

    inline const int getWidth() { return this->width; }
    inline const int getHeight() { return this->height; }

    inline const int getLeft() { return 0; }
    inline const int getBottom() { return 0;} 

    inline const png_byte colorType() { return this->color_type; }
    inline const png_byte bitDepth() { return this->bit_depth; }

    inline uint32_t* operator[](const int index) { return (uint32_t* )this->row_pointers[index]; }

};


}}}

#endif