#pragma once 

#include <unordered_map>

struct GLFWwindow;
class KeyListener {
    public:
        static void endFrame();
        static KeyListener* getInstance();
        static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
        static bool isKeyBeginPressed(int keyCode);
        static bool isKeyDown(int keyCode); 

    private:
        static std::unordered_map<int, bool> keyPressed;
        static std::unordered_map<int, bool> keyBeginPressed;
        static KeyListener* instance;

        KeyListener();
};