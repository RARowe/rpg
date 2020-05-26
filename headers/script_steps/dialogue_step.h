#pragma once
#ifndef DIALOGUE_STEP_H
#define DIALOGUE_STEP_H
#include <vector>
#include <iostream>
#include "game_context.h"
#include "script_step.h"
#include "types.h"
#include "menus/text_box.h"
#include <filesystem>
#include <fstream>
namespace fs = std::filesystem;

class DialogueStep : public ScriptStep
{
    public:
        DialogueStep(GameContext* context)
        {
            std::cout << "DialogueStep is not memory managed" << std::endl;
            std::cout << "DialogueStep needs to be refactored" << std::endl;
            _context = context;
            
   	        std::string line;
   	        std::ifstream infile("resources/scripts/trash_dialogue");
            Speech* s = nullptr;
   	        if (infile) {
   	           	while (std::getline(infile, line))
                {
                    if (line.find("##") == 0)
                    {
                        if (s != nullptr) { _speech.push_back(s); }
                        s = new Speech();
                        line.erase(0, 2);
                        s->speaker = line;
                    }
                    else
                    {
                        s->lines.push_back(line);
                    }
   	           	}
   	        }
            _speech.push_back(s);
   	        infile.close();
        }
        void run()
        {
            _context->openTextBox(&_speech);
            _isComplete = true;
        }
    private:
        GameContext* _context;
        std::vector<const Speech*> _speech;
};
#endif
