#include "Engine/Renderer/OpenGL/Entities/Entity.hpp"
#include "Engine/Renderer/OpenGL/Utils/Screen.hpp"

class Body : public Entity {
public:
    Body();

    void onStart() override;
    void onDraw() override;
    void onUpdate() override;
};