#include "Windows/Window.h"

int main() {
    Window* window = Window::getInstance();
    window->run();
    return 0;
}