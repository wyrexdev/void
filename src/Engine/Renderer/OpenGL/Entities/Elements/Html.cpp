#include "Engine/Renderer/OpenGL/Entities/Elements/Html.hpp"

Html::Html() {
    setBackgroundColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
}

void Html::onStart() {

}

void Html::onDraw() {
    setWidth(getEntitiesSize().x);
    setHeight(getEntitiesSize().y);
}

void Html::onUpdate() {

}