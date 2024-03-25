#include "ImGui_TilemapEditorWindow.h"
#include "Graphics/Texture.h"
#include "Asset/AssetManager.h"
#include "../../EditorLayer.h"

ImGui_TilemapEditorWindow::ImGui_TilemapEditorWindow(EditorLayer& editor)
	: ImGui_Window(editor, "Tilemap Editor")
{
	s_instance = this;
}

void ImGui_TilemapEditorWindow::Render()
{
    static ImVec2 scrolling(0.0f, 0.0f);
    static bool opt_enable_grid = true;
    static bool selecting = false;
    static ImVec2 startCell;
    static ImVec2 endCell;

	if (!m_isOpen)
		return;

    m_editor.SetCurrentMode(EditorMode::TilemapMode);

	if (ImGui::Begin("Tilemap Editor", &m_isOpen, ImGuiWindowFlags_NoScrollbar))
	{
        if (m_tilemap != nullptr && m_tilemap->originalImage != nullptr)
        {
			if (Texture* texture = m_tilemap->originalImage->GetTexture())
			{
                if (ImGui::IsWindowFocused())
                {
                    if (ImGui::GetIO().MouseWheel > 0.0f && m_zoomIndex < S_ZOOM_VALUES.size() - 1)
                    {
                        m_zoomIndex++;
                    }
                    else if (ImGui::GetIO().MouseWheel < 0.0f && m_zoomIndex > 0)
                    {
                        m_zoomIndex--;
                    }
                }

				ImGui::Text("size = %d x %d", texture->GetWidth(), texture->GetHeight());
                ImGui::Text("Zoom = %f", S_ZOOM_VALUES[m_zoomIndex]);

                ImGui::PushItemWidth(150);
                ImGui::InputInt("Rows:", &m_rows); ImGui::SameLine();
				ImGui::InputInt("Cols:", &m_cols);
                ImGui::PopItemWidth();

                if (ImGui::Button("Generate Sprites"))
                {
                    m_tilemap->tilemapSprites.clear();
                    m_tilemap->rows = m_rows;
                    m_tilemap->cols = m_cols;

                    m_tilemap->GenerateSprites();
                }

                ImGui::Checkbox("Enable grid", &opt_enable_grid);

                // Using InvisibleButton() as a convenience 1) it will advance the layout cursor and 2) allows us to use IsItemHovered()/IsItemActive()
                ImVec2 canvas_p0 = ImGui::GetCursorScreenPos();      // ImDrawList API uses screen coordinates!
                ImVec2 canvas_sz = ImGui::GetContentRegionAvail();   // Resize canvas to what's available
                if (canvas_sz.x < 50.0f) canvas_sz.x = 50.0f;
                if (canvas_sz.y < 50.0f) canvas_sz.y = 50.0f;
                ImVec2 canvas_p1 = ImVec2(canvas_p0.x + canvas_sz.x, canvas_p0.y + canvas_sz.y);

                // Draw border and background color
                ImGuiIO& io = ImGui::GetIO();
                ImDrawList* draw_list = ImGui::GetWindowDrawList();
                draw_list->AddRectFilled(canvas_p0, canvas_p1, IM_COL32(50, 50, 50, 255));

                // This will catch our interactions
                ImGui::InvisibleButton("canvas", canvas_sz, ImGuiButtonFlags_MouseButtonLeft | ImGuiButtonFlags_MouseButtonRight);
                const bool is_hovered = ImGui::IsItemHovered(); // Hovered
                const bool is_active = ImGui::IsItemActive();   // Held
                const ImVec2 origin(canvas_p0.x + scrolling.x, canvas_p0.y + scrolling.y); // Lock scrolled origin
                const ImVec2 mouse_pos_in_canvas(io.MousePos.x - origin.x, io.MousePos.y - origin.y);

                struct TextureRenderer {
                    float width;
                    float height;
                    float x = 0.0f;
                    float y = 0.0f;
                };

                TextureRenderer textureRenderer{
                    texture->GetWidth() * S_ZOOM_VALUES[m_zoomIndex],
                    texture->GetHeight() * S_ZOOM_VALUES[m_zoomIndex],
                    origin.x,
                    origin.y
                };

                ImVec2 pMin = canvas_p0;
                ImVec2 pMax = canvas_p1;

                pMin.x += scrolling.x;
                pMin.y += scrolling.y;
                pMax.x = pMin.x + textureRenderer.width;
                pMax.y = pMin.y + textureRenderer.height;

                if (pMin.x < canvas_p0.x)
                    pMin.x = canvas_p0.x;
                else if (pMin.x > canvas_p1.x)
                    pMin.x = canvas_p1.x;

                if (pMax.x > canvas_p1.x)
                    pMax.x = canvas_p1.x;
                else if (pMax.x < canvas_p0.x)
                    pMax.x = canvas_p0.x;

                if (pMin.y < canvas_p0.y)
                    pMin.y = canvas_p0.y;
                else if (pMin.y > canvas_p1.y)
                    pMin.y = canvas_p1.y;

                if (pMax.y > canvas_p1.y)
                    pMax.y = canvas_p1.y;
                else if (pMax.y < canvas_p0.y)
                    pMax.y = canvas_p0.y;

                ImVec2 uvMin = { 0, 1 };
                ImVec2 uvMax = { 1, 0 };

                if (scrolling.x < 0)
                {
                    uvMin.x = -scrolling.x / textureRenderer.width;

                    if (uvMin.x > 1.0f)
                        uvMin.x = 1.0f;
                }

                if (textureRenderer.width + scrolling.x > canvas_sz.x)
                {
                    float rightDiff = textureRenderer.width + scrolling.x - canvas_sz.x;
                    uvMax.x -= rightDiff / textureRenderer.width;

                    if (uvMax.x < 0.0f)
                        uvMax.x = 0.0f;
                }

                if (scrolling.y < 0)
                {
                    uvMin.y = 1 - (-scrolling.y / textureRenderer.height);

                    if (uvMin.y < 0.0f)
                        uvMin.y = 0.0f;
                }

                if (textureRenderer.height + scrolling.y > canvas_sz.y)
                {
                    float bottomDiff = textureRenderer.height + scrolling.y - canvas_sz.y;
                    uvMax.y += (bottomDiff / textureRenderer.height);

                    if (uvMax.y > 1.0f)
                        uvMax.y = 1.0f;
                }

                draw_list->AddImage((ImTextureID)texture->GetRendererID(), pMin, pMax, uvMin, uvMax);
                draw_list->AddRect(canvas_p0, canvas_p1, IM_COL32(255, 255, 255, 255));

                if (is_hovered && !selecting && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
                {
                    selecting = true;
                    startCell = mouse_pos_in_canvas;
                    endCell = mouse_pos_in_canvas;
                }

                if (selecting)
                {
                    endCell = mouse_pos_in_canvas;
                    if (!ImGui::IsMouseDown(ImGuiMouseButton_Left))
                        selecting = false;
                }

                if (is_active && ImGui::IsMouseDragging(ImGuiMouseButton_Right, 0.0f))
                {
                    scrolling.x += io.MouseDelta.x;
                    scrolling.y += io.MouseDelta.y;
                }

                draw_list->PushClipRect(canvas_p0, canvas_p1, true);

                //int usingRows = m_tilemapImage != nullptr ? m_tilemapImage->GetRows() : 1;
                //int usingCols = m_tilemapImage != nullptr ? m_tilemapImage->GetCols() : 1;

                const float GRID_ROW_STEP = textureRenderer.height / m_tilemap->rows;
                const float GRID_COL_STEP = textureRenderer.width / m_tilemap->cols;

                if (opt_enable_grid)
                {
                    for (float x = fmodf(scrolling.x, GRID_COL_STEP); x < canvas_sz.x; x += GRID_COL_STEP)
                        draw_list->AddLine(ImVec2(canvas_p0.x + x, canvas_p0.y), ImVec2(canvas_p0.x + x, canvas_p1.y), IM_COL32(200, 200, 200, 80));
                    for (float y = fmodf(scrolling.y, GRID_ROW_STEP); y < canvas_sz.y; y += GRID_ROW_STEP)
                        draw_list->AddLine(ImVec2(canvas_p0.x, canvas_p0.y + y), ImVec2(canvas_p1.x, canvas_p0.y + y), IM_COL32(200, 200, 200, 80));
                }

                int startRow = -1;
                int startCol = -1;

                int endRow = -1;
                int endCol = -1;

                if (startCell.x >= 0.0f && startCell.x < textureRenderer.width)
                {
                    startCol = (int)(startCell.x / GRID_COL_STEP);
                }

                if (endCell.x >= 0.0f && endCell.x < textureRenderer.width)
                {
                    endCol = endCell.x / GRID_COL_STEP;
                }


                if (startCell.y >= 0.0f && startCell.y < textureRenderer.height)
                {
                    startRow = startCell.y / GRID_ROW_STEP;
                }

                if (endCell.y >= 0.0f && endCell.y < textureRenderer.height)
                {
                    endRow = endCell.y / GRID_ROW_STEP;
                }

                if (startRow != -1 && startCol != -1 && endRow != -1 && endCol != -1)
                {
                    if (startCol > endCol)
                        std::swap(startCol, endCol);

                    if (startRow > endRow)
                        std::swap(startRow, endRow);

                    int numCols = endCol - startCol + 1;
                    int numRows = endRow - startRow + 1;

                    ImVec2 topLeft = ImVec2(origin.x + (startCol * GRID_COL_STEP), origin.y + (startRow * GRID_ROW_STEP));
                    ImVec2 topRight = ImVec2(topLeft.x + (numCols * GRID_COL_STEP), topLeft.y);
                    ImVec2 bottomLeft = ImVec2(topLeft.x, topLeft.y + (numRows * GRID_ROW_STEP));
                    ImVec2 bottomRight = ImVec2(topRight.x, bottomLeft.y);

                    draw_list->AddLine(topLeft, topRight, IM_COL32(255, 255, 0, 255), 2.0f);
                    draw_list->AddLine(topLeft, bottomLeft, IM_COL32(255, 255, 0, 255), 2.0f);
                    draw_list->AddLine(bottomLeft, bottomRight, IM_COL32(255, 255, 0, 255), 2.0f);
                    draw_list->AddLine(topRight, bottomRight, IM_COL32(255, 255, 0, 255), 2.0f);
                }

                startTile.x = startCol;
                startTile.y = startRow;
                endTile.x = endCol;
                endTile.y = endRow;

                draw_list->PopClipRect();
            }
		}
	
    }

    ImGui::End();
}