#include "GuitarModel.h"

GuitarModel::GuitarModel() {
	setup();
}

void GuitarModel::setup() {
    // Load .obj model
    std::string modelPath = "C:/Users/taikilpatrick/Documents/CSC473/Project/Guitar/assets/guitarModel.obj";
    //geom::Source modelDataSource = ci::app::loadAsset(modelPath);
    ci::TriMeshRef model = ci::TriMesh::create(modelDataSource);
    ci::gl::GlslProgRef shader = ci::gl::getStockShader(ci::gl::ShaderDef().color());

    // Create a batch from the loaded model and shader
    //mModelBatch = gl::Batch::create(model, shader);
}

void GuitarModel::draw() {
    ci::gl::clear();

    // Draw the model batch
    ci::gl::pushModelMatrix();
    mModelBatch->draw();
    ci::gl::popModelMatrix();
}
