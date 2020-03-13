#ifndef HIT_BOX_H
#define HIT_BOX_H
typedef struct HitBox
{
    int x1, y1;
    int x2, y2;
} HitBox;

typedef struct HitBoxPoint
{
    int x, y;
} HitBoxPoint;

bool pointInHitBox(const HitBoxPoint* p, const HitBox* h)
{
    return p->x > h->x1 &&
        p->x < h->x2 &&
        p->y > h->y1 &&
        p->y < h->y2;
}

bool hitBoxesIntersect(const HitBox* h1, const HitBox* h2)
{
    HitBoxPoint pl = {h2->x1, h2->y1},
                p2 = {h2->ll
    return h2->x1 > h1->x1 && h2-> x1
}
#endif
