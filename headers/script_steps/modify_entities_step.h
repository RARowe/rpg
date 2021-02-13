#pragma once
#ifndef MODIFY_ENTITIES_STEP_H
#define MODIFY_ENTITIES_STEP_H
// TODO: REMOVE all IOSTREAM
#include <iostream>
#include <memory>
#include "enums.h"
#include "game_context.h"
#include "script_step.h"
#include "types.h"

class ModifyEntitiesStep : public ScriptStep
{
    public:
        ModifyEntitiesStep(GameContext& context, bool stand)
        {
            Entities& es = context.entities;
            for (short i = 0; i < es.back; i++) {
                Entity* e = &es.entities[i];
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
        // TODO: I don't like that this keeps a reference to the entity
        Entity* _bucketHead;
};
#endif
