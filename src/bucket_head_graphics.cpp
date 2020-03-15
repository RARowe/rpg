#include <math.h>
#include "bucket_head_graphics.h"
#include "enums.h"

BucketHeadGraphics::BucketHeadGraphics(GraphicsContext& context, const Entity* player)
{
    _sprites = context.getTexture("bucket_head/bucket.png");
    SDL_QueryTexture(_sprites, NULL, NULL, &_w, &_h);
    _player = player;
}

BucketHeadGraphics::~BucketHeadGraphics()
{
    SDL_DestroyTexture(_sprites);
}

static inline int secondPower(int x)
{
    return x * x;
}

void BucketHeadGraphics::update(Entity& e, SDL_Renderer* renderer)
{
    int x = _player->getX(),
        y = _player->getY();

    int distance = sqrt
    (
        secondPower(e.getX() - x) +
        secondPower(e.getY() - y)
    );

    _srcRect.w = _w / 5;
    _srcRect.h = _h;
    if (distance > 150)
    {
        _srcRect.x = 0;
        _srcRect.y = 0;
    }
    else if (distance > 100)
    {
        _srcRect.x = _srcRect.w;
    }
    else if (distance > 50)
    {
        _srcRect.x = _srcRect.w * 2;
    }
    else
    {
        Uint32 sprite = (SDL_GetTicks() / 500) % 2;
        _srcRect.x = (_w / 5) * (sprite == 0 ? 4 : 3);
        _srcRect.y = 0;
    }
    SDL_RenderCopy(renderer, _sprites, &_srcRect, &_outRect);
}
