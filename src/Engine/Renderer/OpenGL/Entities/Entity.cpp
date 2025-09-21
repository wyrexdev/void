#include "Engine/Renderer/OpenGL/Entities/Entity.hpp"

Entity::~Entity() {}

void Entity::setPosition(glm::vec3 pos)
{
    this->pos = pos;
}

void Entity::setPosition(float x, float y, float z)
{
    this->pos.x = x;
    this->pos.y = y;
    this->pos.z = z;
}

void Entity::setPosition(float x, float y)
{
    this->pos.x = x;
    this->pos.y = y;
}

void Entity::setPosX(float x)
{
    this->pos.x = x;
}

void Entity::setPosY(float y)
{
    this->pos.y = y;
}

void Entity::setZIndex(float z)
{
    this->pos.z = z;
}



void Entity::setRotate(glm::vec3 rotate)
{
    this->rotate = rotate;
}

void Entity::setRotate(float x, float y, float z) {
    this->rotate.x = x;
    this->rotate.y = y;
    this->rotate.z = z;
}


void Entity::setRotate(float x, float y) {
    this->rotate.x = x;
    this->rotate.y = y;
}

void Entity::setRotX(float x) {
    this->rotate.x = x;
}

void Entity::setRotY(float y) {
    this->rotate.y = y;
}

void Entity::setRotZ(float z) {
    this->rotate.z = z;
}



void Entity::setScale(glm::vec3 scale) {
    this->scale = scale;
}

void Entity::setWidth(float w) {
    this->scale.x = w;
}

void Entity::setHeight(float h) {
    this->scale.y = h;
}

void Entity::setScaleZ(float z) {
    this->scale.z = z;
}



glm::vec3 Entity::getPosition() {
    return this->pos;
}

glm::vec3 Entity::getRotation() {
    return this->rotate;
}

glm::vec3 Entity::getScale() {
    return this->scale;
}



void Entity::start() {

}

void Entity::draw() {

}

void Entity::update() {
    
}