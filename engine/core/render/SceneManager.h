
#ifndef scenemanager_h
#define scenemanager_h

#include "Light.h"
#include <vector>
#include "render/SceneRender.h"

class SceneManager {
private:
    SceneRender* scene;
    
    std::vector<Light*> lights;
    Vector3 ambientLight;

    bool childScene;
    bool useDepth;
    bool useTransparency;

    void instanciateRender();
    void updateLights();
    void updateAmbientLight();

public:
    SceneManager();
    virtual ~SceneManager();

    bool isChildScene();
    void setChildScene(bool childScene);

    void setUseDepth(bool useDepth);
    void setUseTransparency(bool useTransparency);

    void setLights(std::vector<Light*> lights);
    void setAmbientLight(Vector3 ambientLight);
    
    SceneRender* getSceneRender();

    bool load();
    bool draw();
    bool viewSize(int x, int y, int width, int height);
};

#endif /* scenemanager_h */
