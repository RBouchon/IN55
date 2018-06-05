#ifndef VERTEX_H
#define VERTEX_H

#include <QVector3D>
#include <QVector2D>
#include "bone.h"


class Vertex
{
public:
    Vertex();

private:
    QVector3D position;
    QVector2D textureCoords;
    QVector3D normal;
    QList<Bone> bonesList;
    QList<float> bonesWeight;

};

#endif // VERTEX_H
