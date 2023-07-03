#include "App.h"
#include <memory>
#include <algorithm>
#include "NastihanMath.h"
#include "Surface.h"
#include "imgui/imgui.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Vertex.h"
#include "TestPlane.h"
#include "NormalMapTwerker.h"
#include "NastihanUtil.h"
#include "Material.h"



App::App(const std::string& commandLine)
	:
	commandLine(commandLine),
	wnd(1600, 900, "DX11"),
	scriptCommander(TokenizeQuoted(commandLine)),
	light(wnd.Gfx())
{
	wnd.Gfx().SetProjection(DirectX::XMMatrixPerspectiveLH(1.0f, 9.0f / 16.0f, 0.5f, 200.0f));
	cube1.SetPos(DirectX::XMFLOAT3{2.0f, 1.5f, 1.0f});
	cube2.SetPos(DirectX::XMFLOAT3{5.0f, 4.0f, 0.0f});

	{
		std::string path = "Models\\brick_wall\\brick_wall.obj";
		Assimp::Importer imp;
		const auto pScene = imp.ReadFile(path,
			aiProcess_Triangulate |
			aiProcess_JoinIdenticalVertices |
			aiProcess_ConvertToLeftHanded |
			aiProcess_GenNormals |
			aiProcess_CalcTangentSpace
		);
		Material mat{ wnd.Gfx(),*pScene->mMaterials[1],path };
		pLoaded = std::make_unique<Mesh>(wnd.Gfx(), mat, *pScene->mMeshes[0]);
	}

}

void App::DoFrame()
{
	const auto dt = timer.Mark() * speedFactor;
	wnd.Gfx().BeginFrame();
	wnd.Gfx().SetCamera(cam.GetMatrix());
	light.Bind(wnd.Gfx(),cam.GetMatrix());


	//cube1.Submit(fc);
	//cube2.Submit(fc);
	light.Submit(fc);

	pLoaded->Submit(fc, DirectX::XMMatrixIdentity());
	//sponza.Draw(wnd.Gfx());

	fc.Execute(wnd.Gfx());


	//light.Submit(wnd.Gfx());

	while (const auto e = wnd.kbd.ReadKey())
	{
		if (e->IsPress() && e->GetCode() == VK_SPACE)
		{
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
		}
	}
	
	if (!wnd.CursorEnabled())
	{
		if (wnd.kbd.KeyIsPressed('W'))
		{
			cam.Translate({ 0.0f,0.0f,dt });
		}
		if (wnd.kbd.KeyIsPressed('A'))
		{
			cam.Translate({ -dt,0.0f,0.0f });
		}
		if (wnd.kbd.KeyIsPressed('S'))
		{
			cam.Translate({ 0.0f,0.0f,-dt });
		}
		if (wnd.kbd.KeyIsPressed('D'))
		{
			cam.Translate({ dt,0.0f,0.0f });
		}
		if (wnd.kbd.KeyIsPressed('R'))
		{
			cam.Translate({ 0.0f,dt,0.0f });
		}
		if (wnd.kbd.KeyIsPressed('F'))
		{
			cam.Translate({ 0.0f,-dt,0.0f });
		}
	}

	while (const auto delta = wnd.mouse.ReadRawDelta())
	{
		if (!wnd.CursorEnabled())
		{
			cam.Rotate((float)delta->x, (float)delta->y);
		}
	}


	// imgui windows
	cam.SpawnControlWindow();
	light.SpawnControlWindow();
	ShowHelperWindow();
	ShowFPSWindow();
	cube1.SpawnControlWindow(wnd.Gfx(), "cube1");
	cube2.SpawnControlWindow(wnd.Gfx(), "cube2");


	//sponza.ShowWindow(wnd.Gfx());
	// present
	wnd.Gfx().EndFrame();
	fc.Reset();
}

void App::ShowHelperWindow()
{
	if (ImGui::Begin("How to use"))
	{
		ImGui::BulletText("press the SPACE bar to toggle between free-look mode and default mode");
	}
	ImGui::End();
}

void App::ShowFPSWindow()
{
	if (ImGui::Begin("FPS"))
	{
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
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