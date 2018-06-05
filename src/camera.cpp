#include "camera.h"

Camera::Camera()
{

}

Camera::~Camera(){

}

void Camera::orienter(int xRel, int yRel)
{
    // Récupération des angles

    m_phi += -yRel * 0.5;
    m_theta += -xRel * 0.5;


    // Limitation de l'angle phi

    if(m_phi > 89.0)
        m_phi = 89.0;

    else if(m_phi < -89.0)
        m_phi = -89.0;


    // Conversion des angles en radian

    float phiRadian = m_phi * 3.14 / 180;
    float thetaRadian = m_theta * 3.14 / 180;


    // Si l'axe vertical est l'axe X

    if(m_axeVertical.x() == 1.0)
    {
        // Calcul des coordonnées sphériques

        m_orientation.setX(sin(phiRadian));
        m_orientation.setY(cos(phiRadian) * cos(thetaRadian));
        m_orientation.setZ(cos(phiRadian) * sin(thetaRadian));
    }


    // Si c'est l'axe Y

    else if(m_axeVertical.y() == 1.0)
    {
        // Calcul des coordonnées sphériques

        m_orientation.setX(cos(phiRadian) * sin(thetaRadian));
        m_orientation.setY(sin(phiRadian));
        m_orientation.setZ(cos(phiRadian) * cos(thetaRadian));
    }


    // Sinon c'est l'axe Z

    else
    {
        // Calcul des coordonnées sphériques

        m_orientation.setX(cos(phiRadian) * cos(thetaRadian));
        m_orientation.setY(cos(phiRadian) * sin(thetaRadian));
        m_orientation.setZ(sin(phiRadian));
    }


    // Calcul de la normale

    m_deplacementLateral.normal(m_axeVertical, m_orientation);
    m_deplacementLateral.normalize();


    // Calcul du point ciblé pour OpenGL

    m_pointCible = m_position + m_orientation;
}

void Camera::avancer()
{
    // Avancée de la caméra

    //if(input.getTouche(SDL_SCANCODE_UP))
    //{
        m_position = m_position + m_orientation * 0.5f;
        m_pointCible = m_position + m_orientation;
    //}
}

void Camera::reculer()
{
    // reculement de la caméra

    //if(input.getTouche(SDL_SCANCODE_UP))
    //{
        m_position = m_position - m_orientation * 0.5f;
        m_pointCible = m_position + m_orientation;
    //}
}

QVector3D Camera::getM_position(){
    return m_position;
}

QVector3D Camera::getM_pointcible(){
    return m_pointCible;
}
QVector3D Camera::getAxeVertical(){
    return m_axeVertical;
}
