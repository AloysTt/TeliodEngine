#include <glm/glm.hpp>
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/quaternion_float.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/euler_angles.hpp"

int main(int argc, char **argv)
{
	glm::quat qu(1.0f, 0.0f, 0.0f, 0.0f);
    float x, y, z;
    glm::extractEulerAngleXYZ(mat4_cast(qu), x, y, z);
    return 0;
}