#include "MusicSimulator.h"

MusicSimulator::MusicSimulator()
    : beatsPerMinute(60.0f), beatsPerMeasure(4), noteValue(4), timePerBeat(0.0), elapsedTime(0.0), isRunning(false) {
    calculateTimePerBeat();
}

void MusicSimulator::setBeatsPerMinute(float bpm) {
    beatsPerMinute = bpm;
    calculateTimePerBeat();
}

void MusicSimulator::setTimeSignature(int beatsPerMeasure, int noteValue) {
    this->beatsPerMeasure = beatsPerMeasure;
    this->noteValue = noteValue;
    calculateTimePerBeat();
}

void MusicSimulator::startSimulation() {
    isRunning = true;
    timer.start();
}

void MusicSimulator::stopSimulation() {
    isRunning = false;
    timer.stop();
    elapsedTime = 0.0;
}

double MusicSimulator::getTime() {
    return timer.getSeconds();
}

void MusicSimulator::update() {
    if (isRunning) {
        double deltaTime = timer.getSeconds();
        timer.start(); // Reset timer for the next frame

        elapsedTime += deltaTime;

        // Your simulation logic goes here
        // For example, triggering events based on the current elapsed time, beats, etc.
    }
}

void MusicSimulator::calculateTimePerBeat() {
    timePerBeat = 60.0 / beatsPerMinute;
}

double MusicSimulator::getTimePerBeat() {
    return timePerBeat;
}
