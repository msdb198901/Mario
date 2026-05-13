#pragma once 

#include <vector>
#include "Observer.h"
#include "Event.h"

class EventSystem {
    public:
        static void addObserver(Observer* observer) {
            observers.push_back(observer);
        }

        static void notify(GameObject *gameObject, Event* event) {
            for (auto observer : observers) {
                observer->onNotify(gameObject, event);
            }
        }

    private:
        static inline std::vector<Observer*> observers;
};