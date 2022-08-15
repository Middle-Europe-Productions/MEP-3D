#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <GLFW/glfw3.h>
#include <glog/logging.h>
#include <imgui.h>
#include <MEP-3D/user_interface.hpp>

namespace UI {

void BeforeRender() {
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
}

void AfterRender() {
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
  ImGuiIO& io = ImGui::GetIO(); (void)io;
  if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
  {
      GLFWwindow* backup_current_context = glfwGetCurrentContext();
      ImGui::UpdatePlatformWindows();
      ImGui::RenderPlatformWindowsDefault();
      glfwMakeContextCurrent(backup_current_context);
  }
}

}  // namespace UI