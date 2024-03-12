#include "GuitarModel.h"

GuitarModel::GuitarModel() {
	setup();
}

void GuitarModel::setup() {
    // Load .obj model
    std::string modelPath = "C:/Users/taikilpatrick/Documents/CSC473/Project/Guitar/assets/guitarModel.obj";
    std::string mtl= "C:/Users/taikilpatrick/Documents/CSC473/Project/Guitar/assets/guitar.mtl";
    //ObjLoader loader(loadFile(modelPath));
    ObjLoader loader(loadFile(modelPath), loadFile(mtl));
    ci::TriMeshRef model = TriMesh::create(loader);
    ci::gl::GlslProgRef shader = ci::gl::getStockShader(ci::gl::ShaderDef().color());

    mModelBatch = gl::Batch::create(*model, shader);
}

void GuitarModel::draw() {
    // Draw the model batch
    ci::gl::pushModelMatrix();
	gl::scale(3, 3, 3);
    mModelBatch->draw();
    ci::gl::popModelMatrix();
}
