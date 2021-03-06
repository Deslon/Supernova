#include "Object.h"
#include "Log.h"
#include "ui/UIObject.h"
#include "Light.h"
#include "Scene.h"
#include "Engine.h"
#include "physics/PhysicsWorld2D.h"

//
// (c) 2018 Eduardo Doria.
//

using namespace Supernova;

Object::Object(){
    loaded = false;
    markToUpdate = true;
    
    parent = NULL;
    scene = NULL;
    body = NULL;

    ownedBody = true;
    allowBodyUpdate = true;

    viewMatrix = NULL;
    projectionMatrix = NULL;
    viewProjectionMatrix = NULL;
    cameraPosition = Vector3(0,0,0);

    scale = Vector3(1,1,1);
    position = Vector3(0,0,0);
    rotation = Quaternion(1,0,0,0);
    center = Vector3(0,0,0);
}

Object::~Object(){
    
    if (parent)
        parent->removeObject(this);

    if (body) {
        if (ownedBody)
            delete body;
        else
            body->attachedObject = NULL;
    }
    
    destroy();
}

void Object::setSceneAndConfigure(Scene* scene){
    if (this->scene == NULL){
        this->scene = scene;
        if (loaded)
            reload();
    }
    
    std::vector<Object*>::iterator it;
    for (it = objects.begin(); it != objects.end(); ++it) {
        (*it)->setSceneAndConfigure(scene);
    }

    //This is for not add object in scene and subscenes
    if (this->scene == scene) {
        if (Light *light_ptr = dynamic_cast<Light *>(this)) {
            scene->addLight(light_ptr);
        }

        if (UIObject *guiobject_ptr = dynamic_cast<UIObject *>(this)) {
            scene->addGUIObject(guiobject_ptr);
        }

        if (SkyBox *sky_ptr = dynamic_cast<SkyBox *>(this)) {
            scene->setSky(sky_ptr);
        }
    }

    if (Scene *scene_ptr = dynamic_cast<Scene *>(this)) {
        scene->addSubScene(scene_ptr);
    }

}

void Object::removeScene(){
    this->scene = NULL;
    
    std::vector<Object*>::iterator it;
    for (it = objects.begin(); it != objects.end(); ++it) {
        (*it)->removeScene();
    }
}

void Object::addObject(Object* obj){
    if (obj != NULL) {
        if (Scene *scene_ptr = dynamic_cast<Scene *>(obj)) {
            scene_ptr->childScene = true;
        }

        if (obj->parent == NULL) {
            objects.push_back(obj);

            obj->parent = this;

            obj->viewMatrix = viewMatrix;
            obj->projectionMatrix = projectionMatrix;
            obj->viewProjectionMatrix = viewProjectionMatrix;
            obj->cameraPosition = cameraPosition;
            obj->modelViewProjectionMatrix = modelViewProjectionMatrix;

            if (scene != NULL) {
                obj->setSceneAndConfigure(scene);

                if (body && scene->physicsWorld){
                    scene->getPhysicsWorld()->addBody(body);
                }
            }

            obj->needUpdate();

            if (loaded)
                obj->load();
        } else {
            Log::Error("Object has a parent already");
        }
    }else{
        Log::Error("Can not add a NULL object");
    }
}

void Object::removeObject(Object* obj){
    
    if (scene != NULL){
        
        if (Light* light_ptr = dynamic_cast<Light*>(this)){
            ((Scene*)scene)->removeLight(light_ptr);
        }
        
        if (Scene* scene_ptr = dynamic_cast<Scene*>(this)){
            ((Scene*)scene)->removeSubScene(scene_ptr);
        }
        
        if (UIObject* guiobject_ptr = dynamic_cast<UIObject*>(this)){
            ((Scene*)scene)->removeGUIObject(guiobject_ptr);
        }

        if (body && scene->physicsWorld){
            scene->getPhysicsWorld()->removeBody(body);
        }
    }
    
    if (Scene* scene_ptr = dynamic_cast<Scene*>(obj)){
        scene_ptr->childScene = false;
    }
    
    std::vector<Object*>::iterator i = std::remove(objects.begin(), objects.end(), obj);
    objects.erase(i,objects.end());
    
    obj->parent = NULL;
    obj->removeScene();
    
    obj->viewMatrix = NULL;
    obj->viewProjectionMatrix = NULL;
    
    obj->needUpdate();
}

