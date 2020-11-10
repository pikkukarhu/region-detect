#pragma once

#ifndef __REGION_HPP_SHAPES_PA_FI__
#define __REGION_HPP_SHAPES_PA_FI__  1

#include <cstdint>
#include "bitmap.hpp"

namespace fi {
namespace pa {
namespace shapes {

#define BUFFER_SIZE 1024

struct point {
    int x, y;
    inline point(int _x, int _y) {x = _x; y = _y; }
};

struct pixel {
    int x, y;
    int32_t color;

    pixel() {x=y=0; color = 0; std::printf("."); }
};

class region {
private:
 
    struct _data {
        _data* next;
        pixel buffer[BUFFER_SIZE];
        int index;
    };

    _data* _current;
    _data* _first;

    pixel* queue_first; // Points next pixel to retrieve with pull od FIFO -buffer
    pixel* queue_last;  // Points last pixel of FIFO buffer,

    _data* queue_first_buffer;  // Databuffer block for FIFO start
    _data* queue_last_buffer;   // Databuffer block of FIFO end

    int _queue_bottom = 0;
    int _queue_top = 0;

public:
   
    inline region() { 
        this->_current = new _data; std::printf("\n");
        this->_current->index = 0;
        this->_current->next = nullptr;

        this->_first = this->_current;
        this->queue_first = queue_last = this->_current->buffer; 
        this->queue_first_buffer = this->queue_last_buffer = this->_current;
    }

    ~region();

    void reset();

    void push(int x, int y, uint32_t color);
    pixel* pull();

    rect getBoundingBox();
    void readBitMap(bitmap* bm);
};


}}}

#endif