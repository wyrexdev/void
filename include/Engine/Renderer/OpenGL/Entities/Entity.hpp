#include "glm/glm/glm.hpp"

class Entity {
public:
    void setPosition(glm::vec3 pos);
    void setPosition(float x, float y, float z);
    void setPosition(float x, float y);
    void setPosX(float x);
    void setPosY(float y);
    void setZIndex(float z);

    void setRotate(glm::vec3 rotate);
    void setRotate(float x, float y, float z);
    void setRotate(float x, float y);
    void setRotX(float x);
    void setRotY(float y);
    void setRotZ(float z);

    void setScale(glm::vec3 scale);
    void setWidth(float x);
    void setHeight(float y);
    void setScaleZ(float z);

    glm::vec3 getPosition();
    glm::vec3 getRotation();
    glm::vec3 getScale();
private:
    glm::vec3 pos;
    glm::vec3 rotate;
    glm::vec3 scale;
};