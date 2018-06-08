#include "vertex.h"

Vertex::Vertex(QVector3D vertexPosition, QVector2D textureCoordinate, QVector3D vertexNormal, QVector<Bone*> bonesList, QVector<float> bonesWeightList)
    : position(vertexPosition), textureCoords(textureCoordinate), normal(vertexNormal), bones(bonesList), bonesWeight(bonesWeightList)
{

}

Vertex::~Vertex()
{

}

QVector3D Vertex::getPosition(){
    return position;
}

QVector2D Vertex::getTextureCoords(){
    return textureCoords;
}

QVector<Bone*> Vertex::getBones(){
    return bones;
}

QVector<float> Vertex::getBonesWeight(){
    return bonesWeight;
}
