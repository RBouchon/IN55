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
   void orienterDroite(int rel);
   void orienterGauche(int rel);
   void orienterHaut(int rel);
   void orienterBas(int rel);


private:
    QVector3D camPosition;
    QVector3D camTarget;
    QVector3D upVector;
    QVector3D PositionMireille = QVector3D(0,0,0);
    float m_phi;
    float m_theta;
    QVector3D camOrientation;
    const float sensibiliteRota=0.001;;
    bool focusMireille;
    int distance;
};

#endif // CAMERA_H
