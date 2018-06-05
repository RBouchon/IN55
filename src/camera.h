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
    orienter(int xRel, int YRel);
private:
    float m_phi;
    float m_theta;
    QVector3D m_orientation;
    QVector3D m_axeVertical;
    QVector3D m_deplacementLateral;
    QVector3D m_position;
    QVector3D m_pointCible;
};

#endif // CAMERA_H