void Object::setSceneDepth(bool depth){
    if (scene) {
        if (scene->getUserDefinedDepth() != S_OPTION_NO)
            scene->useDepth = depth;
    }
}

void Object::setPosition(const float x, const float y, const float z){
    setPosition(Vector3(x, y, z));
}

void Object::setPosition(const float x, const float y){
    setPosition(Vector3(x, y, this->position.z));
}

void Object::setPosition(Vector2 position){
    setPosition(Vector3(position.x, position.y, this->position.z));
}

void Object::setPosition(Vector3 position){
    if (this->position != position){
        this->position = position;
        needUpdate();
    }
}

Vector3 Object::getPosition(){
    return position;
}

Vector3 Object::getWorldPosition(){
    return worldPosition;
}

void Object::setRotation(const float xAngle, const float yAngle, const float zAngle){
    Quaternion qx, qy, qz;

    qx.fromAngleAxis(xAngle, Vector3(1,0,0));
    qy.fromAngleAxis(yAngle, Vector3(0,1,0));
    qz.fromAngleAxis(zAngle, Vector3(0,0,1));

    setRotation(qz * (qy * qx)); //order ZYX
}

void Object::setRotation(Quaternion rotation){
    if (this->rotation != rotation){
        this->rotation = rotation;
        needUpdate();
    }
}

Quaternion Object::getRotation(){
    return this->rotation;
}

Quaternion Object::getWorldRotation(){
    return this->worldRotation;
}

void Object::setScale(const float factor){
    setScale(Vector3(factor,factor,factor));
}

void Object::setScale(Vector3 scale){
    if (this->scale != scale){
        this->scale = scale;
        needUpdate();
    }
}

Vector3 Object::getScale(){
    return this->scale;
}

Vector3 Object::getWorldScale(){
    return this->worldScale;
}

void Object::setCenter(const float x, const float y, const float z){
    setCenter(Vector3(x, y, z));
}

void Object::setCenter(Vector3 center){
    if (this->center != center){
        this->center = center;
        needUpdate();
    }
}

void Object::setCenter(const float x, const float y){
    setCenter(Vector3(x, y, getCenter().z));
}

void Object::setCenter(Vector2 center){
    setCenter(Vector3(center.x, center.y, getCenter().z));
}

Vector3 Object::getCenter(){
    return center;
}

Matrix4 Object::getTransformMatrix(){
    return transformMatrix;
}

Matrix4 Object::getModelMatrix(){
    return modelMatrix;
}

Matrix4 Object::getModelViewProjectMatrix(){
    return modelViewProjectionMatrix;
}

Vector3 Object::getCameraPosition(){
    return cameraPosition;
}

Scene* Object::getScene(){
    return scene;
}

Object* Object::getParent(){
    return parent;
}

int Object::find(Object* object){
    
    for (int i=0; i < objects.size(); i++){
        if (objects[i] == object){
            return i;
        }
    }
    
    return -1;
}

void Object::moveTo(int index){
    if (parent != NULL){
        int pos = parent->find(this);
        if ((index >= 0) && (index <= (parent->objects.size()-1))) {
            if (pos < index) {
                Object *temp = parent->objects[pos];

                for (int i = pos; i < index; i++) {
                    parent->objects[i] = parent->objects[i + 1];
                }
                parent->objects[index] = temp;
            }

            if (pos > index) {
                Object *temp = parent->objects[pos];

                for (int i = pos; i > index; i--) {
                    parent->objects[i] = parent->objects[i - 1];
                }
                parent->objects[index] = temp;
            }
        }
    }
}

