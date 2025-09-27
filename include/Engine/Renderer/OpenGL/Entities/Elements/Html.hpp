#include "Engine/Renderer/OpenGL/Entities/Entity.hpp"

class Html : public Entity {
public:
    Html();

    void onStart() override;
    void onDraw() override;
    void onUpdate() override;
};