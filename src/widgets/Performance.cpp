//
// Created by constexpr_dog on 3/2/21.
//

#include "Performance.h"
#include "imgui.h"

void DisplayPerformanceWindow() {
    ImGui::Begin("Performance", nullptr, ImGuiWindowFlags_NoResize);
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::End();
}
