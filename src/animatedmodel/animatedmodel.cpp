#include "animatedmodel.h"
#include "iostream"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


#include <QMatrix4x4>
#include <QString>
#include <QOpenGLTexture>
#include <QFileInfo>


#include "vertex.h"
#include "bone.h"
#include "keyframe.h"


AnimatedModel:: AnimatedModel(QString fileName)
{

    loadModelFromFile(fileName);


}

AnimatedModel::~AnimatedModel(){

}


//Initialize model from file
void AnimatedModel::loadModelFromFile(QString fileName){

    Assimp::Importer importer;

    int meshID = 0;


    const aiScene* scene = importer.ReadFile(fileName.toStdString(),
    aiProcess_CalcTangentSpace |
    aiProcess_Triangulate |
    aiProcess_JoinIdenticalVertices |
    aiProcess_SortByPType);

    // If the import failed, report it
    if( !scene)
    {
        std::cout << importer.GetErrorString() << std::endl;
    }

    //Get model informations from the scene
    aiMesh **meshes = scene->mMeshes;


    float dezoom = 5;


    //Get list of vertices
    QVector<Vertex*> verticesList;
    QVector<unsigned int> indicesList;

    for(unsigned int i = 0; i<meshes[meshID]->mNumVertices; ++i){

        //Initialize list of bones and weight for this vertex
        QVector<Bone*> vertexBones;
        QVector<float> weight;

        Vertex* newVertex = new Vertex(QVector3D(meshes[meshID]->mVertices[i].x/dezoom,meshes[meshID]->mVertices[i].y/dezoom, meshes[meshID]->mVertices[i].z/dezoom),
                                       QVector2D(meshes[meshID]->mTextureCoords[0][i].x,meshes[meshID]->mTextureCoords[0][i].y),
                                       QVector3D(meshes[meshID]->mNormals[i].x, meshes[meshID]->mNormals[i].y, meshes[meshID]->mNormals[i].z),
                                       vertexBones, weight);
        verticesList.append(newVertex);


    }



    //Create list of bones
    QVector<Bone*> bonesList;

    for(unsigned int j = 0; j<meshes[meshID]->mNumBones; ++j){ //for each bone, create a list of boneschilds

        QVector<QString> bonesChilds;
        for(unsigned int k = 0; k<scene->mRootNode->FindNode(meshes[meshID]->mBones[j]->mName)->mNumChildren; ++k){ //Get the name of each childrens
            bonesChilds.append(QString(scene->mRootNode->FindNode(meshes[meshID]->mBones[j]->mName)->mChildren[k]->mName.data));

        }
        aiMatrix4x4 offsetMatrix = meshes[meshID]->mBones[j]->mOffsetMatrix;
        aiMatrix4x4 transformMatrix = scene->mRootNode->FindNode(meshes[meshID]->mBones[j]->mName)->mTransformation;

        Bone* newBone = new Bone(QString(meshes[meshID]->mBones[j]->mName.data), bonesChilds,
                                 QMatrix4x4(transformMatrix.a1, transformMatrix.a2, transformMatrix.a3, transformMatrix.a4,
                                            transformMatrix.b1, transformMatrix.b2, transformMatrix.b3, transformMatrix.b4,
                                            transformMatrix.c1, transformMatrix.c2, transformMatrix.c3, transformMatrix.c4,
                                            transformMatrix.d1, transformMatrix.d2, transformMatrix.d3, transformMatrix.d4),
                                 QMatrix4x4(offsetMatrix.a1, offsetMatrix.a2, offsetMatrix.a3, offsetMatrix.a4,
                                            offsetMatrix.b1, offsetMatrix.b2, offsetMatrix.b3, offsetMatrix.b4,
                                            offsetMatrix.c1, offsetMatrix.c2, offsetMatrix.c3, offsetMatrix.c4,
                                            offsetMatrix.d1, offsetMatrix.d2, offsetMatrix.d3, offsetMatrix.d4));
        bonesList.append(newBone);


        //Set the bones and weights on each vertices affected by this bone
        for(unsigned int k = 0; k<meshes[meshID]->mBones[j]->mNumWeights; ++k){
            verticesList[meshes[meshID]->mBones[j]->mWeights[k].mVertexId]->getBones().append(newBone);
            verticesList[meshes[meshID]->mBones[j]->mWeights[k].mVertexId]->getBonesWeight().append(meshes[meshID]->mBones[j]->mWeights[k].mWeight);
        }
    }




    //Get list of indices (faces with triangles primitive)
    for(unsigned int i = 0; i<meshes[meshID]->mNumFaces; ++i){

        indicesList.append(meshes[meshID]->mFaces[i].mIndices[0]);
        indicesList.append(meshes[meshID]->mFaces[i].mIndices[1]);
        indicesList.append(meshes[meshID]->mFaces[i].mIndices[2]);

    }


    // Get texture filename

    QString textureName;
    aiString Path;
    if(scene->mMaterials[0]->GetTexture(aiTextureType_DIFFUSE, 0, &Path, NULL, NULL, NULL, NULL, NULL)== AI_SUCCESS){
        textureName = QFileInfo(fileName).path() + '/' + QString(Path.data);

    }else{
        std::cout << "Erreur lors de la récupération du nom du fichier de texture" << std::endl;
    }



    //Get Animations list
    QVector<Animation> animationsList;
    /*
    for(unsigned int i = 0; i<scene->mNumAnimations; ++i){ // For each animation
        for(unsigned int j = 0; j <scene->mAnimations[i]->mNumChannels; ++j){ // For each bones transformation of the animation, get the transformations
            Bone* transformedBone;
            for(int k = 0; k<bones.size(); ++k){ // Get the bone from the list of bones
                if(bones[k]->getName() == QString(scene->mAnimations[i]->mChannels[j]->mNodeName.data)){
                    transformedBone = bones[k];
                }
            }

            QVector<QVector3D> positionTransformationList;
            QVector<QQuaternion> rotationTransformationList;
            QVector<QVector3D> scalingTransformationList;




            for(unsigned int k = 0; k <scene->mAnimations[i]->mChannels[i]->mNumPositionKeys; ++k){ //Get the position transformations of this bone
              //std::cout<<scene->mAnimations[i]->mChannels[i]->mPositionKeys[k].mTime<<std::endl;
            }

        }

    }
*/


    vertices = verticesList;
    indices = indicesList;
    bones = bonesList;
    textureFileName = textureName;
    animations = animationsList;

}

QVector<Vertex*> AnimatedModel::getVertices(){
    return vertices;
}

QVector<unsigned int> AnimatedModel::getIndices(){
    return indices;
}

QString AnimatedModel::getTextureFileName(){
    return textureFileName;
}
