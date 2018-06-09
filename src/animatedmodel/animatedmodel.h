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
    AnimatedModel(QString fileName);
    ~AnimatedModel();
    QVector<Vertex*> getVertices();
    QVector<unsigned int> getIndices();
    QString getTextureFileName();

private:
    void loadModelFromFile(QString fileName);

    QString textureFileName;
    QVector<Vertex*> vertices;
    QVector<unsigned int> indices;
    QVector<Bone*> bones;
    QVector<Animation> animations;

};

#endif // ANIMATEDMODEL_H
