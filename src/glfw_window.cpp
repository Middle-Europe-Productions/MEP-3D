#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glog/logging.h>
#include <MEP-3D/window.hpp>


class GLFWWindowController : public Window {
 public:
  GLFWWindowController(WindowConfig config)
      : init_(false), main_window_(nullptr), config_(config) {}

  bool Init() override {
    if (!glfwInit()) {
      LOG(ERROR) << "GLFW Error";
      glfwTerminate();
      return false;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    main_window_ = glfwCreateWindow(config_.size.x_, config_.size.y_,
                                    config_.name.c_str(), NULL, NULL);

    if (!main_window_) {
      LOG(ERROR) << "GLFW window failed";
      glfwTerminate();
      return false;
    }
    glfwGetFramebufferSize(main_window_, &buffer_size.x_, &buffer_size.y_);
    LOG(INFO) << __FUNCTION__ << ", buffer initialized [x: " << buffer_size.x_
              << ", y: " << buffer_size.y_ << "]";
    glfwMakeContextCurrent(main_window_);

    InitCallbacks();

    glewExperimental = GL_TRUE;

    GLenum err = glewInit();
    if (GLEW_OK != err) {
      LOG(ERROR) << "GLEW failed: " << glewGetErrorString(err);
      glfwDestroyWindow(main_window_);
      glfwTerminate();
      return false;
    }
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    glViewport(0, 0, buffer_size.x_, buffer_size.y_);
    glfwSetWindowUserPointer(main_window_, this);
    init_ = true;
    return true;
  }

  void Close() override { LOG(WARNING) << "Not implemented"; }

  bool IsOpen() override {
    return init_ && !glfwWindowShouldClose(main_window_);
  }

  void Clear(Color color = Black) override {
    glClearColor(color.Rf(), color.Gf(), color.Bf(), color.Af());
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  }

  void FinishLoop() override { glfwSwapBuffers(main_window_); }

  virtual Vec2i GetSize() { return config_.size; }

  virtual Vec2i GetBufferSize() { return buffer_size; }

  virtual float GetAspectRation() {
    return static_cast<float>(buffer_size.x_) /
           static_cast<float>(buffer_size.y_);
  }

  ~GLFWWindowController() {
    glfwDestroyWindow(main_window_);
    glfwTerminate();
  }

 private:
  bool init_;
  GLFWwindow* main_window_;
  GLFWWindowController* shared_window_;
  WindowConfig config_;
  Vec2i buffer_size;

  void InitCallbacks() {
    glfwSetKeyCallback(main_window_, OnKeyEventHandler);
    glfwSetCursorPosCallback(main_window_, OnMouseEventHandler);
  }
  static void OnKeyEventHandler(GLFWwindow* window,
                                int key,
                                int code,
                                int action,
                                int mode);
  static void OnMouseEventHandler(GLFWwindow* window, double xPos, double yPos);
};

void GLFWWindowController::OnKeyEventHandler(GLFWwindow* window,
                                             int key,
                                             int code,
                                             int action,
                                             int mode) {
  GLFWWindowController* master_window =
      static_cast<GLFWWindowController*>(glfwGetWindowUserPointer(window));
  if (key >= (int)'A' && key <= (int)'Z') {
    KeyEvent key_event;
    key_event.code = static_cast<Keyboard>(key - 'A');
    if (action == GLFW_PRESS) {
      key_event.action = Action::Pressed;
    } else if (action == GLFW_RELEASE) {
      key_event.action = Action::Released;
    }
    master_window->ForAllObservers(
        [&key_event](WindowObserver* obs) { obs->OnKeyEvent(key_event); });
  }
}

void GLFWWindowController::OnMouseEventHandler(GLFWwindow* window,
                                               double xPos,
                                               double yPos) {
  GLFWWindowController* master_window =
      static_cast<GLFWWindowController*>(glfwGetWindowUserPointer(window));
  MouseEvent mouse_event{xPos, yPos};
  master_window->ForAllObservers(
      [&mouse_event](WindowObserver* obs) { obs->OnMouseEvent(mouse_event); });
}

std::unique_ptr<Window> Window::GetInstance(WindowConfig config) {
  return std::make_unique<GLFWWindowController>(config);
}