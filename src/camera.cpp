#include "camera.h"
#include "iostream"

/**
 * @brief Camera::Camera
 *
 * Constructor of Camera
 *
 * @param /
 * @return /
 */
Camera::Camera(){
    m_phi =0;
    m_theta =0;
    distance = 200;
    camPosition = QVector3D(0, 0, distance);
    camTarget = PositionModel;
    upVector = QVector3D(0.0,1.0,0.0);
}

/**
 * @brief Camera::Camera
 *
 * Destructor of Camera
 *
 * @param /
 * @return /
 */
Camera::~Camera(){

}


/**
 * @brief Camera::zoom
 *
 * Allow the camera to forward/advance toward the 3DModel
 *
 * @param rel
 * @return void
 */
void Camera::zoom( int rel){
    distance -= rel;
    camPosition = camPosition.normalized() * distance;
}

/**
 * @brief Camera::dezoom
 *
 * Is the opposite of zoom
 *
 * @param rel
 * @return void
 */
void Camera::dezoom(int rel){
    distance += rel;
    camPosition = camPosition.normalized() * distance;
}


/**
 * @brief Camera::dezoom
 *
 * Allow the camera to rotate around the 3D model like a sphere movement
 *
 * @param xRel
 * @param yRel
 * @return void
 */
void Camera::orienter(int xRel, int yRel){

        m_phi += -xRel * sensibiliteRota;

        m_theta += yRel * sensibiliteRota;

    // Conversion des angles en radian

    float phiR = m_phi * M_PI / 180;
    float thetaR = m_theta * M_PI / 180;



    camPosition.setX(distance*sin(phiR) * cos(thetaR));
    camPosition.setY(distance*sin(thetaR));
    camPosition.setZ(distance* cos(phiR) * cos(thetaR));


}

/**
 * @brief Camera::getCam_position
 *
 * Get fonction to set the first ViewMatrix's column in a lookAt
 *
 * @param /
 * @return QVector3D
 */
QVector3D Camera::getCam_position(){
    return camPosition;
}

/**
 * @brief Camera::getCam_pointcible
 *
 * Get fonction to set the second ViewMatrix's column in a lookAt
 *
 * @param /
 * @return QVector3D
 */
QVector3D Camera::getCam_pointcible(){
    return camTarget;
}

/**
 * @brief Camera::getCamUpVector
 *
 * Get fonction to set the last ViewMatrix's column in a lookAt
 *
 * @param /
 * @return QVector3D
 */
QVector3D Camera::getCamUpVector(){
    return upVector;
}


