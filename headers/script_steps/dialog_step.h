#pragma once
#ifndef DIALOG_STEP_H
#define DIALOG_STEP_H
#include "script_step.h"

class DialogStep : public ScriptStep
{
    public:
        DialogStep(GameContext* context, const char* imagePath, const char* text)
        {
            _context = context;
            _imagePath = imagePath;
            _text = text;
        }
        void run()
        {
            _context->openDialog(_imagePath, _text);
            _isComplete = true;
        }
    private:
        GameContext* _context;
        const char* _imagePath;
        const char* _text;
};
#endif
