#include "Submesh.h"

using namespace Supernova;

Submesh::Submesh(){
    this->render = NULL;

    this->distanceToCamera = -1;
    this->material = NULL;
    this->materialOwned = false;
    this->dynamic = false;

    this->loaded = false;

    this->minBufferSize = 0;
}

Submesh::Submesh(Material* material): Submesh() {
    this->material = material;
}

Submesh::~Submesh(){
    if (materialOwned)
        delete material;
    
    if (render)
        delete render;

    if (loaded)
        destroy();
}

Submesh::Submesh(const Submesh& s){
    this->indices = s.indices;
    this->distanceToCamera = s.distanceToCamera;
    this->materialOwned = s.materialOwned;
    this->material = s.material;
    this->dynamic = s.dynamic;
    this->loaded = s.loaded;
    this->render = s.render;
    this->minBufferSize = s.minBufferSize;
}

Submesh& Submesh::operator = (const Submesh& s){
    this->indices = s.indices;
    this->distanceToCamera = s.distanceToCamera;
    this->materialOwned = s.materialOwned;
    this->material = s.material;
    this->dynamic = s.dynamic;
    this->loaded = s.loaded;
    this->render = s.render;
    this->minBufferSize = s.minBufferSize;

    return *this;
}

bool Submesh::isDynamic(){
    return dynamic;
}

unsigned int Submesh::getMinBufferSize(){
    return minBufferSize;
}

void Submesh::setIndices(std::vector<unsigned int> indices){
    this->indices = indices;
}

void Submesh::addIndex(unsigned int index){
    this->indices.push_back(index);
}

std::vector<unsigned int>* Submesh::getIndices(){
    return &indices;
}

unsigned int Submesh::getIndex(int offset){
    return indices[offset];
}

void Submesh::createNewMaterial(){
    this->material = new Material();
    this->materialOwned = true;
}

void Submesh::setMaterial(Material* material){
    this->material = material;
}

Material* Submesh::getMaterial(){
    return this->material;
}

SubmeshRender* Submesh::getSubmeshRender(){
    return render;
}

bool Submesh::load(){
    SubmeshRender::newInstance(&render);
        
    render->setSubmesh(this);
    render->load();

    loaded = true;
    
    return true;
}

bool Submesh::draw(){
    return render->draw();
}

void Submesh::destroy(){
    render->destroy();

    loaded = false;
}
