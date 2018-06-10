#ifndef ANIMATEDMODEL_H
#define ANIMATEDMODEL_H

#include <QVector>
#include <QString>
#include <QMatrix4x4>
#include "bone.h"
#include <assimp/scene.h>
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
    QVector<Bone*> getBones();
    QVector<Animation*> getAnimations();
    QVector<QMatrix4x4> getTransformationsAtTime(double time);


private:
    const aiScene* scene;

    void loadModelFromFile(QString fileName);
    void calculateBonesTransformations(double time, QVector<QMatrix4x4> &transformationList, QMatrix4x4 parentTransformation, aiNode* node);
    QString textureFileName;
    QVector<Vertex*> vertices;
    QVector<unsigned int> indices;
    QVector<Bone*> bones;
    QVector<Animation*> animations;
    QMatrix4x4 globalTransform;
    unsigned int rootBoneIndex;


};

#endif // ANIMATEDMODEL_H
