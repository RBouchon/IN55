/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtCore module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "mainwidget.h"
#include "camera.h"
#include "iostream"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <QMatrix4x4>
#include <QString>


#include "animatedmodel/animatedmodel.h"
#include "animatedmodel/vertex.h"
#include "animatedmodel/bone.h"
#include "animatedmodel/keyframe.h"

#include <QMouseEvent>

#include <math.h>

MainWidget::MainWidget(QWidget *parent) :
    QOpenGLWidget(parent),
    geometries(0),
    angularSpeed(0)
{
}

MainWidget::~MainWidget()
{
    // Make sure the context is current when deleting the texture
    // and the buffers.
    makeCurrent();
    delete geometries;
    doneCurrent();
}

//! [0]
void MainWidget::mousePressEvent(QMouseEvent *e)
{
    // Save mouse press position
    mousePressPosition = QVector2D(e->localPos());
}

void MainWidget::keyPressEvent(QKeyEvent *event){
    switch ( event->key() )
        {
            case Qt::Key_Left :
                //Démarrer annimation gauche
                break;
            case Qt::Key_Right:
                //Démarrer annimation droite
                break;
            case Qt::Key_Down:
                //Démarrer annimation bas
                break;
            case Qt::Key_Up:
                //Démarrer annimation saut
                break;

            case Qt::Key_Z:
                //Bouger cam en avant
                cam.avancer();
                break;
            case Qt::Key_S:
                //Bouger cam en arriere
                cam.reculer();
                break;
            default:
                break;
        }
}

void MainWidget::mouseMoveEvent(QMouseEvent *event){
    cam.orienter(1,1);
    view.lookAt(cam.getM_position(),cam.getM_pointcible(),cam.getAxeVertical());
    qInfo("je bouge");

}

void MainWidget::mouseReleaseEvent(QMouseEvent *e)
{
       /*
    // Mouse release position - mouse press position
    QVector2D diff = QVector2D(e->localPos()) - mousePressPosition;

    // Rotation axis is perpendicular to the mouse position difference
    // vector
    QVector3D n = QVector3D(diff.y(), diff.x(), 0.0).normalized();

    // Accelerate angular speed relative to the length of the mouse sweep
    qreal acc = diff.length() / 100.0;

    // Calculate new rotation axis as weighted sum
    rotationAxis = (rotationAxis * angularSpeed + n * acc).normalized();

    // Increase angular speed
    angularSpeed += acc;*/
}
//! [0]

//! [1]
void MainWidget::timerEvent(QTimerEvent *)
{
    // Decrease angular speed (friction)
    angularSpeed *= 0.99;

    // Stop rotation when speed goes below threshold
    if (angularSpeed < 0.01) {
        angularSpeed = 0.0;
    } else {
        // Update rotation
        rotation = QQuaternion::fromAxisAndAngle(rotationAxis, angularSpeed) * rotation;

        // Request an update
        update();
    }
}
//! [1]

void MainWidget::initializeGL()
{
    initializeOpenGLFunctions();

    glClearColor(0, 0, 0, 1);

    initShaders();

//! [2]
    // Enable depth buffer
    glEnable(GL_DEPTH_TEST);

    // Enable back face culling
    glEnable(GL_CULL_FACE);
//! [2]


    //Import 3DModel in the scene (should be in modelloader)
    Assimp::Importer importer;

    int meshID = 0;
    const aiScene* scene = importer.ReadFile("modeles/Character.dae",
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





    float dezoom = 7;

    //Get list of vertices
    QVector<Vertex*> vertices;
    QVector<unsigned int> facesIndices;

    for(unsigned int i = 0; i<meshes[meshID]->mNumVertices; ++i){

        //Initialize list of bones and weight for this vertex
        QVector<Bone*> vertexBones;
        QVector<float> weight;

        Vertex* newVertex = new Vertex(QVector3D(meshes[meshID]->mVertices[i].x/dezoom,meshes[meshID]->mVertices[i].y/dezoom, meshes[meshID]->mVertices[i].z/dezoom),
                                       QVector2D(meshes[meshID]->mTextureCoords[0][i].x,meshes[meshID]->mTextureCoords[0][i].y),
                                       QVector3D(meshes[meshID]->mNormals[i].x, meshes[meshID]->mNormals[i].y, meshes[meshID]->mNormals[i].z),
                                       vertexBones, weight);
        vertices.append(newVertex);



    }



    //Create list of bones
    QVector<Bone*> bones;

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
        bones.append(newBone);


        //Set the bones and weights on each vertices affected by this bone
        for(unsigned int k = 0; k<meshes[meshID]->mBones[j]->mNumWeights; ++k){
            vertices[meshes[meshID]->mBones[j]->mWeights[k].mVertexId]->getBones().append(newBone);
            vertices[meshes[meshID]->mBones[j]->mWeights[k].mVertexId]->getBonesWeight().append(meshes[meshID]->mBones[j]->mWeights[k].mWeight);
        }
    }




    //Get list of indices (faces with triangles primitive)
    for(unsigned int i = 0; i<meshes[meshID]->mNumFaces; ++i){

        facesIndices.append(meshes[meshID]->mFaces[i].mIndices[0]);
        facesIndices.append(meshes[meshID]->mFaces[i].mIndices[1]);
        facesIndices.append(meshes[meshID]->mFaces[i].mIndices[2]);

    }


    // Get texture filename
    QString textureFileName;
    aiString Path;
    if(scene->mMaterials[0]->GetTexture(aiTextureType_DIFFUSE, 0, &Path, NULL, NULL, NULL, NULL, NULL)== AI_SUCCESS){
        textureFileName = QString(Path.data);
    }else{
        std::cout << "Erreur lors de la récupération du nom du fichier de texture" << std::endl;
    }


    //Get Animations list
    QVector<Animation> animations;
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
              std::cout<<scene->mAnimations[i]->mChannels[i]->mPositionKeys[k].mTime<<std::endl;
            }

        }

    }

    geometries = new GeometryEngine(AnimatedModel(vertices, facesIndices, textureFileName, bones, animations));

    // Use QBasicTimer because its faster than QTimer
    timer.start(12, this);
}

//! [3]
void MainWidget::initShaders()
{
    // Compile vertex shader
    if (!program.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/vshader.glsl"))
        close();

    // Compile fragment shader
    if (!program.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/fshader.glsl"))
        close();

    // Link shader pipeline
    if (!program.link())
        close();

    // Bind shader pipeline for use
    if (!program.bind())
        close();
}
//! [3]


//! [5]
void MainWidget::resizeGL(int w, int h)
{
    // Calculate aspect ratio
    qreal aspect = qreal(w) / qreal(h ? h : 1);

    // Set near plane to 3.0, far plane to 7.0, field of view 45 degrees
    const qreal zNear = 3.0, zFar = 7.0, fov = 45.0;

    // Reset projection
    projection.setToIdentity();

    // Set perspective projection
    projection.perspective(fov, aspect, zNear, zFar);


}
//! [5]

void MainWidget::paintGL()
{
    // Clear color and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

//! [6]
    // Calculate model view transformation
    QMatrix4x4 matrix;
    matrix.translate(0.0, 0.0, -5.0);
    matrix.rotate(rotation);

    // Set modelview-projection matrix
    program.setUniformValue("mvp",view * projection * matrix);
//! [6]

    // Draw cube geometry
    geometries->drawGeometry(&program);
}
