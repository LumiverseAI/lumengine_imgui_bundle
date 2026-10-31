#include <imgui.h>
#include <imgui_node_editor.h>
#include <TextEditor.h>
#include <string>
int main() {
    ImGui::CreateContext();
    ax::NodeEditor::Config config;
    config.SettingsName = nullptr;
    auto* editor = ax::NodeEditor::CreateEditor(&config);
    ax::NodeEditor::SetCurrentEditor(editor);
    ax::NodeEditor::NodeState state{};
    state.Id = 1;
    state.Position = ImVec2(12.25f, 34.5f);
    ax::NodeEditor::SetNodeState(state);
    std::string saved = ax::NodeEditor::SaveSettingsToMemory();
    ax::NodeEditor::ClearState();
    ax::NodeEditor::LoadSettingsFromMemory(saved.c_str());
    ax::NodeEditor::NodeState restored{};
    bool valid = ax::NodeEditor::GetNodeState(1, &restored) && restored.Position.x == 12.25f;
    TextEditor text;
    text.SetText("exported consumer");
    valid = valid && !text.GetText().empty() && ax::NodeEditor::GetSelectedNodes().empty();
    ax::NodeEditor::DestroyEditor(editor);
    ImGui::DestroyContext();
    return valid ? 0 : 1;
}
