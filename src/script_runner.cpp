#include "script_runner.h"

ScriptRunner& ScriptRunner::addStep(ScriptStep* step)
{
    _steps.push_back(step);
    return *this;
}

void ScriptRunner::processStep()
{
    ScriptStep& step = *_steps[_currentStep];

    if (step.isComplete())
    {
        _currentStep++;

        if (_currentStep == _steps.size())
        {
            clear();
        }
    }
    else
    {
        step.run();
    }
}

void ScriptRunner::run()
{
    _isRunning = true;
}

void ScriptRunner::clear()
{
    for (auto s : _steps)
    {
        delete s;
    }
    _steps.clear();
    _currentStep = 0;
    _isRunning = false;
}

bool ScriptRunner::isRunning()
{
    return _isRunning;
}
