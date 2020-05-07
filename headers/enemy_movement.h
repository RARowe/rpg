#ifndef ENEMY_MOVEMENT_H
#define ENEMY_MOVEMENT_H
#include "enums.h"
#include "movement_handler.h"
#include "game_context.h"
#include "game_math.h"

class EnemyMovement : public MovementHandler
{
    public:
        static EnemyMovement* getInstance(GameContext* context)
        {
            static EnemyMovement handler(context);
            return &handler;
        }
        void update(Entity& e, const float timeStep)
		{
            int startX = e.getX();
            int startY = e.getY();

            if (distanceBetween(e, *_context->getPlayer()) < 250.0f)
            {
                e.move(relativeDirection(e, *_context->getPlayer()), timeStep);
            }
            else
            {
		        _time += timeStep;
                bool outsideMap = startX > 19 * 32 || startX < 0 || startY > 13 * 32 || startY < 0;

                if (outsideMap)
                {
                    _randomThing = 8;
                }
                if (_time > _randomAmountOfTime)
                {
                    _randomAmountOfTime = (float)(std::rand() % 4);
                    _randomThing = std::rand() % 8;
                    _time = 0.0f;
                }

                switch (_randomThing)
                {
                    case 0:
                        e.move(Direction::UP, timeStep);
                        break;
                    case 1:
                        e.move(Direction::DOWN, timeStep);
                        break;
                    case 2:
                        e.move(Direction::LEFT, timeStep);
                        break;
                    case 3:
                        e.move(Direction::RIGHT, timeStep);
                        break;
                    default:
                        break;
                }
            }
            _context->resolveCollision(e, startX, startY);
		}
	private:
		EnemyMovement(GameContext* context) : _context(context) {}
        float _time = 0.0f;
        float _randomAmountOfTime = 0.0f;
        int _randomThing = 0;
		GameContext* _context;
};
#endif