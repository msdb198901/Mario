#include "GameObject.h"

unsigned int GameObject::ID_COUNTER = 0;

GameObject::GameObject() {
    uid = ID_COUNTER++;
}

GameObject::~GameObject() {
}
