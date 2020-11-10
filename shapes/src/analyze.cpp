#include <stdlib.h>
#include <stdio.h>

#include "pngfile.hpp"
#include "region.hpp"
#include "analyze.hpp"

using namespace fi::pa::shapes;
                            
const uint32_t Test1[16][10] = {
//   0x0001 0x0002  0x0003  0x0004  0x0005  0x0006  0x0007  0x0008, 0x0009 0x000A    
    {0,     0,      0xFFF0, 0xFFF0, 0xFFF0, 0,      0,      0,      0,      0xFFF0},
    {0,     0,      0xFFF0, 0,      0xFFF0, 0,      0,      0,      0xFFF0, 0     },
    {0,     0,      0xFFF0, 0,      0xFFF0, 0,      0,      0xFFF0, 0,      0xFFF0},
    {0,     0,      0xFFF0, 0xFFF0, 0xFFF0, 0,      0xFFF0, 0,      0,      0     },
    {0,     0,      0,      0,      0,      0,      0xFFF0, 0,      0,      0xFFF0},
    {0,     0,      0xFFF0, 0,      0xFFF0, 0xFFF0, 0,      0,      0,      0xFFF0},
    {0,     0,      0,      0xFFF0, 0xFFF0, 0,      0,      0,      0,      0xFFF0},
    {0,     0,      0xFFF0, 0xFFF0, 0,      0,      0,      0,      0,      0xFFF0},
    {0,     0,      0,      0,      0,      0,      0,      0,      0,      0xFFF0},
    {0,     0,      0xFFF0, 0xFFF0, 0xFFF0, 0,      0,      0,      0,      0xFFF0},
    {0,     0,      0     , 0     , 0,      0,      0,      0,      0,      0xFFF0},
    {0,     0,      0xFFF0, 0xFFF0, 0xFFF0, 0xAAA0, 0,      0,      0,      0xFFF0},
    {0,     0,      0xFFF0, 0xFFF0, 0xFFF0, 0,      0,      0,      0,      0xFFF0},
    {0,     0,      0xFFF0, 0xFFF0, 0xFFF0, 0,      0,      0,      0,      0xFFF0},
    {0,     0,      0xFFF0, 0xFFF0, 0xFFF0, 0,      0,      0,      0,      0xFFF0},
    {0,     0,      0xFFF0, 0xFFF0, 0xFFF0, 0,      0,      0,      0,      0xFFF0},
};

int main(int argc, char** argv) {
   if (argc >= 2) {
 //       pngfile img = pngfile(argv[1]);

 //       printf("Image %s : width = %d, height = %d \n\n", argv[1], img.getWidth(), img.getHeight());
        rect a(0, 0, 11, 17);
        bitmap img2 = bitmap(a);

        for (int j = 0; j < 16; ++j) {
            for (int i =0; i < 10; ++i) {
                img2[j][i] = Test1[j][i];
            }
        }

        for (int j = 0; j < 16; ++j) {
            std::printf("\n");
            for (int i =0; i < 10; ++i) {
                std::printf("%s", img2[j][i] > 0xA ? "¤" : " ");
            }
        }
        analyze reg;
        reg.detect_regions(img2);
    }
    else {
        std::printf("No filanames");
    }
}

namespace fi {
namespace pa {
namespace shapes {

analyze::~analyze() {
  for (std::vector<bitmap *>::iterator it = this->shapes.begin() ; it != this->shapes.end(); ++it) {
      delete *it;
  }
}

void analyze::detect_regions(image& img) {
   
    // Neighborhood of point. Comment hour index direction.
    const point NH[] = {
        {0, 1},     // 00:00
        {1, 1},     // 01:30
        {1, 0},     // 03:00
        {1, -1},    // 04:30
        {0, -1},    // 06:00
        {-1, -1},   // 07:30
        {-1, 0},    // 09:00
        {-1, 1}     // 10:30
    };

    uint64_t sum = 0;
    union {
        struct {
            uint8_t R;
            uint8_t G;
            uint8_t B;
            uint8_t alpha;
        } _pixel;
        uint32_t _X;
    };

    // Calculate average darknes
    for (int i = 0; i < img.getHeight(); ++i) {
       for (size_t j = 0; j < img.getWidth(); ++j) {
            _X = img[i][j];
            sum += _pixel.R + _pixel.G + _pixel.B;
        }
    }
    uint64_t avg = sum / (img.getHeight() * img.getWidth());

    std::printf("\nAverage darkness = %ld \n", avg);

    region rg;
    // Extract regions
    for (int i = 0; i < img.getHeight(); ++i) {

        for (int j = 0; j < img.getWidth(); ++j) {

            _X = img[i][j];
            int c = _pixel.R + _pixel.G + _pixel.B;

            if (2 * c > avg) {             

                rg.push(i, j, _X);
                img[i][j] = 0;
                pixel* p;
                while ((p = rg.pull()) != nullptr) {

                    for (int k = 0; k < sizeof(NH) / sizeof(point); ++k) {
                        point nh = NH[k];
                        int row = p->y + nh.y;
                        if (row < i || row >= img.getHeight()) {
                            continue;
                        }
                        int col = p->x + nh.x;
                        if (col < 0 || col >= img.getWidth()) {
                            continue;
                        }
                        _X = img[row][col];
                        int c = _pixel.R + _pixel.G + _pixel.B;

                        if (2 * c > avg) {
                            rg.push(row, col, _X);
                            img[row][col] = 0;
                        }
                    }
                } 
                this->shapes.push_back(new bitmap(rg.getBoundingBox()));
                bitmap* img_B = this->shapes.back();
                rg.readBitMap(img_B);

                for (int j = 0; j < img_B->getHeight(); ++j) {
                    std::printf("\n");
                    for (int i =0; i < img_B->getWidth(); ++i) {
                        std::printf("%s", (*img_B)[j][i] > 0xA? "¤" : " ");
                    }
                }
                rg.reset();
                std::printf("\n---------------------------\n");
            } 
        } 
    }

}

}}}