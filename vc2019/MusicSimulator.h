#pragma once

#include "cinder/audio/audio.h"
#include "cinder/app/App.h"
#include "cinder/Timer.h"

class MusicSimulator {
public:
    MusicSimulator();

    // Setters for parameters
    void setBeatsPerMinute(float bpm);
    void setTimeSignature(int beatsPerMeasure, int noteValue);

    // Functions to start and stop the simulation
    void startSimulation();
    void stopSimulation();

    // Function to update the simulation
    void update();

private:
    // Private helper functions
    void calculateTimePerBeat();

    // Simulation parameters
    float beatsPerMinute;
    int beatsPerMeasure;
    int noteValue;

    // Timing variables
    double timePerBeat;
    double elapsedTime;
    ci::Timer timer;

    // Flag to check if the simulation is running
    bool isRunning;
};
