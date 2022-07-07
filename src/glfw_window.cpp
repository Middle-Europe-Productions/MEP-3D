#include "window.hpp"
#include "glog/logging.h"
#include "GL/glew.h"
#include<GLFW/glfw3.h>

class GLFWWindow: public Window {
public:
	GLFWWindow(WindowConfig config):
        init_(false), main_window_(nullptr), config_(config) {}

	bool Init() {
        if (!glfwInit()) {
            LOG(ERROR) << "GLFW Error";
            glfwTerminate();
            return false;
        }
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

        main_window_ = glfwCreateWindow(config_.width, config_.height, config_.name.c_str(), NULL, NULL);
        if (!main_window_) {
            LOG(ERROR) << "GLFW window failed";
            glfwTerminate();
            return false;
        }
        int buffer_X, buffer_Y;
        glfwGetFramebufferSize(main_window_, &buffer_X, &buffer_Y);

        glfwMakeContextCurrent(main_window_);

        glewExperimental = GL_TRUE;

        GLenum err = glewInit();
        if (GLEW_OK != err) {
            LOG(ERROR) << "GLEW failed: " << glewGetErrorString(err);
            glfwDestroyWindow(main_window_);
            glfwTerminate();
            return false;
        }
        glEnable(GL_DEPTH_TEST);
        glViewport(0, 0, buffer_X, buffer_Y);
        init_ = true;
        return true;
	}
	void Close() {
		LOG(WARNING) << "Not implemented";
	}
	
	bool IsOpen() {
        return init_ && !glfwWindowShouldClose(main_window_);
	}

	void FinishLoop() {
        glfwSwapBuffers(main_window_);
	}
	std::size_t GetHight() {
        return config_.width;
	}
	std::size_t GetWidth() {
        return config_.height;
	}
	~GLFWWindow() {

	}

private:
    bool init_;
    GLFWwindow* main_window_;
    WindowConfig config_;
};

std::unique_ptr<Window> Window::GetInstance(WindowConfig config) {
    return std::make_unique<GLFWWindow>(config);
}