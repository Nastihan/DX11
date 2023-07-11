#pragma once
#include "Camera.h"
#include <memory>
#include "ImGui/imgui.h"

class CameraContainer
{
public:
    CameraContainer() = default;
    void Add(std::unique_ptr<Camera> pCam)
    {
        cameras.push_back(std::move(pCam));
    }
    Camera& GetActiveCam()
    {
        return *cameras.at(activeIndex);
    }
    void SwitchCam()
    {
        if (ImGui::Begin("test"))
        {
            static int activeCameraIndex = 0; 
            if (ImGui::BeginCombo("Cam", cameras[activeCameraIndex]->GetName().c_str()))
            {
                for (int i = 0; i < cameras.size(); i++)
                {
                    bool isSelected = (activeCameraIndex == i);
                    if (ImGui::Selectable(cameras[i]->GetName().c_str(), isSelected))
                    {
                        activeCameraIndex = i;
                        activeIndex = i; 
                    }
                }

                ImGui::EndCombo();
            }
        }
        ImGui::End();
    }

private:
    std::vector<std::unique_ptr<Camera>> cameras;
    int activeIndex = 0; 
};