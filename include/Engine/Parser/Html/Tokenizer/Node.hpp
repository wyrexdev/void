#pragma once

#include <vector>
#include <memory>
#include <string>

class Node {
public:
    std::string name;
    Node* parent = nullptr;
    std::vector<std::unique_ptr<Node>> children;

    virtual ~Node() = default;

    void appendChild(std::unique_ptr<Node> child) {
        child->parent = this;
        children.push_back(std::move(child));
    }
};
