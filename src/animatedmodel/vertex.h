#ifndef VERTEX_H
#define VERTEX_H

#include <QVector3D>
#include <QVector2D>
#include <QVector>
#include "bone.h"


class Vertex
{
public:
    Vertex(QVector3D vertexPosition, QVector2D textureCoordinate, QVector3D vertexNormal, QVector<Bone*> bonesList, QVector<float> bonesWeightList);
    ~Vertex();
    QVector3D getPosition();
    QVector2D getTextureCoords();
    QVector<Bone*> getBones();
    QVector<float> getBonesWeight();
private:
    QVector3D position;
    QVector2D textureCoords;
    QVector3D normal;
    QVector<Bone*> bones;
    QVector<float> bonesWeight;

};

#endif // VERTEX_H
