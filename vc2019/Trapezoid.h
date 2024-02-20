#pragma once

#include "cinder/gl/gl.h"

class Trapezoid {
public:
    Trapezoid();
    void setup();
    void draw();

private:
    ci::gl::VboMeshRef mTrapezoidMesh;
};
