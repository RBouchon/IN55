#ifndef CAMERA_H
#define CAMERA_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QMatrix4x4>
#include <QQuaternion>
#include <QVector2D>
#include <QBasicTimer>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>

class Camera
{
public:
    Camera();
    ~Camera();
    void orienter(int xRel, int YRel);
    void avancer();
    void reculer();
    QVector3D getCam_position();
    QVector3D getCam_pointcible();
    QVector3D getCamUpVector();
    void changeFocusMireille();
    void getFocusOnMireille();
    void resetAngles();
private:
    QVector3D camPosition;
    QVector3D camTarget;
    QVector3D upVector;
    QVector3D PositionMireille = QVector3D(0,0,-5);
    float m_phi;
    float m_theta;
    QVector3D camOrientation;
    float sensibiliteRota;
    bool focusMireille;
};

#endif // CAMERA_H
