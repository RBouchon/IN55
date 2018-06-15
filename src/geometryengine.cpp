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

#include "geometryengine.h"
#include "animatedmodel/vertex.h"

#include <QVector2D>
#include <QVector3D>
#include <QVector>
#include <iostream>


struct VertexData
{
    QVector3D vertexPosition;
    QVector2D texCoord;
    QVector4D bonesIndex;  // Index of the bones
    QVector4D weight;  // Weight of the bones
};


//! [0]
GeometryEngine::GeometryEngine(AnimatedModel animatedModel)
    : indexBuf(QOpenGLBuffer::IndexBuffer), model(animatedModel)
{

    initializeOpenGLFunctions();

    // Generate 2 VBOs
    arrayBuf.create();
    indexBuf.create();

    // Initializes cube geometry and transfers it to VBOs
    initGeometry();

}

GeometryEngine::~GeometryEngine()
{
    arrayBuf.destroy();
    indexBuf.destroy();
}
//! [0]

void GeometryEngine::initGeometry()
{

    //Vertexs and Indices initialisations from QVector
    nbrVertices = model.getVertices().size();


    VertexData *vertices = new VertexData[nbrVertices];
    QVector<Vertex*> modelVertices = model.getVertices();

    for(unsigned int i = 0; i<nbrVertices; ++i){

        // Get the index for each bones influencing the vertex
        QVector4D bonesIndex = QVector4D(0,0,0,0);
        QVector4D weight = QVector4D(0,0,0,0);
        for(int j = 0; j<modelVertices[i]->getBones().size(); ++j ){
            for(int k = 0; k<model.getBones().size(); ++k){
                if(modelVertices[i]->getBones()[j]->getName() == model.getBones()[k]->getName() ){
                    bonesIndex[j] = k;
                }
            }
            weight[j] = modelVertices[i]->getBonesWeight()[j];

        }


        vertices[i] = {modelVertices[i]->getPosition(), modelVertices[i]->getTextureCoords(), bonesIndex, weight };

    }


    nbrIndices = model.getIndices().size();
    QVector<unsigned int> indicesList = model.getIndices();
    unsigned int *indices = &indicesList[0];



////! [1]
    // Transfer vertex data to VBO 0
    arrayBuf.bind();
    arrayBuf.allocate(vertices, nbrVertices * sizeof(VertexData));

    // Transfer index data to VBO 1
    indexBuf.bind();
    indexBuf.allocate(indices, nbrIndices * sizeof(unsigned int));

//! [1]

}

//! [2]
void GeometryEngine::drawGeometry(QOpenGLShaderProgram *program)
{




    // Tell OpenGL which VBOs to use
    arrayBuf.bind();
    indexBuf.bind();


    // Offset for position
    quintptr offset = 0;

    // Tell OpenGL programmable pipeline how to locate vertex position data
    int vertexLocation = program->attributeLocation("vertexPosition");
    program->enableAttributeArray(vertexLocation);
    program->setAttributeBuffer(vertexLocation, GL_FLOAT, offset, 3, sizeof(VertexData));

    // Offset for texture coordinate
    offset += sizeof(QVector3D);

    // Tell OpenGL programmable pipeline how to locate vertex texture coordinate data
    int texLocation = program->attributeLocation("texCoord");
    program->enableAttributeArray(texLocation);
    program->setAttributeBuffer(texLocation, GL_FLOAT, offset, 2, sizeof(VertexData));

    // Offset for bonesIndex
    offset += sizeof(QVector2D);

    // Tell OpenGL programmable pipeline how to locate bonesIndex data
    int bonesInd = program->attributeLocation("bonesIndex");
    program->enableAttributeArray(bonesInd);
    program->setAttributeBuffer(bonesInd, GL_FLOAT, offset, 4, sizeof(VertexData));

    // Offset for weight
    offset += sizeof(QVector4D);

    // Tell OpenGL programmable pipeline how to locate weight data
    int weightInd = program->attributeLocation("weight");
    program->enableAttributeArray(weightInd);
    program->setAttributeBuffer(weightInd, GL_FLOAT, offset, 4, sizeof(VertexData));


    // Draw cube geometry using indices from VBO 1
    glDrawElements(GL_TRIANGLES, nbrIndices, GL_UNSIGNED_INT, 0);
}
//! [2]
