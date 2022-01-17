#include "types.h"

static bool utils_entities_collide(const Body* b1, const Body* b2) {
    int x2 = b1->x+ b1->w,
        y2 = b1->y + b1->h,
        b2x2 = b2->x + b2->w,
        b2y2 = b2->y + b2->h;
    bool below = b2->y >= y2,
         above = b2y2 <= b1->y,
         left = b2x2 <= b1->x,
         right = b2->x >= x2;
    return !(below || above || left || right);
}

