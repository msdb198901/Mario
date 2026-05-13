#pragma once

class GameObject;
class Event;
class Observer {
    public:
        virtual void onNotify(GameObject *gameObject, Event* event) = 0;
};