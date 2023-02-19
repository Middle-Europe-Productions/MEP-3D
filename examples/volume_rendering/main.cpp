#include <MEP-3D/mep-3d.hpp>

#include <imgui.h>

#include <MEP-3D/template/ui_element.hpp>
#include <MEP-3D/template/ui_handlers_impl.hpp>

#include <MEP-3D/common.hpp>

namespace {
constexpr char kSceneName[] = "main-scene";

const char* kVertexShader = R"(
#version 330 core

layout(location = 0) in vec3 pos;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

smooth out vec3 tex_coord;

void main()
{
	gl_Position = projection*view*model*vec4(pos.xyz,1);
	tex_coord = pos + vec3(0.5);
}
)";

const char* kFragmentShader = R"(
#version 330 core

layout(location = 0) out vec4 vFragColor;	//fragment shader output

smooth in vec3 tex_coord;				//3D texture coordinates form vertex shader
								                //interpolated by rasterizer

//uniforms
uniform sampler3D	volume;		  //volume dataset
uniform vec3		eye_position;	//camera position
uniform vec3		step_size;	  //ray step size

//constants
const int MAX_SAMPLES = 200;	//total samples for each ray march step
const vec3 texMin = vec3(0);	//minimum texture access coordinate
const vec3 texMax = vec3(1);	//maximum texture access coordinate

void main()
{
	vec3 dataPos = tex_coord;
	vec3 geomDir = normalize((tex_coord-vec3(0.5)) - eye_position);
	vec3 dirStep = geomDir * step_size;
	bool stop = false;

	for (int i = 0; i < MAX_SAMPLES; i++) {
		dataPos = dataPos + dirStep;

		stop = dot(sign(dataPos-texMin),sign(texMax-dataPos)) < 3.0;

		if (stop)
			break;

		float sample = texture(volume, dataPos).r;

		float prev_alpha = sample - (sample * vFragColor.a);
		vFragColor.rgb = prev_alpha * vec3(sample) + vFragColor.rgb;
		vFragColor.a += prev_alpha;

		if(vFragColor.a>0.99)
			break;
	}
}
)";

constexpr char kMyRuntimeConfig[] = R"({
  "window": [
    {
      "name": "Volume Rendering",
      "return": {
        "scene": [
          {
            "name": "Volumes",
            "return": [
              {
              "name": "Volumes",
              "return": "draw_volume"
              },
              {
              "name": "Volume Controller",
              "return": "test"
              },
              {
              "name": "System Info",
              "return": "system_info"
              },
              {
              "name": "Cameras",
              "return": "draw_camera"
              },
              {
              "name":"Window info",
              "return": "draw_window"
              }
            ]
          }
        ],
        "popup": [
          {
            "return": "my_popup"
          }
        ]
      }
    }
  ]
})";

}  // namespace

class VolumeRenderer final : public Scene {
 public:
  VolumeRenderer() : Scene("volume_renderer") {}
  void OnAttach() override {
    auto& window = GetEngine()->GetWindow();
    if (!window) {
      LOG(ERROR) << "Window does not exist";
      return;
    }
    // Camera config
    view_ = std::make_unique<PerspectiveView>(PerspectiveView::Config(
        {glm::radians(45.0f), window->GetAspectRation(), 0.1f, 10000.0f}));
    auto config = PerspectiveCameraConfig::Create();
    config.start_position = {5.0, 3.0, -5.0};
    config.start_yaw = 135;
    config.start_pitch = -17;
    auto camera = std::make_unique<PerspectiveCamera>(config);

    // Init window
    window->AddView(view_.get());
    window->AddCamera(camera.get());
    window->AddObserver(camera.get());
    // Shader setup
    auto shader = std::make_unique<Shader>("ray_casting");
    shader->CreateFromString(kVertexShader, kFragmentShader);
    shaders::CacheDefaultUnifroms(*shader);
    shader->Use();
    shader->SetUniform("step_size",
                       glm::vec3(1.0f / 256, 1.0f / 256, 1.0f / 256));
    shader->Stop();

    // Volume setup
    volume_ = std::make_unique<Volume>();
    volume_->LoadFromFile("skull_256x256x256_uint8.raw", {256, 256, 256},
                          Texture3D::Type::BYTE_8);
    volume_->Bind(shader.get());
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    Attach(std::move(camera), std::move(shader), std::move(volume_));
  }
  void OnDetach() override {}
  void OnUpdate(float time_delta) override { UpdateAll(time_delta); }
  void OnDraw(RenderTarget& render_target) override {
    glEnable(GL_BLEND);
    GetShaders()[0]->Use();
    DrawAll(render_target);
    GetShaders()[0]->Stop();
    glDisable(GL_BLEND);
  }

  virtual void OnReceive(const Identity& id, const std::string& message) {
    LOG(INFO) << "Message received: " << id.ToString() << ", " << message;
  }

 private:
  std::unique_ptr<Volume> volume_;
  std::unique_ptr<PerspectiveView> view_;
};

enum MyOwnEnum { Test = UI_ELEMENT_COUNT + 1, Popup };

UI_HANDLER(MyOwnEnum, Test, SceneUILayer) {
  if (ImGui::Button("Add Volume")) {
    LOG(INFO) << "Start";
    std::unique_ptr<Volume> volume = std::make_unique<Volume>();
    volume->LoadFromFile("beechnut_1024x1024x1546_uint16.raw",
                         {1024, 1024, 1546}, Texture3D::Type::BYTE_16);
    volume->Bind(GetContext()->GetScenePtr()->GetShaders()[0].get());
    GetContext()->GetScenePtr()->Attach(std::move(volume));
    LOG(INFO) << "Finish";
  }
};

UI_HANDLER(MyOwnEnum, Popup, SceneUILayer) {
  ImGui::Text("Hello");
};

int main(int argc, char* argv[]) {
  ADD_UI_ELEMENT(MyOwnEnum::Test, "test");
  ADD_UI_ELEMENT(MyOwnEnum::Popup, "my_popup");
  utils::Init(argc, argv);
  auto window = Window::GetInstance({{1280, 720}, "Volume Rendering"});
  window->Init();
  auto engine = utils::CreateEngineWithSceneUI(
      std::move(window), std::make_unique<VolumeRenderer>(),
      SceneUILayer::Create(kMyRuntimeConfig));
  engine->Run();
}
