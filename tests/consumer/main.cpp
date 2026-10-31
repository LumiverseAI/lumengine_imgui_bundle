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
    auto& io = ImGui::GetIO();
    io.IniFilename = nullptr;
    io.DisplaySize = ImVec2(800, 600);
    unsigned char* pixels;
    int width, height;
    io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);
    auto frame = [] {
        ImGui::NewFrame();
        ImGui::SetNextWindowSize(ImVec2(700, 500));
        ImGui::Begin("Consumer");
        ax::NodeEditor::Begin("Canvas");
        ax::NodeEditor::BeginNode(1);
        ImGui::TextUnformatted("Node");
        ax::NodeEditor::EndNode();
        ax::NodeEditor::End();
        ImGui::End();
        ImGui::Render();
    };
    frame();
    frame();
    frame();
    ax::NodeEditor::NodeState state{};
    state.Id = 1;
    state.Position = ImVec2(12.0f, 34.0f);
    ax::NodeEditor::SetNodeState(state);
    frame();
    std::string saved = ax::NodeEditor::SaveSettingsToMemory();
    ax::NodeEditor::ClearState();
    ax::NodeEditor::LoadSettingsFromMemory(saved.c_str());
    frame();
    ax::NodeEditor::NodeState restored{};
    bool valid = ax::NodeEditor::GetNodeState(1, &restored) && restored.Position.x == 12.0f;
    TextEditor text;
    text.SetText("exported consumer");
    valid = valid && !text.GetText().empty() && ax::NodeEditor::GetSelectedNodes().empty();
    ax::NodeEditor::DestroyEditor(editor);
    ImGui::DestroyContext();
    return valid ? 0 : 1;
}
