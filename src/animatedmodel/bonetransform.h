#ifndef BONETRANSFORM_H
#define BONETRANSFORM_H

#include <QVector3D>
#include <QQuaternion>

class BoneTransform
{
public:
    BoneTransform();
private:
    QVector3D position;
    QQuaternion rotation;
};

#endif // BONETRANSFORM_H
