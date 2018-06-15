#include "vertex.h"


/**
 * @brief Vertex::Vertex
 *
 * Constructor for a Vertex.
 *
 * @param vertexPosition
 * @param textureCoordinate
 * @param vertexNormal
 * @param bonesList
 * @param bonesWeightList
 */
Vertex::Vertex(QVector3D vertexPosition, QVector2D textureCoordinate, QVector3D vertexNormal, QVector<Bone *> bonesList, QVector<float> bonesWeightList)
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

QVector3D Vertex::getNormal(){
    return normal;
}

QVector<Bone*> &Vertex::getBones(){
    return bones;
}

QVector<float> &Vertex::getBonesWeight(){
    return bonesWeight;
}
