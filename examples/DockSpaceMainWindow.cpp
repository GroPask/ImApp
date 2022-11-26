#include <ImApp/ImApp.hpp>

#include <imgui_internal.h>

int main()
{
    ImApp::SetNextMainWindowSize(800, 600, ImApp::Cond::FirstUseEver);

    return ImApp::Run("ImApp_Example_DockSpaceMainWindow", [&]()
    {
		bool open = true;

        if (ImApp::BeginMainWindowContent(ImApp::MainWindowContentFlag::MenuBar))
        {
			const ImGuiID dockspaceId = ImGui::GetID("MyDockSpace");

			if (ImGui::DockBuilderGetNode(dockspaceId) == NULL)
			{
				ImGui::DockBuilderRemoveNode(dockspaceId);
				ImGui::DockBuilderAddNode(dockspaceId, ImGuiDockNodeFlags_DockSpace);
				ImGui::DockBuilderSetNodeSize(dockspaceId, ImGui::GetContentRegionAvail());

				ImGuiID dockMainId = dockspaceId;
				const ImGuiID dockRightId = ImGui::DockBuilderSplitNode(dockMainId, ImGuiDir_Right, 0.20f, NULL, &dockMainId);
				const ImGuiID dockBottomId = ImGui::DockBuilderSplitNode(dockMainId, ImGuiDir_Down, 0.30f, NULL, &dockMainId);
				const ImGuiID dockLeftId = ImGui::DockBuilderSplitNode(dockMainId, ImGuiDir_Left, 0.20f, NULL, &dockMainId);

				ImGui::DockBuilderDockWindow("Main", dockMainId);
				ImGui::DockBuilderDockWindow("Right", dockRightId);
				ImGui::DockBuilderDockWindow("Bottom", dockBottomId);
				ImGui::DockBuilderDockWindow("Left", dockLeftId);
				ImGui::DockBuilderFinish(dockspaceId);
			}

			ImGui::DockSpace(dockspaceId, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_None);

			if (ImGui::BeginMenuBar())
			{
				if (ImGui::BeginMenu("File"))
				{
					if (ImGui::MenuItem("Close"))
						open = false;

					ImGui::EndMenu();
				}

				ImGui::EndMenuBar();
			}
        }

        ImApp::EndMainWindowContent();

		ImGui::Begin("Main");
		ImGui::End();

		ImGui::Begin("Right");
		ImGui::End();

		ImGui::Begin("Bottom");
		ImGui::End();

		ImGui::Begin("Left");
		ImGui::End();

		return open;
    });
}
