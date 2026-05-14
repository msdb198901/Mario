#pragma once

#include "Observer.h"

struct GLFWwindow;
class Framebuffer;
class PickingTexture;
class ImGuiLayer;

class Scene;
class SceneInitializer;
// class ALCdevice;
// class ALCcontext;
class Window : public Observer {
    public:
        static Window* getInstance();
        static void changeScene(SceneInitializer* sceneInitializer);

        Window(int width, int height);
        ~Window();

        void run();

        void update(float deltaTime);
        void render();
        void onNotify(GameObject *gameObject, Event* event) override;

        static int getWidth();
        static int getHeight();
        static Framebuffer* getFramebuffer();
        static float getTargetAspectRatio() { return 16.0f / 9.0f; }
        static ImGuiLayer* getImGuiLayer();
        static Scene* getCurrentScene();

        static bool RELEASE_BUILD;
    private:
        void init();
        void initAudio();
        void loop();

        void cleanupAudio();

    private:
        int width;
        int height;
        const char* title;
        GLFWwindow* glfwWindow;

        Framebuffer* framebuffer;
        PickingTexture* pickingTexture;

        ImGuiLayer* imguiLayer;
        Scene *currentScene = nullptr;

        // ALCdevice* audioDevice;
        // ALCcontext* audioContext;
        bool runtimePlaying = false;
        static Window *instance;
};