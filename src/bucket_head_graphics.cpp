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
    _player = player;
}

void BucketHeadGraphics::update(Entity& e, const TimeStep timeStep)
{
    int sprite = 0;
    if ((BucketHeadStateType)e.getState() == BucketHeadStateType::NORMAL)
    {
        float distance = distanceBetween(e, *_player);
        if (distance > 150.0f)
        {
            sprite = 0;
        }
        else if (distance > 100.0f)
        {
            sprite = 1;
        }
        else if (distance > 50.0f)
        {
            sprite = 2;
        }
        else
        {
            sprite = (((int)(timeStep.getTotalTime() / 0.5)) % 2) + 3;
        }
    }
    else
    {
        sprite = (((int)(timeStep.getTotalTime() / 0.5)) % 2) + 3;
    }
    _context->drawSprite("bucket_head", sprite, e);

    if (e.isEmoting())
    {
        _context->drawEmote(e, "");
    }
}
