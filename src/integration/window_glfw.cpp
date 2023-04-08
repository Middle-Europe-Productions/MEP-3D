#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glog/logging.h>
#include <MEP-3D/window.hpp>

#ifdef MEP_ENGINE_USE_IMGUI
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <imgui.h>
#endif

namespace mep {
namespace {
Key ToMepKey(int value) {
  switch (value) {
    case GLFW_KEY_SPACE:
      return Key::Space;
    case GLFW_KEY_APOSTROPHE:
      return Key::Apostrophe;
    case GLFW_KEY_COMMA:
      return Key::Comma;
    case GLFW_KEY_MINUS:
      return Key::Minus;
    case GLFW_KEY_PERIOD:
      return Key::Period;
    case GLFW_KEY_SLASH:
      return Key::Slash;
    case GLFW_KEY_0:
      return Key::Key0;
    case GLFW_KEY_1:
      return Key::Key1;
    case GLFW_KEY_2:
      return Key::Key2;
    case GLFW_KEY_3:
      return Key::Key3;
    case GLFW_KEY_4:
      return Key::Key4;
    case GLFW_KEY_5:
      return Key::Key5;
    case GLFW_KEY_6:
      return Key::Key6;
    case GLFW_KEY_7:
      return Key::Key7;
    case GLFW_KEY_8:
      return Key::Key8;
    case GLFW_KEY_9:
      return Key::Key9;
    case GLFW_KEY_SEMICOLON:
      return Key::Semicolon;
    case GLFW_KEY_EQUAL:
      return Key::Equal;
    case GLFW_KEY_A:
      return Key::A;
    case GLFW_KEY_B:
      return Key::B;
    case GLFW_KEY_C:
      return Key::C;
    case GLFW_KEY_D:
      return Key::D;
    case GLFW_KEY_E:
      return Key::E;
    case GLFW_KEY_F:
      return Key::F;
    case GLFW_KEY_G:
      return Key::G;
    case GLFW_KEY_H:
      return Key::H;
    case GLFW_KEY_I:
      return Key::I;
    case GLFW_KEY_J:
      return Key::J;
    case GLFW_KEY_K:
      return Key::K;
    case GLFW_KEY_L:
      return Key::L;
    case GLFW_KEY_M:
      return Key::M;
    case GLFW_KEY_N:
      return Key::N;
    case GLFW_KEY_O:
      return Key::O;
    case GLFW_KEY_P:
      return Key::P;
    case GLFW_KEY_Q:
      return Key::Q;
    case GLFW_KEY_R:
      return Key::R;
    case GLFW_KEY_S:
      return Key::S;
    case GLFW_KEY_T:
      return Key::T;
    case GLFW_KEY_U:
      return Key::U;
    case GLFW_KEY_V:
      return Key::V;
    case GLFW_KEY_W:
      return Key::W;
    case GLFW_KEY_X:
      return Key::X;
    case GLFW_KEY_Y:
      return Key::Y;
    case GLFW_KEY_Z:
      return Key::Z;
    case GLFW_KEY_LEFT_BRACKET:
      return Key::LeftBracket;
    case GLFW_KEY_BACKSLASH:
      return Key::Backslash;
    case GLFW_KEY_RIGHT_BRACKET:
      return Key::RightBracket;
    case GLFW_KEY_GRAVE_ACCENT:
      return Key::Accent;
    case GLFW_KEY_ESCAPE:
      return Key::Escape;
    case GLFW_KEY_ENTER:
      return Key::Enter;
    case GLFW_KEY_TAB:
      return Key::Tab;
    case GLFW_KEY_BACKSPACE:
      return Key::Backspace;
    case GLFW_KEY_INSERT:
      return Key::Insert;
    case GLFW_KEY_DELETE:
      return Key::Delete;
    case GLFW_KEY_RIGHT:
      return Key::RightArrow;
    case GLFW_KEY_LEFT:
      return Key::LeftArrow;
    case GLFW_KEY_DOWN:
      return Key::DownArrow;
    case GLFW_KEY_UP:
      return Key::UpArrow;
    case GLFW_KEY_CAPS_LOCK:
      return Key::CapsLock;
    case GLFW_KEY_F1:
      return Key::F1;
    case GLFW_KEY_F2:
      return Key::F2;
    case GLFW_KEY_F3:
      return Key::F3;
    case GLFW_KEY_F4:
      return Key::F4;
    case GLFW_KEY_F5:
      return Key::F5;
    case GLFW_KEY_F6:
      return Key::F6;
    case GLFW_KEY_F7:
      return Key::F7;
    case GLFW_KEY_F8:
      return Key::F8;
    case GLFW_KEY_F9:
      return Key::F9;
    case GLFW_KEY_F10:
      return Key::F10;
    case GLFW_KEY_F11:
      return Key::F11;
    case GLFW_KEY_F12:
      return Key::F12;
    case GLFW_KEY_F13:
      return Key::F13;
    case GLFW_KEY_F14:
      return Key::F14;
    case GLFW_KEY_F15:
      return Key::F15;
    case GLFW_KEY_F16:
      return Key::F16;
    case GLFW_KEY_F17:
      return Key::F17;
    case GLFW_KEY_F18:
      return Key::F18;
    case GLFW_KEY_F19:
      return Key::F19;
    case GLFW_KEY_F20:
      return Key::F20;
    case GLFW_KEY_LEFT_SHIFT:
      return Key::LeftShift;
    case GLFW_KEY_LEFT_CONTROL:
      return Key::LeftControl;
    case GLFW_KEY_LEFT_ALT:
      return Key::LeftAlt;
    case GLFW_KEY_LEFT_SUPER:
      return Key::LeftSuper;
    case GLFW_KEY_RIGHT_SHIFT:
      return Key::RightShift;
    case GLFW_KEY_RIGHT_CONTROL:
      return Key::RightControl;
    case GLFW_KEY_RIGHT_ALT:
      return Key::RightAlt;
    case GLFW_KEY_RIGHT_SUPER:
      return Key::RightSuper;
    case GLFW_MOUSE_BUTTON_LEFT:
      return Key::MouseLeft;
    case GLFW_MOUSE_BUTTON_RIGHT:
      return Key::MouseRight;
    case GLFW_MOUSE_BUTTON_MIDDLE:
      return Key::MouseMiddle;
    case GLFW_MOUSE_BUTTON_4:
      return Key::Mouse1;
    case GLFW_MOUSE_BUTTON_5:
      return Key::Mouse2;
    case GLFW_MOUSE_BUTTON_6:
      return Key::Mouse3;
    case GLFW_MOUSE_BUTTON_7:
      return Key::Mouse4;
    case GLFW_MOUSE_BUTTON_8:
      return Key::Mouse5;
    default:
    case GLFW_KEY_UNKNOWN:
      VLOG(1) << "Detected umapped key " << value;
      return Key::UnknownKey;
  }
}
};  // namespace

class GLFWWindowController : public Window {
 public:
  GLFWWindowController(WindowConfig config)
      : init_(false),
        block_events_(false),
        exception_(Key::UnknownKey),
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

