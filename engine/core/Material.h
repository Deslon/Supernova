#ifndef Material_h
#define Material_h

#define S_TEXTURE_2D 1
#define S_TEXTURE_CUBE 2

#include <string>
#include "math/Vector4.h"
#include <vector>

class Material {
private:
    
    std::vector<std::string> textures;
    Vector4 color;
    int textureType;
    
public:
    Material();
    Material(const Material& s);
    virtual ~Material();
    
    bool transparent; //TODO: not public
    
    Material& operator = (const Material& s);
    
    void setTexture(std::string texture);
    void setColor(Vector4 color);
    void setTextureType(int textureType);
    void setTextureCube(std::string front, std::string back, std::string left, std::string right, std::string up, std::string down);
    
    std::vector<std::string> getTextures();
    Vector4* getColor();
    int getTextureType();
    
};

#endif /* Material_h */