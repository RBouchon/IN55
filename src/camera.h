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
    QVector3D getCam_position();
    QVector3D getCam_pointcible();
    QVector3D getCamUpVector();
    void zoom(int rel);
    void dezoom(int rel);
    void orienter(int xRel, int YRel);


private:

    float m_phi;
    float m_theta;
    const float sensibiliteRota=1.2;
    bool focusModel;
    int distance;
    QVector3D camOrientation;
    QVector3D camPosition;
    QVector3D camTarget;
    QVector3D upVector;
    const QVector3D PositionModel = QVector3D(0,0,0);
};

#endif // CAMERA_H
