#include "glm/glm/glm.hpp"

class Entity {
public:
    void setPosition(glm::vec3 pos) {
        this->pos = pos;
    }

    void setPosition(float x, float y, float z) {
        this->pos.x = x;
        this->pos.y = y;
        this->pos.z = z;
    }

    void setPosition(float x, float y) {
        this->pos.x = x;
        this->pos.y = y;
    }

    void setPosX(float x) {
        this->pos.x = x;
    }

    void setPosY(float y) {
        this->pos.y = y;
    }

    void setZIndex(float z) {
        this->pos.z = z;
    }

    void setRotate(glm::vec3 rotate) {

    }
private:
    glm::vec3 pos;
    glm::vec3 rotate;
    glm::vec3 scale;
};