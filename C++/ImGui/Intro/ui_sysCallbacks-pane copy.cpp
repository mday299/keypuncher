#include "ui_sysCallbacks-pane.hpp"

// UI Core
#include <ui_core/src/app.h>

// Extern ImGui includes
#include <imgui-docking/imgui.h>
//#include <implot.h>
#include <misc/cpp/imgui_stdlib.h>

namespace ui_sysCallbacks {
struct TestItem
{
    int id;
    std::string name;
    bool enabled;
};

void RefreshData() {
    ImGui::Text("Refresh pushed \n");
}

bool sysCallbacksPane::Draw(App& app)
{
    ImGui::Text("! Autogen systemCallbacks !");

    static int clicked = 0;
    if (ImGui::Button("Button"))
        clicked++;
    if (clicked & 1) {
        ImGui::SameLine();
        ImGui::Text("Thanks for clicking me!");
    }

    static int refreshed = 0;
    if (ImGui::Button("Refresh")) {
        RefreshData();
        refreshed++;
    }    
    if (refreshed & 1) {
        ImGui::SameLine();
        ImGui::Text("Refresh toggled");
    }

    ImGui::Separator();

    ImGui::Dummy(ImVec2(0.0f, 20.0f)); //20 pixel high Dummy space
    ImGui::Spacing();

    static char buf1[32] = ""; 
    ImGui::InputText("InputText", buf1, std::size(buf1));

    static char buf2[32] = ""; ImGui::InputText("decimal", buf2, std::size(buf2), ImGuiInputTextFlags_CharsDecimal);
    static char buf3[32] = ""; ImGui::InputText("hexadecimal", buf3, std::size(buf3), ImGuiInputTextFlags_CharsHexadecimal | ImGuiInputTextFlags_CharsUppercase);
    static char buf4[32] = ""; ImGui::InputText("uppercase", buf4, std::size(buf4), ImGuiInputTextFlags_CharsUppercase);
    static char buf5[32] = ""; ImGui::InputText("no blank", buf5, std::size(buf5), ImGuiInputTextFlags_CharsNoBlank);

    static int testInt = -1; ImGui::InputInt("testInt", &testInt);

    /* Now try a table: ---------- */ 
    static bool editing = false;
    static std::vector<TestItem> committedItems =
    {
        { 1, "Alpha", true },
        { 2, "Beta", false },
        { 3, "Gamma", true }, 
        { 4, "foo", true },
        { 5, "foo", true },
        { 6, "foo", true },
        { 7, "foo", true },
        { 8, "foo", true },
        { 9, "foo", true }
    };
    static std::vector<TestItem> editedItems;

    if (!editing) {
        if (ImGui::Button("Edit")) {
            editedItems = committedItems;
            editing = true;
        }
    }

    std::vector<TestItem>& displayedItems = editing ? editedItems : committedItems;

    // Remember that ImGui uses an immediate-mode UI model.
    //
    // So 'if (ImGui::BeginTable(...' might return false if: 
    //      Clipped / not visible
    //      Not enough space to render
    // Matters much more with a table with thousands of entries!
    if (ImGui::BeginTable(
            "TestTable",
            3,  //num cols
            ImGuiTableFlags_Borders |
            ImGuiTableFlags_RowBg |
            ImGuiTableFlags_Resizable | 
            ImGuiTableFlags_ScrollY, ImVec2(0, 200) //first arg = locked columns,
                                                    //second arg = locked rows;
                                                    //defines scroll area in pixels
        )) {

        // Freeze the first row (the header)
        ImGui::TableSetupScrollFreeze(0, 1);    

        //Define column headers
        ImGui::TableSetupColumn("ID",
                                ImGuiTableColumnFlags_WidthFixed,
                                80.0f); //width in Pixels

        ImGui::TableSetupColumn("Name",
                                ImGuiTableColumnFlags_WidthStretch);

        ImGui::TableSetupColumn("Enabled");

        //Draw column headers
        ImGui::TableHeadersRow();

        for (auto& item : displayedItems) {
            //Explain why this is needed!!
            ImGui::PushID(item.id);    

            //Next row:
            ImGui::TableNextRow();

            ImGui::TableNextColumn();
            ImGui::Text("%d", item.id);

            ImGui::TableNextColumn();
            if (editing) {
                ImGui::InputText("##name", &item.name);
            }
            else {
                ImGui::Text("%s", item.name.c_str());
            }

            ImGui::TableNextColumn();
            ImGui::Text("%s", item.enabled ? "Yes" : "No");

            ImGui::PopID();
        }

        ImGui::EndTable();
        /* End table -------------- */
    } 
    // REAL END OF TABLE

    if (editing) {
        if (ImGui::Button("OK")) {
            committedItems = editedItems;
            editing = false;
        }

        ImGui::SameLine();

        if (ImGui::Button("Cancel")) {
            editing = false;
        }
    }   
 
    return true;
}
} // namespace ui_sysCallbacks
