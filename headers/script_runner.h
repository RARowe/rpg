#pragma once
#ifndef SCRIPT_RUNNER_H
#define SCRIPT_RUNNER_H
#include <vector>
#include "script_steps/script_step.h"

class ScriptRunner
{
    public:
        ScriptRunner& addStep(ScriptStep* step);
        void processStep();
        void run();
        void clear();
        bool isRunning();
    private:
        std::vector<ScriptStep*> _steps;
        int _currentStep = 0;
        bool _isRunning = false;
};
#endif
