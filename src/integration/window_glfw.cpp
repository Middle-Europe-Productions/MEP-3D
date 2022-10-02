#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glog/logging.h>
#include <MEP-3D/window.hpp>

#ifdef MEP_ENGINE_USE_IMGUI
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <imgui.h>
#endif

class GLFWWindowController : public Window {
 public:
  GLFWWindowController(WindowConfig config)
      : init_(false),
        block_events_(false),
        exception_(Keyboard::UnknownKey),
        main_window_(nullptr),
        config_(config) {}

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
    glfwSwapInterval(config_.use_vsync);

    InitCallbacks();

#ifdef MEP_ENGINE_USE_IMGUI
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
    io.ConfigFlags |= ImGuiWindowFlags_MenuBar;
    ImGui::StyleColorsDark();
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
      style.WindowRounding = 0.0f;
      style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }
    ImGui_ImplGlfw_InitForOpenGL(main_window_, true);
    ImGui_ImplOpenGL3_Init("#version 330");
    LOG(INFO) << "IamGui initialized";
#endif

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

  void BlockEvents(bool status, Keyboard exception) override {
    block_events_ = status;
    exception_ = exception;
    ForAllObservers(
        [status](WindowObserver* obs) { obs->OnEventStatusChanged(status); });
  }

  bool ShouldBlockEvent() { return block_events_; }

  void Clear(Color color = Black) override {
    glClearColor(color.Rf(), color.Gf(), color.Bf(), color.Af());
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  }

  void StartLoop() override { glfwPollEvents(); }

  void FinishLoop() override { glfwSwapBuffers(main_window_); }

  Vec2i GetSize() override { return config_.size; }

  Vec2i GetBufferSize() override { return buffer_size; }

  float GetAspectRation() override {
    return static_cast<float>(buffer_size.x_) /
           static_cast<float>(buffer_size.y_);
  }

  ~GLFWWindowController() override {
    LOG(INFO) << "Destroying window!";
#ifdef MEP_ENGINE_USE_IMGUI
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
#endif
    glfwDestroyWindow(main_window_);
    glfwTerminate();
  }

 private:
  bool init_;
  bool block_events_;
  Keyboard exception_;
  GLFWwindow* main_window_;
  GLFWWindowController* shared_window_;
  WindowConfig config_;
  Vec2i buffer_size;

  void InitCallbacks() {
    glfwSetKeyCallback(main_window_, OnKeyEventHandler);
    glfwSetCursorPosCallback(main_window_, OnMouseEventHandler);
    glfwSetWindowSizeCallback(main_window_, OnWindowResizeEventHandler);
  }
  static void OnKeyEventHandler(GLFWwindow* window,
                                int key,
                                int code,
                                int action,
                                int mode);
  static void OnMouseEventHandler(GLFWwindow* window, double xPos, double yPos);
  static void OnWindowResizeEventHandler(GLFWwindow* window,
                                         int width,
                                         int height);
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
    if (action != GLFW_RELEASE && master_window->ShouldBlockEvent() &&
        key_event.code != master_window->exception_) {
      return;
    }

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
  if (master_window->ShouldBlockEvent())
    return;
  MouseEvent mouse_event{xPos, yPos};
  master_window->ForAllObservers(
      [&mouse_event](WindowObserver* obs) { obs->OnMouseEvent(mouse_event); });
}

void GLFWWindowController::OnWindowResizeEventHandler(GLFWwindow* window,
                                                      int width,
                                                      int height) {
  GLFWWindowController* master_window =
      static_cast<GLFWWindowController*>(glfwGetWindowUserPointer(window));
  glfwGetFramebufferSize(master_window->main_window_,
                         &master_window->buffer_size.x_,
                         &master_window->buffer_size.y_);
  LOG(INFO) << __FUNCTION__
            << ", buffer size changed [x: " << master_window->buffer_size.x_
            << ", y: " << master_window->buffer_size.y_ << "]";
  glViewport(0, 0, master_window->buffer_size.x_,
             master_window->buffer_size.y_);
  if (!master_window->GetView()) {
    LOG(ERROR) << "You did not bind view to window";
  } else {
    master_window->GetView()->UpdateAspectRation(
        master_window->GetAspectRation());
  }
  Vec2i size{width, height};
  master_window->ForAllObservers(
      [&size](WindowObserver* obs) { obs->OnWindowResizeEvent(size); });
}

std::unique_ptr<Window> Window::GetInstance(WindowConfig config) {
  return std::make_unique<GLFWWindowController>(config);
}