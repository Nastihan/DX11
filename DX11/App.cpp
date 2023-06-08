#include "App.h"
#include "Box.h"
#include "Pyramid.h"
#include "AssimpTest.h"
#include "SkinnedBox.h"
#include <memory>
#include <algorithm>
#include "NastihanMath.h"
#include "Surface.h"
#include "GDIPlusManager.h"
#include "imgui/imgui.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Vertex.h"


GDIPlusManager gdipm;


App::App()
	:
	wnd(1600, 900, "DX11"),
	light(wnd.Gfx())
{
	


	wnd.Gfx().SetProjection(DirectX::XMMatrixPerspectiveLH(1.0f, 9.0f / 16.0f, 0.5f, 60.0f));
}

void App::DoFrame()
{
	const auto dt = timer.Mark() * speedFactor;
	wnd.Gfx().BeginFrame();
	wnd.Gfx().SetCamera(cam.GetMatrix());
	light.Bind(wnd.Gfx(),cam.GetMatrix());

	nano.Draw(wnd.Gfx(),(DirectX::XMMatrixRotationRollPitchYaw(model.pitch,model.yaw,model.roll) * DirectX::XMMatrixTranslation(model.x,model.y,model.z)));
	light.Draw(wnd.Gfx());


	// imgui windows
	SpawnSimulationWindow();
	cam.SpawnControlWindow();
	light.SpawnControlWindow();
	SpawnModelControlWindow();


	// present
	wnd.Gfx().EndFrame();
}

void App::SpawnSimulationWindow() noexcept
{
	if (ImGui::Begin("Simulation Speed"))
	{
		ImGui::SliderFloat("Speed Factor", &speedFactor, 0.0f, 6.0f, "%.4f", 3.2f);
		ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::Text("Status: %s", wnd.kbd.KeyIsPressed(VK_SPACE) ? "PAUSED" : "RUNNING (hold spacebar to pause)");
	}
	ImGui::End();
}

void App::SpawnModelControlWindow() noexcept
{
	if(ImGui::Begin("Model"))
	{
		ImGui::SliderAngle("roll", &model.roll, -180.0f, 180.0f);
		ImGui::SliderAngle("pitch", &model.pitch, -180.0f, 180.0f);
		ImGui::SliderAngle("yaw", &model.yaw, -180.0f, 180.0f);
		ImGui::SliderFloat("x", &model.x, -40.0f, 40.0f);
		ImGui::SliderFloat("y", &model.y, -40.0f, 40.0f);
		ImGui::SliderFloat("z", &model.z, -40.0f, 40.0f);

	}
	ImGui::End();
}



App::~App()
{}


int App::Go()
{
	while (true)
	{
		// process all messages pending, but to not block for new messages
		if (const auto ecode = Window::ProcessMessages())
		{
			// if return optional has value, means we're quitting so return exit code
			return *ecode;
		}
		DoFrame();
	}
}