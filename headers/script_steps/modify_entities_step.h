#pragma once
#ifndef MODIFY_ENTITIES_STEP_H
#define MODIFY_ENTITIES_STEP_H
#include <iostream>
#include "entity.h"
#include "enums.h"
#include "game_context.h"
#include "script_step.h"

class ModifyEntitiesStep : public ScriptStep
{
    public:
        ModifyEntitiesStep(GameContext& context, bool stand)
        {
            for (auto e : context.getEntities())
            {
                if (e->getType() == EntityType::BUCKET_HEAD)
                {
                    _bucketHead = e;
                    break;
                }
            }
            _stand = stand;
        }
        void run()
        {
            if (_stand)
            {
                _bucketHead->setState((int)BucketHeadStateType::STANDING);
                _bucketHead->setEmote(true);
            }
            else
            {
                _bucketHead->setState((int)BucketHeadStateType::NORMAL);
                _bucketHead->setEmote(false);
            }
            _isComplete = true;
        }
    private:
        bool _stand = false;
        Entity* _bucketHead = nullptr;
};
#endif
