#include "Engine/Engine.hpp"

Engine::Engine() {

}

std::string Engine::parse(const std::string &content) {
    this->content = content;
    return "";
}

SkiaRenderWidget *Engine::getSkiaView() {
    return skiaView;
}