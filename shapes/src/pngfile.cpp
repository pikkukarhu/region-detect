#include <stdlib.h>
#include <stdio.h>
#include <png.h>

#include "pngfile.hpp"


namespace fi {
namespace pa {
namespace shapes {

pngfile::pngfile(char *filename) {
    
    FILE *fp = fopen(filename, "rb");

    png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png) {
        abort();
    }

    png_infop info = png_create_info_struct(png);
    if (!info) {
        abort();
    }

    png_init_io(png, fp);
    png_read_info(png, info);

    this->width      = png_get_image_width(png, info);
    this->height     = png_get_image_height(png, info);
    this->color_type = png_get_color_type(png, info);
    this->bit_depth  = png_get_bit_depth(png, info);

    // Read any color_type into 8bit depth, RGBA format.
    // See http://www.libpng.org/pub/png/libpng-manual.txt

    if (bit_depth == 16) {
        png_set_strip_16(png);
    }

    if (color_type == PNG_COLOR_TYPE_PALETTE) {
        png_set_palette_to_rgb(png);
    }

    // PNG_COLOR_TYPE_GRAY_ALPHA is always 8 or 16bit depth.
    if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8) {
        png_set_expand_gray_1_2_4_to_8(png);
    }

    if (png_get_valid(png, info, PNG_INFO_tRNS)) {
        png_set_tRNS_to_alpha(png);
    }

    // These color_type don't have an alpha channel then fill it with 0xff.
    if (color_type == PNG_COLOR_TYPE_RGB ||
        color_type == PNG_COLOR_TYPE_GRAY ||
        color_type == PNG_COLOR_TYPE_PALETTE) {

        png_set_filler(png, 0xFF, PNG_FILLER_AFTER);
    }

    if(color_type == PNG_COLOR_TYPE_GRAY ||
        color_type == PNG_COLOR_TYPE_GRAY_ALPHA) {

        png_set_gray_to_rgb(png);
    }

    png_read_update_info(png, info);

    printf("bit_depth= %d row_bytes= %d columns= %d\n", png_get_bit_depth(png, info), (int)png_get_rowbytes(png, info), this->width);

    this->row_pointers = new png_bytep [(sizeof(png_bytep) * height)]; 
    for (int y = 0; y < height; y++) {
        row_pointers[y] = new png_byte [(png_get_rowbytes(png, info))];
    }
    png_read_image(png, row_pointers);

    fclose(fp);
    png_destroy_read_struct(&png, &info, NULL);
}


pngfile::~pngfile() {

    if (this->row_pointers != NULL) {
        for (int y = 0; y < height; y++) {
            delete[] row_pointers[y];
        }
        delete[] row_pointers;
    }
}

}}}