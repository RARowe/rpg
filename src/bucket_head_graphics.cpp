#include "bucket_head_graphics.h"
#include "enums.h"


BucketHeadGraphics::BucketHeadGraphics(GraphicsContext& context)
{
    _sprites = context.getTexture("bucket_head/bucket.png");
    SDL_QueryTexture(_sprites, NULL, NULL, &_w, &_h);
}

BucketHeadGraphics::~BucketHeadGraphics()
{
    SDL_DestroyTexture(_sprites);
}

void BucketHeadGraphics::update(Entity& e, SDL_Renderer* renderer)
{
    Uint32 sprite = (SDL_GetTicks() / 500) % 2;
    _srcRect.x = (_w / 5) * (sprite == 0 ? 4 : 3);
    _srcRect.y = 0;
    _srcRect.w = _w / 5;
    _srcRect.h = _h;
    SDL_RenderCopy(renderer, _sprites, &_srcRect, &_outRect);
}
