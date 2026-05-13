#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Window.h"
#include "EventSystem.h"
#include "Framebuffer.h"
#include "PickingTexture.h"
#include "ImGuiLayer.h"

#include "Scene.h"
#include "LevelEditorSceneInitializer.h"
#include "SceneInitializer.h"

// #include <AL/alc.h>
#include <iostream>
#include <stdexcept>


Window *Window::instance = nullptr;
bool Window::RELEASE_BUILD = false;

Window* Window::getInstance() {
    if (instance == nullptr) {
        instance = new Window(800, 600);
    }
    return instance;
}

void Window::changeScene(SceneInitializer* sceneInitializer) {
    // Implement scene change logic here
    if (instance->currentScene) {
        instance->currentScene->destroy();
        //delete instance->currentScene;
    }
    instance->currentScene = new Scene(sceneInitializer);
    instance->currentScene->load();
    instance->currentScene->init();
    instance->currentScene->start();
}

Window::Window(int width, int height) : width(width), height(height) {
    // Initialize window here
    title = "Game Window";
    EventSystem::addObserver(this);
}

Window::~Window() {
    // Clean up window here

}

void Window::run() {
    // Main loop for the window here
    init();
    loop();

    cleanupAudio();
    glfwDestroyWindow(glfwWindow);
    glfwTerminate();
}

void Window::init() {
    // Initialization code here
    glfwSetErrorCallback([](int error, const char* description) {
        std::cerr << "GLFW Error (" << error << "): " << description << std::endl;
    });

    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create window here
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindow = glfwCreateWindow(width, height, title, NULL, NULL);
    if (!glfwWindow) {
        std::cerr << "Failed to create window" << std::endl;
        glfwTerminate();
        return;
    }

    glfwSetCursorPosCallback(glfwWindow, [](GLFWwindow* window, double xpos, double ypos) {
        // Handle mouse movement here
    });

    glfwSetMouseButtonCallback(glfwWindow, [](GLFWwindow* window, int button, int action, int mods) {
        // Handle mouse button events here
    });

    glfwSetScrollCallback(glfwWindow, [](GLFWwindow* window, double xoffset, double yoffset) {
        // Handle scroll events here
    });

    glfwSetKeyCallback(glfwWindow, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
        // Handle key events here
    });

    glfwSetWindowSizeCallback(glfwWindow, [](GLFWwindow* window, int width, int height) {
        // Handle window size changes here
        instance->width = width;
        instance->height = height;
    });

    // Make the context of the window current here
    glfwMakeContextCurrent(glfwWindow);

    glfwSwapInterval(1); // Enable V-Sync
    glfwShowWindow(glfwWindow); // Show the window

    // Initialize OpenAL here
    initAudio();

    // 加载 OpenGL 函数指针
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return;
    }

    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    
    framebuffer = new Framebuffer(3840, 2160);
    pickingTexture = new PickingTexture(3840, 2160);
    glViewport(0, 0, width, height);
    

    imguiLayer = new ImGuiLayer(glfwWindow, pickingTexture);
    imguiLayer->initImGui();
    Window::changeScene(new LevelEditorSceneInitializer());
}

void Window::initAudio() {
    // // 1. 获取默认设备名称并打开设备
    // const ALCchar* defaultDeviceName = alcGetString(nullptr, ALC_DEFAULT_DEVICE_SPECIFIER);
    // audioDevice = alcOpenDevice(defaultDeviceName);
    // if (!audioDevice) {
    //     throw std::runtime_error("Failed to open default OpenAL device");
    // }

    // // 2. 创建音频上下文（属性列表传 nullptr 表示使用默认属性，等同于 Java 中的 {0}）
    // audioContext = alcCreateContext(audioDevice, nullptr);
    // if (!audioContext) {
    //     alcCloseDevice(audioDevice);
    //     throw std::runtime_error("Failed to create OpenAL context");
    // }

    // // 3. 激活当前线程的音频上下文
    // if (!alcMakeContextCurrent(audioContext)) {
    //     alcDestroyContext(audioContext);
    //     alcCloseDevice(audioDevice);
    //     throw std::runtime_error("Failed to make OpenAL context current");
    // }

    // // 4. 检查 OpenAL 1.0 支持（可选）
    // const char* version = alGetString(AL_VERSION);
    // if (!version || version[0] == '\0') {
    //     throw std::runtime_error("OpenAL 1.0 not supported");
    // }
    // std::cout << "OpenAL version: " << version << std::endl;

    // // 验证无错误
    // ALCenum error = alcGetError(audioDevice);
    // if (error != ALC_NO_ERROR) {
    //     throw std::runtime_error("OpenAL initialization error: " + std::to_string(error));
    // }
}

void Window::cleanupAudio() {
    // // 5. 释放音频上下文和设备资源
    // alcMakeContextCurrent(nullptr);
    // alcDestroyContext(audioContext);
    // alcCloseDevice(audioDevice);
}

void Window::loop() {
    // Main loop code here
     while (!glfwWindowShouldClose(glfwWindow)) {
        float deltaTime = 0.0f; // Calculate delta time here

        update(deltaTime);
        render();

        instance->imguiLayer->update(deltaTime, currentScene);

        glfwSwapBuffers(glfwWindow);
        glfwPollEvents();
     }
}

void Window::update(float deltaTime) {
    // Update window here
}

void Window::render() {
    // Render window here
}

void Window::onNotify(GameObject *gameObject, Event* event) {
    // Handle notifications here
}

int Window::getWidth() {
    return instance->width;
}

int Window::getHeight() {
    return instance->height;
}

Framebuffer* Window::getFramebuffer() {
    return instance->framebuffer;
}

ImGuiLayer* Window::getImGuiLayer() {
    return instance->imguiLayer;
}

Scene* Window::getCurrentScene() {
    return instance->currentScene;
}