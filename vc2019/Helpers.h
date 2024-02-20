#pragma once

#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"


using namespace ci;

namespace Helpers
{
	void rotateFromBase(float angle, const glm::vec3 axis, const glm::vec3 distance);
	void applyShear(const glm::mat4& shearMatrix);
}

