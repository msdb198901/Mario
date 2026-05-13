#pragma once

#include <string>

class GameObject {
    public:
        GameObject();
        virtual ~GameObject();

    private:
        static unsigned int ID_COUNTER;
        unsigned int uid = ID_COUNTER++;
        std::string name;
};