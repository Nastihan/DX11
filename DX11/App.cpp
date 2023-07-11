#include "App.h"
#include <algorithm>
#include "NastihanMath.h"
#include "imgui/imgui.h"
#include "NastihanUtil.h"
#include "Testing.h"
#include "PerfLog.h"
#include "TestModelProbe.h"

namespace dx = DirectX;

App::App(const std::string& commandLine) :
	commandLine(commandLine),
	wnd(1600, 900, "DX11"),
	scriptCommander(TokenizeQuoted(commandLine)),
	light(wnd.Gfx())
{
	cameras.Add(std::make_unique<Camera>(DirectX::XMFLOAT3{ -13.5f, 6.0f, 3.5f }, 0.0f, PI / 2.0f, "First"));
	cameras.Add(std::make_unique<Camera>(DirectX::XMFLOAT3{10.0f, 2.0f, -1.0f}, -0.3f, -0.8 , "Second" ));

	cube.SetPos({ 2.0f,2.5f,0.0f });
	cube2.SetPos({ 0.0f,4.0f,3.0f });

	nano.SetRootTransform(
		dx::XMMatrixRotationY(PI / 2.f) *
		dx::XMMatrixTranslation(27.f, -0.56f, 1.7f)
	);
	gobber.SetRootTransform(
		dx::XMMatrixRotationY(-PI / 2.f) *
		dx::XMMatrixTranslation(-8.f, 10.f, 0.f)
	);

	cube.LinkTechniques(rg);
	cube2.LinkTechniques(rg);
	light.LinkTechniques(rg);
	sponza.LinkTechniques(rg);
	gobber.LinkTechniques(rg);
	nano.LinkTechniques(rg);

	wnd.Gfx().SetProjection(dx::XMMatrixPerspectiveLH(1.0f, 9.0f / 16.0f, 0.5f, 400.0f));
}

void App::HandleInput(float dt)
{
	while (const auto e = wnd.kbd.ReadKey())
	{
		if (!e->IsPress())
		{
			continue;
		}

		switch (e->GetCode())
		{
		case VK_SPACE:
			if (wnd.CursorEnabled())
			{
				wnd.DisableCursor();
				wnd.mouse.EnableRaw();
			}
			else
			{
				wnd.EnableCursor();
				wnd.mouse.DisableRaw();
			}
			break;
		case VK_F1:
			showDemoWindow = true;
			break;
		}
	}

	if (!wnd.CursorEnabled())
	{
		if (wnd.kbd.KeyIsPressed('W'))
		{
			cameras.GetActiveCam().Translate({0.0f,0.0f,dt});
		}
		if (wnd.kbd.KeyIsPressed('A'))
		{
			cameras.GetActiveCam().Translate({ -dt,0.0f,0.0f });
		}
		if (wnd.kbd.KeyIsPressed('S'))
		{
			cameras.GetActiveCam().Translate({ 0.0f,0.0f,-dt });
		}
		if (wnd.kbd.KeyIsPressed('D'))
		{
			cameras.GetActiveCam().Translate({ dt,0.0f,0.0f });
		}
		if (wnd.kbd.KeyIsPressed('R'))
		{
			cameras.GetActiveCam().Translate({ 0.0f,dt,0.0f });
		}
		if (wnd.kbd.KeyIsPressed('F'))
		{
			cameras.GetActiveCam().Translate({ 0.0f,-dt,0.0f });
		}
	}

	while (const auto delta = wnd.mouse.ReadRawDelta())
	{
		if (!wnd.CursorEnabled())
		{
			cameras.GetActiveCam().Rotate((float)delta->x, (float)delta->y);
		}
	}
}

void App::DoFrame(float dt)
{
	wnd.Gfx().BeginFrame(0.07f, 0.0f, 0.12f);
	wnd.Gfx().SetCamera(cameras.GetActiveCam().GetMatrix());
	light.Bind(wnd.Gfx(), cameras.GetActiveCam().GetMatrix());

	light.Submit();
	cube.Submit();
	sponza.Submit();
	cube2.Submit();
	gobber.Submit();
	nano.Submit();

	rg.Execute(wnd.Gfx());

	// imgui windows
	static MP sponzeProbe{ "Sponza" };
	static MP gobberProbe{ "Gobber" };
	static MP nanoProbe{ "Nano" };
	sponzeProbe.SpawnWindow(sponza);
	gobberProbe.SpawnWindow(gobber);
	nanoProbe.SpawnWindow(nano);
	cameras.GetActiveCam().SpawnControlWindow();
	cameras.SwitchCam();
	light.SpawnControlWindow();
	ShowImguiDemoWindow();
	cube.SpawnControlWindow(wnd.Gfx(), "Cube 1");
	cube2.SpawnControlWindow(wnd.Gfx(), "Cube 2");
	ShowFPSWindow();
	rg.RenderWidgets(wnd.Gfx());

	// present
	wnd.Gfx().EndFrame();
	rg.Reset();
}

void App::ShowImguiDemoWindow()
{
	if (showDemoWindow)
	{
		ImGui::ShowDemoWindow(&showDemoWindow);
	}
}

void App::ShowFPSWindow()
{
	if (ImGui::Begin("FPS"))
	{
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

		static std::vector<float> values;
		values.push_back(ImGui::GetIO().Framerate);

		ImGui::PlotLines("FPS Plot", values.data(), static_cast<int>(values.size()));
	}
	ImGui::End();
}

App::~App() {}

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
		// execute the game logic
		const auto dt = timer.Mark() * speed_factor;
		HandleInput(dt);
		DoFrame(dt);
	}
}
