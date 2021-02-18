#pragma once
#ifndef SCRIPT_RUNNER_H
#define SCRIPT_RUNNER_H
#include <vector>
#include "script_steps/script_step.h"

class GameContext;

class ScriptRunner
{
    public:
        void processStep(GameContext* c);
        void run();
        bool isRunning();
    private:
        bool _isRunning = false;
        int _currentStep = 0;
        std::vector<std::string> _steps;
};
#endif
