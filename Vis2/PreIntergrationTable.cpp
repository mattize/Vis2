#include "PreIntegrationTable.h"

void PreIntegrationTable::defineUI() {

    static float volume_positions[4][4] = {
        {0.0f, 0.5f, 1.0f},
        {0.0f, 0.5f, 1.0f},
        {0.0f, 0.5f, 1.0f},
        {0.0f, 0.5f, 1.0f}
    };

    static int volume_values[4][4] = {
        {255, 128, 0},
        {255, 128, 0},
        {255, 128, 0},
        {255, 128, 0}
    };

    static float medium_positions[4][4] = {
        {0.0f, 0.5f, 1.0f},
        {0.0f, 0.5f, 1.0f},
        {0.0f, 0.5f, 1.0f},
        {0.0f, 0.5f, 1.0f}
    };

    static int medium_values[4][4] = {
        {255, 128, 0},
        {255, 128, 0},
        {255, 128, 0},
        {255, 128, 0}
    };

    static float refraction_positions[4][4] = {
        {0.0f, 0.5f, 1.0f},
        {0.0f, 0.5f, 1.0f},
        {0.0f, 0.5f, 1.0f},
        {0.0f, 0.5f, 1.0f}
    };

    static int refraction_values[4][4] = {
        {255, 128, 0},
        {255, 128, 0},
        {255, 128, 0},
        {255, 128, 0}
    };

    float currentHeight = 0;
    float fixedWidth = 230.0f; 
    ImGui::SetNextWindowSizeConstraints(ImVec2(fixedWidth, 0), ImVec2(fixedWidth, FLT_MAX));
    ImGui::SetNextWindowSize(ImVec2(fixedWidth, 0)); // Set an initial size for the parent container
    ImGui::SetNextWindowPos(ImVec2(0, 0)); // Position the parent container

    if (ImGui::Begin("Transfer Function Editor", NULL, ImGuiWindowFlags_NoResize)) {

        // First Transfer Function (Collapsible)
        if (ImGui::CollapsingHeader("Volume TF")) {
            for (int i = 0; i < channels.size(); i++) {
                ImGui::Indent();
                ImGui::PushID(("Volume " + channels[i] + " Channel").c_str());

                std::string headerLabel = channels[i] + " Channel";

                if (ImGui::CollapsingHeader(headerLabel.c_str())) {
                    for (int point = 0; point < CONTROL_POINTS_COUNT; ++point) {
                        ImGui::PushID(("VolumeTF_" + std::to_string(i) + "_" + std::to_string(point)).c_str());

                        ImGui::PushItemWidth(100.0f);
                        ImGui::SliderFloat(("Position##" + std::to_string(point)).c_str(), &volume_positions[i][point], 0.0f, 1.0f, "%.2f");
                        ImGui::SliderInt(("Value##" + std::to_string(point)).c_str(), &volume_values[i][point], 0, 255);

                        ImGui::PopItemWidth();
                        ImGui::PopID();
                    }
                }

                ImGui::PopID();
                ImGui::Unindent();
            }
        }

        ImGui::Separator(); // Visual separation between the two transfer functions

        // Second Transfer Function (Collapsible)
        if (ImGui::CollapsingHeader("Medium TF")) {
            for (int i = 0; i < channels.size(); i++) {
                ImGui::Indent();
                ImGui::PushID(("Medium " + channels[i] + " Channel").c_str());

                std::string headerLabel = channels[i] + " Channel";

                if (ImGui::CollapsingHeader(headerLabel.c_str())) {
                    for (int point = 0; point < CONTROL_POINTS_COUNT; ++point) {
                        ImGui::PushID(("MediumTF_" + std::to_string(i) + "_" + std::to_string(point)).c_str());

                        ImGui::PushItemWidth(100.0f);
                        ImGui::SliderFloat(("Position##" + std::to_string(point)).c_str(), &medium_positions[i][point], 0.0f, 1.0f, "%.2f");
                        ImGui::SliderInt(("Value##" + std::to_string(point)).c_str(), &medium_values[i][point], 0, 255);

                        ImGui::PopItemWidth();
                        ImGui::PopID();
                    }
                }

                ImGui::PopID();
                ImGui::Unindent();
            }
        }

        ImGui::Separator(); // Visual separation between the two transfer functions

        // Second Transfer Function (Collapsible)
        if (ImGui::CollapsingHeader("Refraction TF")) {
            for (int i = 0; i < channels.size(); i++) {
                ImGui::Indent();
                ImGui::PushID(("Refraction " + channels[i] + " Channel").c_str());

                std::string headerLabel = channels[i] + " Channel";

                if (ImGui::CollapsingHeader(headerLabel.c_str())) {
                    for (int point = 0; point < CONTROL_POINTS_COUNT; ++point) {
                        ImGui::PushID(("RefractionTF_" + std::to_string(i) + "_" + std::to_string(point)).c_str());

                        ImGui::PushItemWidth(100.0f);
                        ImGui::SliderFloat(("Position##" + std::to_string(point)).c_str(), &refraction_positions[i][point], 0.0f, 1.0f, "%.2f");
                        ImGui::SliderInt(("Value##" + std::to_string(point)).c_str(), &refraction_values[i][point], 0, 255);

                        ImGui::PopItemWidth();
                        ImGui::PopID();
                    }
                }

                ImGui::PopID();
                ImGui::Unindent();
            }
        }
    }
    ImGui::End();
}