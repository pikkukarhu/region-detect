#pragma once

#ifndef __BITMAP_HPP_SHAPES_PA_FI__
#define __BITMAP_HPP_SHAPES_PA_FI__ 1

#include "image.hpp"
#include <png.h>
#include <stdlib.h>
#include <iostream>

namespace fi {
namespace pa {
namespace shapes {

struct rect {
    int x1, y1; // Left bottom
    int x2, y2; // right top;

    inline rect(int _x1, int _y1, int _x2, int _y2) {x1 = _x1, y1 = _y1; x2 = _x2, y2 = _y2; }
};

class bitmap : public image {
private:
    int left, bottom;
    int width, height;
    uint32_t* _bitmap;
public:

    inline bitmap(const rect area) {

        left = area.x1;
        bottom = area.y1;
        
        width = abs(area.x2 - area.x1);
        height = abs(area.y2 - area.y1);

        this->_bitmap = new uint32_t[(width +1 ) * (height + 1)];

        printf (" created new bitmap %d x %d", width, height);
    }

    /**
     * Move constructor moves data from source to this.
     * Allow return bitmap object without deleting reserved heap.
    ne bitmap(bitmap&& src) : _bitmap(nullptr) {
        this->area = src.area;
        this->_bitmap = src._bitmap;

        src.area.x1 = 
        src.area.x2 =
        src.area.y1 =
        src.area.y2 = 0;
        src._bitmap = nullptr;

        printf("move constructor - data moved to new bitmap object!");
    }
 */
/*

    inline bitmap& operator= (const bitmap& other) {
        if (this == &other) {
            return *this;
        }
        if (_bitmap != nullptr) {
            delete[] _bitmap;
        }
        area = other.area;
        int w = abs(area.x2 - area.x1);
        int h = abs(area.y2 - area.y1);

        std::copy(other._bitmap, other._bitmap + w * h, _bitmap);
         printf("copy constructor - data moved to new bitmap object!");
        return *this;
    }
*/
    inline ~bitmap() { if (this->_bitmap != nullptr) { printf("delete non-null bitmp!\n"); delete[] this->_bitmap; } else { printf("No data to delete\n");}}

    inline const int getWidth() { return width; }
    inline const int getHeight() { return height; }

    inline const int getLeft() { return left; }
    inline const int getBottom() { return bottom; }

    inline uint32_t* operator[](int index) {return this->_bitmap + (index * getWidth());}
};

}}}
#endif