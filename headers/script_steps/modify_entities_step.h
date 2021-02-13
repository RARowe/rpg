#pragma once
#ifndef MODIFY_ENTITIES_STEP_H
#define MODIFY_ENTITIES_STEP_H
#include <iostream>
#include <memory>
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
                if (e->type == EntityType::BUCKET_HEAD)
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
                _bucketHead->state = (int)BucketHeadStateType::STANDING;
                _bucketHead->isEmoting = true;
            }
            else
            {
                _bucketHead->state = (int)BucketHeadStateType::NORMAL;
                _bucketHead->isEmoting = false;
            }
            _isComplete = true;
        }
    private:
        bool _stand = false;
        std::shared_ptr<Entity> _bucketHead;
};
#endif
