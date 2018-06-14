#include "camera.h"
#include "iostream"

Camera::Camera()
{
    m_phi =0;
    m_theta =0;
    camPosition = QVector3D(-55, 55, -210);
    camTarget = PositionMireille;
    upVector = QVector3D(0.0,1.0,0.0);

    focusMireille = false;

}

Camera::~Camera(){

}


void Camera::orienter(int xRel, int YRel){
    m_phi += -YRel * sensibiliteRota;
    m_theta += -xRel * sensibiliteRota;
    //qDebug() << newMousePosition;
    //qDebug() << mousePressPosition;
    //qDebug() << m_phi;
    //qDebug() << m_theta;


    // Limitation de l'angle phi

    if(m_phi > 89.0)
        m_phi = 89.0;

    else if(m_phi < -89.0)
        m_phi = -89.0;


    // Conversion des angles en radian

    float phiRadian = m_phi * M_PI / 180;
    float thetaRadian = m_theta * M_PI / 180;


    // Si l'axe vertical est l'axe X

    if(upVector.x() == 1.0)
    {
        // Calcul des coordonnées sphériques

        camOrientation.setX(sin(phiRadian));
        camOrientation.setY(cos(phiRadian) * cos(thetaRadian));
        camOrientation.setZ(cos(phiRadian) * sin(thetaRadian));
    }


    // Si c'est l'axe Y

    else if(upVector.y() == 1.0)
    {
        // Calcul des coordonnées sphériques

        camOrientation.setX(cos(phiRadian) * sin(thetaRadian));
        camOrientation.setY(sin(phiRadian));
        camOrientation.setZ(cos(phiRadian) * cos(thetaRadian));
    }


    // Sinon c'est l'axe Z

    else
    {
        // Calcul des coordonnées sphériques

        camOrientation.setX(cos(phiRadian) * cos(thetaRadian));
        camOrientation.setY(cos(phiRadian) * sin(thetaRadian));
        camOrientation.setZ(sin(phiRadian));
    }
    qDebug() << camOrientation;


       camTarget = camPosition + camOrientation;




}


void Camera::avancer(){

if(camOrientation.isNull()){
    float phiRadian = m_phi * M_PI / 180;
    float thetaRadian = m_theta * M_PI / 180;
    camOrientation.setX(cos(phiRadian) * sin(thetaRadian));
    camOrientation.setY(sin(phiRadian));
    camOrientation.setZ(cos(phiRadian) * cos(thetaRadian));
}

    camPosition = camPosition + camOrientation * 4;

       camTarget = camPosition + camOrientation;


}
void Camera::reculer(){
    if(camOrientation.isNull()){
        float phiRadian = m_phi * M_PI / 180;
        float thetaRadian = m_theta * M_PI / 180;
        camOrientation.setX(cos(phiRadian) * sin(thetaRadian));
        camOrientation.setY(sin(phiRadian));
        camOrientation.setZ(cos(phiRadian) * cos(thetaRadian));
    }

    camPosition = camPosition - camOrientation * 4;

       camTarget = camPosition + camOrientation;


}

QVector3D Camera::getCam_position(){
    return camPosition;
}
QVector3D Camera::getCam_pointcible(){
    return camTarget;
}
QVector3D Camera::getCamUpVector(){
    return upVector;
}

void Camera::getFocusOnMireille(){
    camTarget = PositionMireille;
    resetAngles();
    camOrientation = QVector3D(0.0,0.0,0.0);
}

void Camera::changeFocusMireille(){

    getFocusOnMireille();


}

void Camera::resetAngles(){
    m_phi = 0;
    m_theta = 0;
}
