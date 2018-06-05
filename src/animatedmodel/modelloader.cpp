#include "modelloader.h"

ModelLoader::ModelLoader()
{
    {
    public:
        ModelLoader();
        bool Load(QString pathToFile);
        void getBufferData(QVector<float> **vertices, QVector<float> **normals,
                            QVector<unsigned int> **indices);

        QSharedPointer<Node> getNodeData() { return m_rootNode; }
        QSharedPointer<MaterialInfo> processMaterial(aiMaterial *mater);
        QSharedPointer<Mesh> processMesh(aiMesh *mesh);
        void processNode(const aiScene *scene, aiNode *node, Node *parentNode, Node &newNode);

        void transformToUnitCoordinates();
        void findObjectDimensions(Node *node, QMatrix4x4 transformation, QVector3D &minDimension, QVector3D &maxDimension);

        QVector<float> m_vertices;
        QVector<float> m_normals;
        QVector<unsigned int> m_indices;

        QVector<QSharedPointer<MaterialInfo> > m_materials;
        QVector<QSharedPointer<Mesh> > m_meshes;
        QSharedPointer<Node> m_rootNode;
}
