#include "time_step.h"

float TimeStep::getTimeStep() const
{
    return _timeStep;
}

float TimeStep::getTotalTime() const
{
    return _totalTime;
}

void TimeStep::setTimeStep(float timeStep)
{
    _timeStep = timeStep;
    _totalTime += timeStep;
}