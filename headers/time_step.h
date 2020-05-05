#ifndef TIME_STEP_H
#define TIME_STEP_H

class TimeStep
{
    public:
        float getTimeStep() const;
        float getTotalTime() const;
        void setTimeStep(float timeStep);
    private:
        float _timeStep = 0.0f;
        float _totalTime = 0.0f;
};
#endif