
#ifndef mesh_h
#define mesh_h

#include "Object.h"
#include "math/Vector4.h"
#include "render/MeshManager.h"
#include "Submesh.h"

class Mesh: public Object {
    
private:
    void removeAllSubmeshes();
    
protected:
    MeshManager mesh;

    Matrix4 modelViewMatrix;
    Matrix4 normalMatrix;

    std::vector<Vector3> vertices;
    std::vector<Vector3> normals;
    std::vector<Vector2> texcoords;
    std::vector<Submesh*> submeshes;

    int primitiveMode;
    
    bool transparent;
    
    void addSubmesh(Submesh* submesh);
    std::vector<Submesh*> getSubmeshes();
    
    void updateDistanceToCamera();
    void sortTransparentSubmeshes();
    
    float distanceToCamera;
    
public:
    Mesh();
    virtual ~Mesh();

    void setColor(float red, float green, float blue, float alpha);

    void setTexture(std::string texture);
    void setTexture(std::string texture, int submeshIndex);

    int getPrimitiveMode();
    Vector4 getColor();
    std::vector<Vector3> getVertices();
    std::vector<Vector3> getNormals();
    std::vector<Vector2> getTexcoords();
    std::string getTexture();
    std::string getTexture(int submeshIndex);
    
    void setTexcoords(std::vector<Vector2> texcoords);
    void setTransparency(bool transparency);

    void setPrimitiveMode(int primitiveMode);
    void addVertex(Vector3 vertex);
    void addNormal(Vector3 normal);
    void addTexcoord(Vector2 texcoord);

    bool meshDraw();

    void transform(Matrix4* viewMatrix, Matrix4* viewProjectionMatrix, Vector3* cameraPosition);
    void update();

    bool load();
    bool draw();
    void destroy();

};

#endif /* mesh_h */