  void BlockEvents(bool status, Key exception) override {
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

  void* GetNativeWindow() override { return main_window_; }

  void UpdateWindowName(const std::string& name) override {
    glfwSetWindowTitle(main_window_, name.c_str());
    config_.name = name;
  }

  void UpdateVSync(bool enabled) override {
    glfwSwapInterval(enabled);
    config_.use_vsync = enabled;
  }

  const WindowConfig& GetConfig() const override { return config_; }

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
  Key exception_;
  GLFWwindow* main_window_;
  WindowConfig config_;
  Vec2i buffer_size;

  void InitCallbacks() {
    glfwSetKeyCallback(main_window_, OnKeyEventHandler);
    glfwSetMouseButtonCallback(main_window_, OnMouseButtonEventHandler);
    glfwSetCursorPosCallback(main_window_, OnMouseEventHandler);
    glfwSetWindowSizeCallback(main_window_, OnWindowResizeEventHandler);
  }
  static void OnKeyEventHandler(GLFWwindow* window,
                                int key,
                                int code,
                                int action,
                                int mode);
  static void OnMouseButtonEventHandler(GLFWwindow* window,
                                        int button,
                                        int action,
                                        int mods);
  static void HandleKey(GLFWwindow* window, int key, int action);
  static void OnMouseEventHandler(GLFWwindow* window, double xPos, double yPos);
  static void OnWindowResizeEventHandler(GLFWwindow* window,
                                         int width,
                                         int height);
};

void GLFWWindowController::OnKeyEventHandler(GLFWwindow* window,
                                             int key,
                                             int,
                                             int action,
                                             int) {
  HandleKey(window, key, action);
}

void GLFWWindowController::OnMouseButtonEventHandler(GLFWwindow* window,
                                                     int button,
                                                     int action,
                                                     int) {
  HandleKey(window, button, action);
}

void GLFWWindowController::HandleKey(GLFWwindow* window, int key, int action) {
  GLFWWindowController* master_window =
      static_cast<GLFWWindowController*>(glfwGetWindowUserPointer(window));

  KeyEvent key_event;
  key_event.code = ToMepKey(key);
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

void GLFWWindowController::OnMouseEventHandler(GLFWwindow* window,
                                               double xPos,
                                               double yPos) {
  GLFWWindowController* master_window =
      static_cast<GLFWWindowController*>(glfwGetWindowUserPointer(window));
  if (master_window->ShouldBlockEvent())
    return;
  MouseEvent mouse_event{static_cast<float>(xPos), static_cast<float>(yPos)};
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
  VLOG(5) << __FUNCTION__
          << ", buffer size changed [x: " << master_window->buffer_size.x_
          << ", y: " << master_window->buffer_size.y_ << "]";
  glViewport(0, 0, master_window->buffer_size.x_,
             master_window->buffer_size.y_);
  if (width > 0 && height > 0) {
    if (!master_window->GetView()) {
      LOG(ERROR) << "You did not bind view to window";
    } else {
      master_window->GetView()->UpdateAspectRation(
          master_window->GetAspectRation());
    }
  }
  Vec2i size{width, height};
  master_window->ForAllObservers(
      [&size](WindowObserver* obs) { obs->OnWindowResizeEvent(size); });
}

std::unique_ptr<Window> Window::GetInstance(WindowConfig config) {
  return std::make_unique<GLFWWindowController>(config);
}
}  // namespace mep
