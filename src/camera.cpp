#include "camera.h"

Camera::Camera()
{

}

Camera::~Camera(){

}

void Camera::orienter(int xRel, int yRel)
{
    // Modification des angles
    m_phi += -yRel * 0.5;
    m_theta += -xRel * 0.5;

    // Limitation de l'angle phi

    if(m_phi > 89.0)
        m_phi = 89.0;

    else if(m_phi < -89.0)
        m_phi = -89.0;

    // Calcul des coordonnées sphériques

    m_orientation.x = cos(m_phi) * sin(m_theta);
    m_orientation.y = sin(m_phi);
    m_orientation.z = cos(m_phi) * cos(m_theta);

}

