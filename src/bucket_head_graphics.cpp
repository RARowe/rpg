#include "bucket_head_graphics.h"
#include "enums.h"
#include "game_math.h"

GraphicsHandler* BucketHeadGraphics::getInstance(GraphicsContext* context, const Entity* player)
{
    static BucketHeadGraphics handler(context, player);
    return &handler;
}

BucketHeadGraphics::BucketHeadGraphics(GraphicsContext* context, const Entity* player)
{
    _context = context;
    _sprites = context->getTexture("bucket_head/bucket.png");
    SDL_QueryTexture(_sprites, NULL, NULL, &_w, &_h);
    _player = player;
}

BucketHeadGraphics::~BucketHeadGraphics()
{
    SDL_DestroyTexture(_sprites);
}

void BucketHeadGraphics::update(Entity& e, const float timeStep)
{
    SDL_Rect out = { (int)e.getX(), (int)e.getY(), e.getW(), e.getH() };

    float distance = distanceBetween(e, *_player);

    _srcRect.w = _w / 5;
    _srcRect.h = _h;
    if ((BucketHeadStateType)e.getState() == BucketHeadStateType::NORMAL)
    {
        if (distance > 150.0f)
        {
            _srcRect.x = 0;
            _srcRect.y = 0;
        }
        else if (distance > 100.0f)
        {
            _srcRect.x = _srcRect.w;
        }
        else if (distance > 50.0f)
        {
            _srcRect.x = _srcRect.w * 2;
        }
        else
        {
            Uint32 sprite = (SDL_GetTicks() / 500) % 2;
            _srcRect.x = (_w / 5) * (sprite == 0 ? 4 : 3);
            _srcRect.y = 0;
        }
    }
    else
    {
        Uint32 sprite = (SDL_GetTicks() / 500) % 2;
        _srcRect.x = (_w / 5) * (sprite == 0 ? 4 : 3);
        _srcRect.y = 0;
    }
    SDL_RenderCopy(_context->getRenderer(), _sprites, &_srcRect, &out);
    if (e.isEmoting())
    {
        _context->drawEmote(e, "");
    }
}