void Object::moveToFirst(){
    moveTo(parent->objects.size()-1);
}

void Object::moveToLast(){
    moveTo(0);
}

void Object::moveDown(){
    if (parent != NULL){
        int pos = parent->find(this);
        
        if (pos > 0){
            Object* temp = parent->objects[pos];
            parent->objects[pos] = parent->objects[pos-1];
            parent->objects[pos-1] = temp;
        }
    }
    
}
void Object::moveUp(){
    if (parent != NULL){
        int pos = parent->find(this);
        
        if ((pos >= 0) && (pos < (parent->objects.size()-1))){
            Object* temp = parent->objects[pos];
            parent->objects[pos] = parent->objects[pos+1];
            parent->objects[pos+1] = temp;
        }
    }
}

void Object::lookAt(Vector3 target, Vector3 up){
    Matrix4 m1 = Matrix4::lookAtMatrix(target, worldPosition, up);

    Quaternion oldRotation = rotation;

    rotation.fromRotationMatrix(m1);
    if (parent)
        rotation = parent->getWorldRotation().inverse() * rotation;

    if (rotation != oldRotation)
        needUpdate();
}

void Object::addAction(Action* action){
    bool found = false;

    std::vector<Action*>::iterator it;
    for (it = actions.begin(); it != actions.end(); ++it) {
        if (action == (*it))
            found = true;
    }

    if (!found){
        if (!action->object) {
            actions.push_back(action);
            action->object = this;
        }else{
            Log::Error("This action is attached to other object");
        }
    }
}

void Object::removeAction(Action* action){
    if (action->object == this){
        std::vector<Action*>::iterator i = std::remove(actions.begin(), actions.end(), action);
        actions.erase(i,actions.end());
        action->object = NULL;
    }else{
        Log::Error("This action is attached to other object");
    }
}

void Object::updateVPMatrix(Matrix4* viewMatrix, Matrix4* projectionMatrix, Matrix4* viewProjectionMatrix, Vector3* cameraPosition){
    
    this->viewMatrix = viewMatrix;
    this->projectionMatrix = projectionMatrix;
    this->viewProjectionMatrix = viewProjectionMatrix;
    this->cameraPosition = *cameraPosition;

    updateMVPMatrix();
    
    std::vector<Object*>::iterator it;
    for (it = objects.begin(); it != objects.end(); ++it) {
        (*it)->updateVPMatrix(viewMatrix, projectionMatrix, viewProjectionMatrix, cameraPosition);
    }
    
}

void Object::updateModelMatrix(){
    markToUpdate = false;

    Matrix4 centerMatrix = Matrix4::translateMatrix(-center);
    Matrix4 scaleMatrix = Matrix4::scaleMatrix(scale);
    Matrix4 translateMatrix = Matrix4::translateMatrix(position);
    Matrix4 rotationMatrix = rotation.getRotationMatrix();

    this->transformMatrix = translateMatrix * rotationMatrix * scaleMatrix;
    this->modelMatrix = this->transformMatrix * centerMatrix;

    if (parent != NULL){
        Matrix4 parentCenterMatrix = Matrix4::translateMatrix(parent->center);
        this->modelMatrix = parent->modelMatrix * parentCenterMatrix * this->modelMatrix;
        worldRotation = parent->worldRotation * rotation;
        worldScale = Vector3(parent->worldScale.x * scale.x, parent->worldScale.y * scale.y, parent->worldScale.z * scale.z);
        worldPosition = modelMatrix * center;
    }else{
        worldRotation = rotation;
        worldScale = scale;
        worldPosition = position;
    }

    updateMVPMatrix();

    if (allowBodyUpdate) {
        updateBodyFromObject();
    }
}

void Object::needUpdate(){
    if (!markToUpdate) {
        markToUpdate = true;

        std::vector<Object *>::iterator it;
        for (it = objects.begin(); it != objects.end(); ++it) {
            (*it)->needUpdate();
        }
    }
}

