#pragma once
#ifndef __ANALYZE_HPP_SHAPES_PA_FI__
#define __ANALYZE_HPP_SHAPES_PA_FI__ 1

#include "image.hpp"
#include "bitmap.hpp"

#include <png.h>
#include <vector>

namespace fi {
namespace pa {
namespace shapes {

class analyze {
private:
    std::vector<bitmap *> shapes;
public:
    inline analyze() {}
    ~analyze();
    void detect_regions(image& img);
};


}}}
#endif
