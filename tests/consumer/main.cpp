#include <imgui.h>
#include <imgui_node_editor.h>
#include <TextEditor.h>
#include <string>
#include <cstring>
int main() {
    ImGui::CreateContext();
    ax::NodeEditor::Config config;
    std::string saved;
    config.SettingsFile = nullptr;
    config.UserPointer = &saved;
    config.SaveSettings = [](const char* data, size_t size, ax::NodeEditor::SaveReasonFlags, void* user) {
        static_cast<std::string*>(user)->assign(data, size);
        return true;
    };
    config.LoadSettings = [](char* data, void* user) {
        const auto& value = *static_cast<std::string*>(user);
        if (data) std::memcpy(data, value.data(), value.size());
        return value.size();
    };
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
    ax::NodeEditor::SetNodePosition(1, ImVec2(12.0f, 34.0f));
    ax::NodeEditor::SelectNode(1);
    frame();
    ax::NodeEditor::SetCurrentEditor(nullptr);
    ax::NodeEditor::DestroyEditor(editor);
    editor = ax::NodeEditor::CreateEditor(&config);
    ax::NodeEditor::SetCurrentEditor(editor);
    frame();
    frame();
    const auto restored = ax::NodeEditor::GetNodePosition(1);
    bool valid = restored.x == 12.0f && restored.y == 34.0f && ax::NodeEditor::IsNodeSelected(1);
    TextEditor text;
    text.SetText("exported consumer");
    valid = valid && !text.GetText().empty() && ax::NodeEditor::GetSelectedNodes().size() == 1;
    ax::NodeEditor::DestroyEditor(editor);
    ImGui::DestroyContext();
    return valid ? 0 : 1;
}