void Object::updateMVPMatrix(){
    if (this->viewProjectionMatrix != NULL){
        this->modelViewProjectionMatrix = (*this->viewProjectionMatrix) * this->modelMatrix;
    }
}

bool Object::isIn3DScene(){
    if (scene && scene->is3D())
        return true;
    
    return false;
}

void Object::setOwnedBody(bool ownedBody){
    this->ownedBody = ownedBody;
}

Body2D* Object::createBody2D(){
    setBody(new Body2D());
    return (Body2D*)getBody();
}

//void Object::createBody3D(){
//    setBody(new Body3D());
//}

void Object::setBody(Body* body){
    if (!body && this->body){

        this->body->attachedObject = NULL;
        this->body = NULL;

    }else if (!body->attachedObject){

        if (this->body != body) {
            if (ownedBody)
                delete this->body;

            this->body = body;
            body->attachedObject = this;

            updateBodyFromObject();
        }

    }else{
        Log::Error("Body is attached with other object already");
    }
}

Body* Object::getBody(){
    return body;
}

void Object::updateBodyFromObject(){
    if (body) {
        if (body->isWorldSpace()) {
            body->setPosition(worldPosition);
            body->setRotation(worldRotation);
        }else{
            body->setPosition(position);
            body->setRotation(rotation);
        }
    }
}


void Object::updateFromBody(){
    if (body){
        bool needUpdateBody = false;
        Vector3 bodyPosition = body->getPosition();
        Quaternion bodyRotation = body->getRotation();

        if (body->isWorldSpace()) {

            if (getWorldPosition() != bodyPosition) {
                position = parent->getModelMatrix().inverse() * bodyPosition;
                needUpdateBody = true;
            }

            if (getWorldRotation() != bodyRotation) {
                rotation = parent->rotation.inverse() * bodyRotation;
                needUpdateBody = true;
            }

        }else{

            if (getPosition() != bodyPosition) {
                position = bodyPosition;
                needUpdateBody = true;
            }

            if (getRotation() != bodyRotation) {
                rotation = bodyRotation;
                needUpdateBody = true;
            }

        }

        if (needUpdateBody) {
            allowBodyUpdate = false;
            needUpdate();
            allowBodyUpdate = true;
        }
    }
}

bool Object::isLoaded(){
    return loaded;
}

bool Object::isMarkToUpdate(){
    return markToUpdate;
}

bool Object::reload(){
    //TODO: Check if it is really necessary
    destroy();
    return load();
}

bool Object::load(){

    if ((position.z != 0) && isIn3DScene()){
        setSceneDepth(true);
    }

    std::vector<Object*>::iterator it;
    for (it = objects.begin(); it != objects.end(); ++it) {
        (*it)->load();
    }

    if (scene && body && scene->physicsWorld)
        scene->getPhysicsWorld()->addBody(body);

    loaded = true;

    return loaded;

}

bool Object::draw(){
    if (position.z != 0){
        setSceneDepth(true);
    }
    
    std::vector<Object*>::iterator it;
    for (it = objects.begin(); it != objects.end(); ++it) {
        if ((*it)->scene != (*it)){ //if not a scene object
            if ((*it)->loaded)
                (*it)->draw();
        }
    }
    
    return loaded;
}

void Object::update(){

    for (int i = 0; i < actions.size(); i++) {
        if (actions[i]->isRunning()) {
            actions[i]->update(Engine::getSceneUpdateTime());
        }
    }

    if (markToUpdate) {
        updateModelMatrix();
    }

    std::vector<Object*>::iterator it;
    for (it = objects.begin(); it != objects.end(); ++it) {
        (*it)->update();
    }
}

void Object::destroy(){

    std::vector<Object*>::iterator it;
    for (it = objects.begin(); it != objects.end(); ++it){
        (*it)->destroy();
    }

    loaded = false;

}

Object* Object::getObject(unsigned int index) const{
    if (index >= 0 && index < objects.size())
        return objects[index];

    return NULL;
}

const std::vector<Object *> &Object::getObjects() const {
    return objects;
}
