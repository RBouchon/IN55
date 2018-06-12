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
private:
    QVector3D camPosition;
    QVector3D camTarget;
    QVector3D upVector;

    float m_phi;
    float m_theta;
    QVector3D camOrientation;

    float sensibiliteRota;
};

#endif // CAMERA_H
