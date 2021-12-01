#include "Sphere.h"
#include "TriangleStrip.h"
#include "Disk.h"
#include "SphereShell.h"

#include <glm/glm.hpp>
#include "glm/gtx/transform.hpp"


Sphere::Sphere()
{
}

Sphere::Sphere(int param1, int param2):
    Sphere(param1, param2, glm::mat4(1.0f))
{
}

Sphere::Sphere(int param1, int param2, glm::mat4 transform) :
    m_param1(param1),
    m_param2(param2),
    m_triangleStrip(std::make_unique<TriangleStrip>() ),
    m_transform(glm::mat4(transform))
{

    build();


    /**
     * Initialize the OpenGLShapeProperties so that the shape is ready to be drawn.
     * Note that this does not actually draw the shape, that happens in ShapesScene.cpp
     */
    initializeOpenGLShapeProperties();
}

void Sphere::build() {

    if (m_param1 < 4) {
        m_param1 = 3;
    }

    if (m_param2 < 4) {
        m_param2 = 3;
    }


    SphereShell sphere =
            SphereShell(m_param1, m_param2, m_transform);

    m_triangleStrip->addTriangleStripTo(&m_vertexData, m_param1, m_param2, sphere);
}

float Sphere::intersects(glm::vec4 &p, glm::vec4 &d) {
    if (! (d.y == 0.0f)) {
        float t = ((-0.5f) - p.y ) / d.y;

        if (t > 0.0f) {
            if (d.y > 0 && d.x > 0) {
                float yPlaneHit_Z = p.z + (d.z * t);
                float yPlaneHit_X = p.x + (d.x * t);

                float poly = pow(yPlaneHit_Z, 2) + pow(yPlaneHit_X, 2);

                if ( 0.25f >=  poly ) {
                    return t;
                }
                return 0;
            }
        }
    }
    return 0;
}


Sphere::~Sphere()
{
}
