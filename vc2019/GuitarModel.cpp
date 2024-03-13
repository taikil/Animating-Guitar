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
		.subdivisionsHeight(10).radius(0.1).length(2.1);
	auto scale = geom::Scale(0.1, 0.1, 0.1);
	auto rot = geom::Rotate(quat(vec3(0, 0, float(M_PI / 2))));
	fret = gl::Batch::create(capsule >> rot >> scale, shader);
}

void GuitarModel::draw() {
	// Draw the model batch
	gl::pushModelMatrix();
	{
		gl::pushModelMatrix();
		{
			gl::scale(6, 6, 6);
			gl::translate(0, -0.5, 0);
			mModelBatch->draw();
		}
		gl::popModelMatrix();
		gl::pushModelMatrix();
		{
			gl::color(0.886, 0.922, 0.914);
			gl::translate(0.04, 2.0, 0.27);
			fret->draw();
			float zTrans = 0; // depth of fretboard gets closer
			float spacing = -0.12; // Distance between frets
			for (int i = 0; i < 20; i++) {
				gl::pushModelMatrix();
				gl::translate(0, spacing, zTrans);
				gl::scale(1.024, 1, 1);
				fret->draw();
				zTrans += 0.00001;
				spacing += 0.0002;
			}
			popN(20);
		}
		gl::popModelMatrix();
	}
	gl::popModelMatrix();
}

void GuitarModel::popN(int n) {
	for (int i = 0; i < n; i++) {
		gl::popModelMatrix();
	}
}
