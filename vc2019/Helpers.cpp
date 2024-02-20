#include "Helpers.h"

namespace Helpers {
    void applyShear(const glm::mat4& shearMatrix) {
    
    }
    void rotateFromBase(float angle, const glm::vec3 axis, const glm::vec3 base) {
        gl::translate(-base);
        gl::rotate(angleAxis(angle, axis));
        gl::translate(base);
    }
}
