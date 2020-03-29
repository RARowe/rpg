#pragma once
#ifndef SCRIPT_STEP_H
#define SCRIPT_STEP_H
class ScriptStep
{
    public:
        virtual void run() = 0;
        bool isComplete()
        {
            return _isComplete;
        }
    protected:
        bool _isComplete = false;
};
#endif
