#pragma once

enum EventType {
    GAME_START,
    GAME_OVER,
    SAVE_LEVEL,
    LOAD_LEVEL,
    USER_EVENT,
};

class Event {
    public:
        Event() : type(USER_EVENT) {}
        Event(EventType type) : type(type) {}
        virtual ~Event() {}
        
        EventType getType() const { return type; }
    private:
        EventType type;
};