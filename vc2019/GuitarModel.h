#pragma once

#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Batch.h"
#include "cinder/gl/GlslProg.h"
#include "cinder/ObjLoader.h"
#include "Helpers.h"

using namespace ci;

class GuitarModel {
public:
    GuitarModel();
    void setup();
    void draw();

private:
    ci::gl::BatchRef mModelBatch;
};

