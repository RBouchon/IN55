#include "animatedmodel.h"

AnimatedModel:: AnimatedModel(QVector<Vertex*> verticesList, QVector<unsigned int> indicesList, QString textureFileName, QVector<Bone*> bonesList, QVector<Animation> animationsList)
    : vertices(verticesList), indices(indicesList), texture(textureFileName), bones(bonesList), animations(animationsList)
{

}

AnimatedModel::~AnimatedModel(){

}

QVector<Vertex*> AnimatedModel::getVertices(){
    return vertices;
}

QVector<unsigned int> AnimatedModel::getIndices(){
    return indices;
}

QString AnimatedModel::getTextureFileName(){
    return texture;
}
