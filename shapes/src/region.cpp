#include "region.hpp"
#include "bitmap.hpp"

namespace fi {
namespace pa {
namespace shapes {


region::~region() {
    _data* c = (_data*) this->_first;
    while (c != nullptr) {
        _data* a = c->next;
        delete c;
        c = a;
    }
}
 
void region::reset() {

    _data* c = (_data*) this->_first;
    while (c != nullptr) {
        c->index = 0;
        c = c->next;
    }
    this->_current = this->_first;
    this->queue_first = this->queue_last = this->_first->buffer;

    this->_queue_top = this->_queue_bottom = 0;
} 

void region::push(int row, int col, uint32_t color) {

    // If this buffer block, full, advance to next.
    if (this->_current->index >= BUFFER_SIZE) {
        printf("#Advance to next block\n");
        // If no next block, create new.
        if (this->_current->next == nullptr) {
            printf("#Create new block\n");
            _data* a = new _data;
            a->next = nullptr;
            a->index = 0;
            this->_current->next = (_data *) a;

        } 
        this->_current = this->_current->next;
        this->_current->index = 0;
    }
    pixel* p =  this->_current->buffer + this->_current->index;
    p->x = col;
    p->y = row;
    p->color = color;
  
    // Increase index to point next slot.
    ++ (this->_current->index);  
    ++ (this->_queue_top);
}

pixel* region::pull() {
    // Queue is empty, return null;
    if (this->_queue_bottom >= this->_queue_top) {
        return nullptr;
    }
  
    int block = this->_queue_bottom / BUFFER_SIZE;
    int index = this->_queue_bottom % BUFFER_SIZE;

    _data* c = this->_first;
    for (int i = 0; i < block; ++i) {
        c = c->next;
    }
    pixel* r = c->buffer + index;
    ++this->_queue_bottom;
    return r;
}
 

rect region::getBoundingBox() {

    int xmax, ymax, xmin, ymin;
    xmax = ymax = xmin = ymin = -1;

    // TODO update min/max values already when adding, so avoid extra luuping here.
    _data* c = (_data *) this->_first;
    while (c != nullptr) {
        for (size_t i = 0; i < c->index && i < BUFFER_SIZE; i++) {
           pixel* px = &(c->buffer[i]);
           if (xmax == -1 || xmax < px->x) {
               xmax = px->x;
           }
           if (ymax == -1 || ymax < px->y) {
               ymax = px->y;
           }
           if (xmin == -1 || xmin > px->x) {
               xmin = px->x;
           }
           if (ymin == -1 || ymin > px->y) {
               ymin = px->y;
           }
        }
        c = c->index >= BUFFER_SIZE ? c->next : nullptr;
    }

    // Fix  maxnumbers, becacuse index starts 0
    ymax += 1;
    xmax += 1;
    return rect(xmin, ymin, xmax , ymax);
}
    
void region::readBitMap(bitmap* bm) {

    _data* c = (_data *) this->_first;
    while (c != nullptr) {
        for (size_t i = 0; i < c->index && i < BUFFER_SIZE; i++) {
            pixel* px = &(c->buffer[i]);

            int ymin = bm->getBottom();
            int xmin = bm->getLeft();
            bm->operator[](px->y - ymin)[px->x - xmin] = px->color;
        }
        c = c->index >= BUFFER_SIZE ? c->next : nullptr;
    }
}

}}}