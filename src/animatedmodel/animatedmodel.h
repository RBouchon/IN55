#ifndef ANIMATEDMODEL_H
#define ANIMATEDMODEL_H

#include <QVector>
#include <QString>
#include "bone.h"
#include "vertex.h"
#include "animation.h"

class AnimatedModel
{
public:
    AnimatedModel(QVector<Vertex*> verticesList, QVector<unsigned int> indicesList, QString textureFileName, QVector<Bone*> bonesList, QVector<Animation> animationsList);
    ~AnimatedModel();
    QVector<Vertex*> getVertices();
    QVector<unsigned int> getIndices();
    QString getTextureFileName();

private:
    QVector<Vertex*> vertices;
    QVector<unsigned int> indices;
    QString texture;
    QVector<Bone*> bones;
    QVector<Animation> animations;

};

#endif // ANIMATEDMODEL_H
