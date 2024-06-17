#include "Node.h"

Node::Node() : up(nullptr), down(nullptr), left(nullptr), right(nullptr), visited(false) {}

void Node::setUp(Node* node) {
    up = node;
}

void Node::setDown(Node* node) {
    down = node;
}

void Node::setLeft(Node* node) {
    left = node;
}

void Node::setRight(Node* node) {
    right = node;
}
