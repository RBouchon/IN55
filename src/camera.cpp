#include "camera.h"
#include "iostream"

Camera::Camera()
{
    m_phi =0;
    m_theta =0;
    distance = 200;
    camPosition = QVector3D(0, 0, distance);
    camTarget = PositionMireille;
    upVector = QVector3D(0.0,1.0,0.0);

    focusMireille = false;

}

Camera::~Camera(){

}

void Camera::orienterDroite(int rel){
    m_theta += rel;

    float thetaRadian = m_theta * M_PI / 180;
    float phiRadian = m_phi * M_PI / 180;

    camPosition.setX(distance*sin(thetaRadian) * cos(phiRadian));
    camPosition.setY(distance*sin(phiRadian));
    camPosition.setZ(distance* cos(phiRadian) * cos(thetaRadian));
}

void Camera::orienterGauche(int rel){
    m_theta -= rel;

    float thetaRadian = m_theta * M_PI / 180;
    float phiRadian = m_phi * M_PI / 180;

    camPosition.setX(distance*sin(thetaRadian) * cos(phiRadian));
    camPosition.setY(distance*sin(phiRadian));
    camPosition.setZ(distance* cos(phiRadian) * cos(thetaRadian));
}

void Camera::orienterHaut(int rel){
    m_phi += rel;

    float thetaRadian = m_theta * M_PI / 180;
    float phiRadian = m_phi * M_PI / 180;

    camPosition.setX(distance*sin(thetaRadian) * cos(phiRadian));
    camPosition.setY(distance*sin(phiRadian));
    camPosition.setZ(distance* cos(phiRadian) * cos(thetaRadian));
}

void Camera::orienterBas(int rel){
    m_phi -= rel;

    float thetaRadian = m_theta * M_PI / 180;
    float phiRadian = m_phi * M_PI / 180;

    camPosition.setX(distance*sin(thetaRadian) * cos(phiRadian));
    camPosition.setY(distance*sin(phiRadian));
    camPosition.setZ(distance* cos(phiRadian) * cos(thetaRadian));
}

void Camera::zoom( int rel){
    distance -= rel;
    camPosition = camPosition.normalized() * distance;
}

void Camera::dezoom(int rel){
    distance += rel;
    camPosition = camPosition.normalized() * distance;
}


void Camera::orienter(int xRel, int yRel){
    qDebug() << xRel;
    qDebug() << yRel;

        m_phi += -xRel * 1.2;


        m_theta += yRel * 1.2;



    //qDebug() << newMousePosition;
    //qDebug() << mousePressPosition;
    //qDebug() << m_phi;
    //qDebug() << m_theta;








    // Conversion des angles en radian

    float phiRadian = m_phi * M_PI / 180;
    float thetaRadian = m_theta * M_PI / 180;



    camPosition.setX(distance*sin(phiRadian) * cos(thetaRadian));
    camPosition.setY(distance*sin(thetaRadian));
    camPosition.setZ(distance* cos(phiRadian) * cos(thetaRadian));

    /*
    if(camOrientation.isNull()){
        camOrientation = camTarget - camPosition;
        camOrientation = camOrientation.normalized();
        qDebug() << camOrientation;
    }
    else{

    }

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


*/

}


void Camera::avancer(){

if(camOrientation.isNull()){
    camOrientation = camTarget - camPosition;
    camOrientation = camOrientation.normalized();
    qDebug() << camOrientation;
}

    camPosition = camPosition + camOrientation * 4;

    camTarget = camPosition + camOrientation;


}
void Camera::reculer(){
    if(camOrientation.isNull()){
        camOrientation = camTarget - camPosition;
        camOrientation = camOrientation.normalized();
        qDebug() << camOrientation;
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
