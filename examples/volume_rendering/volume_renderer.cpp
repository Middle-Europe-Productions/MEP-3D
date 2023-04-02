#include "volume_renderer.hpp"
#include "common.hpp"

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

layout(location = 0) out vec4 FragColor;	//fragment shader output

smooth in vec3 tex_coord;				//3D texture coordinates form vertex shader
								                //interpolated by rasterizer

//uniforms
uniform sampler3D	volume;		              //volume dataset
uniform sampler2D	transfer_function;		  //transfer function
uniform vec3		eye_position;	            //camera position
uniform vec3		step_size;	              //ray step size

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
  FragColor = vec4(0.0, 0.0, 0.0, 0.0);

	for (int i = 0; i < MAX_SAMPLES; i++) {
		dataPos = dataPos + dirStep;

    // Ray termination: Test if outside volume ...
    vec3 temp1 = sign(dataPos - texMin);
    vec3 temp2 = sign(texMax - dataPos);
    float inside = dot(temp1, temp2);
    // ... and exit loop
    if (inside < 3.0)
        break;

		float sample = texture(volume, dataPos).r;

    vec4 src = texture(transfer_function, vec2(sample, 0));

    FragColor = (1.0 - FragColor.a) * src + FragColor;

		if(FragColor.a>0.95)
			break;
	}
}
)";
}  // namespace

VolumeRenderer::VolumeRenderer() : Scene(vr::kVolumeRenderer) {}

void VolumeRenderer::OnAttach() {
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
  // Arcball camera
  auto arcball_config = ArcballCameraConfig::Create();
  arcball_config.start_position = glm::vec3(1.0f, 1.0f, -5.0f);
  auto arcball_camera = std::make_unique<ArcballCamera>(arcball_config);

  // Init window
  window->AddView(view_.get());
  window->AddCamera(camera.get());
  window->AddObserver(camera.get());
  window->AddObserver(arcball_camera.get());
  // Shader setup
  auto shader = std::make_unique<Shader>("ray_casting");
  shader->CreateFromString(kVertexShader, kFragmentShader);
  shaders::CacheDefaultUnifroms(*shader);

  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  Attach(std::move(camera), std::move(shader), std::move(arcball_camera));
}

void VolumeRenderer::OnDetach() {}

void VolumeRenderer::OnUpdate(float time_delta) {
  UpdateAll(time_delta);
}

void VolumeRenderer::OnDraw(RenderTarget& render_target) {
  glEnable(GL_BLEND);
  GetShaders()[0]->Use();
  transfer_function_texture_.Use(2);
  // TODO: Add shader observer
  GetShaders()[0]->SetUniform("step_size",
                              glm::vec3(1.0f / 256, 1.0f / 256, 1.0f / 256));
  GetShaders()[0]->SetUniform("transfer_function",
                              (int)transfer_function_texture_.GetHandler());
  DrawAll(render_target);
  transfer_function_texture_.Stop();
  GetShaders()[0]->Stop();
  glDisable(GL_BLEND);
}

void VolumeRenderer::OnReceive(const IdentityView& id,
                               const nlohmann::json& message) {
  LOG(INFO) << "Message received: " << id.ToString() << ", " << message;
  if (message.contains(vr::kTextureNode) &&
      message.contains(vr::kTextureAction)) {
    if (message[vr::kTextureAction] == vr::kTextureCreate) {
      transfer_function_texture_.SetHandler(
          static_cast<unsigned int>(message[vr::kTextureNode]));
    } else {
      transfer_function_texture_.Clear();
    }
  }
}