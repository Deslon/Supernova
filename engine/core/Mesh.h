
#ifndef mesh_h
#define mesh_h

//
// (c) 2018 Eduardo Doria.
//

#include "ConcreteObject.h"
#include "math/Vector4.h"
#include "math/Vector3.h"
#include "math/Vector2.h"
#include "render/ObjectRender.h"
#include "util/AttributeBuffer.h"
#include "SubMesh.h"
#include <array>

namespace Supernova {

    class Mesh: public ConcreteObject {
        
    private:
        void removeAllSubMeshes();
        
    protected:
        ObjectRender* render;
        ObjectRender* shadowRender;

        std::vector<AttributeBuffer> buffers;

        std::vector<Vector3> vertices;
        std::vector<Vector3> normals;
        std::vector<Vector2> texcoords;
        std::vector<SubMesh*> submeshes;

        std::vector<std::array<float, 4>> boneWeights;
        std::vector<std::array<float, 4>> boneIds; //TODO: Change to int when supported

        std::vector<Matrix4> bonesMatrix;
        
        bool skymesh;
        bool textmesh;
        bool skinning;
        bool dynamic;

        int primitiveType;
        
        void addSubMesh(SubMesh* submesh);
        
        void sortTransparentSubMeshes();

        void updateBuffers();
        void updateVertices();
        void updateNormals();
        void updateTexcoords();
        void updateIndices();
        
    public:
        Mesh();
        virtual ~Mesh();

        int getPrimitiveType();
        std::vector<Vector3> getVertices();
        std::vector<Vector3> getNormals();
        std::vector<Vector2> getTexcoords();
        std::vector<SubMesh*> getSubMeshes();
        bool isSky();
        bool isText();
        bool isDynamic();
        
        void setTexcoords(std::vector<Vector2> texcoords);

        void setPrimitiveType(int primitiveType);
        void addVertex(Vector3 vertex);
        void addNormal(Vector3 normal);
        void addTexcoord(Vector2 texcoord);

        virtual void updateVPMatrix(Matrix4* viewMatrix, Matrix4* projectionMatrix, Matrix4* viewProjectionMatrix, Vector3* cameraPosition);
        virtual void updateMatrix();
        
        virtual bool textureLoad();
        virtual bool shadowLoad();
        virtual bool shadowDraw();
        
        virtual bool renderDraw();
        
        virtual bool load();
        virtual void destroy();

    };
    
}

#endif /* mesh_h */
