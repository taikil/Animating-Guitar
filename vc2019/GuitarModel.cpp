#include "GuitarModel.h"

GuitarModel::GuitarModel() {
	setup();
}

void GuitarModel::setup() {
	// Load .obj model
	std::string modelPath = "C:/Users/taikilpatrick/Documents/CSC473/Project/Guitar/assets/guitarModel.obj";
	std::string mtl = "C:/Users/taikilpatrick/Documents/CSC473/Project/Guitar/assets/guitar.mtl";

	ObjLoader loader(loadFile(modelPath), loadFile(mtl));
	TriMeshRef model = TriMesh::create(loader);
	//gl::GlslProgRef shader = gl::getStockShader(gl::ShaderDef().color());
	auto lambert = gl::ShaderDef().lambert().color();
	gl::GlslProgRef shader = gl::getStockShader(lambert);
	mModelBatch = gl::Batch::create(*model, shader);
	//Frets
	auto capsule = geom::Capsule().subdivisionsAxis(10)
		.subdivisionsHeight(10).radius(0.1);
	fret = gl::Batch::create(capsule, shader);
}

void GuitarModel::draw() {
	// Draw the model batch
	gl::pushModelMatrix();
	{
		gl::pushModelMatrix();
		{
			gl::scale(3, 3, 3);
			gl::translate(0, -0.5, 0);
			mModelBatch->draw();
		}
		gl::popModelMatrix();
		gl::pushModelMatrix();
		{
			gl::translate(0.02, 1, 0.13);
			gl::rotate(angleAxis(float(M_PI / 2), vec3(0, 0, 1)));
			gl::scale(0.1, 0.1, 0.1);
			fret->draw();
		}
		gl::popModelMatrix();
	}
	gl::popModelMatrix();
}
