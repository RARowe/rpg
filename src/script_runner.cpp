#include <filesystem>
#include <fstream>
#include "script_runner.h"
#include "game_context.h"
#include "types.h"
namespace fs = std::filesystem;

static void processInstruction(GameContext* c, std::string& line) {
}

void ScriptRunner::processStep(GameContext* c)
{
    // TODO: This should handle a "no steps in file" case
    auto&& line = _steps[_currentStep];
    if (line.find("# ")) {
        processInstruction(c, line);
    } else if (line.find("##")) {
        // TODO: SCRIPT this should be more robust
        Speech s;
        line.erase(0,3);
        s.speaker = line;
        _currentStep++;

        while (!_steps[_currentStep].find("#")) {
            s.lines.push_back(_steps[_currentStep]);
            _currentStep++;
        }
        c->openTextBox(s);
    }

    _currentStep++;
    if (_currentStep > _steps.size()) {
        _isRunning = false;
        _currentStep = 0;
        _steps.clear();
    }
}

void ScriptRunner::run()
{
    _isRunning = true;
   	std::string line;
   	std::ifstream infile("resources/scripts/trash_dialogue");
   	if (infile) {
   	   	while (std::getline(infile, line))
        {
            _steps.push_back(line);
   	   	}
   	}
   	infile.close();
}

bool ScriptRunner::isRunning()
{
    return _isRunning;
}
