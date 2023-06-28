#pragma once
#include "Technique.h"
#include "ImGui/imgui.h"

class Probe
{
public:
	void SetTechnique(Technique& technique)
	{
		pTechnique = &technique;
	}
	void SetStep(Step& step)
	{
		pStep = &step;
	}
	void DoSth()
	{
		if (ImGui::Begin("test"))
		{
			ImGui::Text("this is a test");
		}
		ImGui::End();
	}
private:
	Technique* pTechnique;
	Step* pStep;
};