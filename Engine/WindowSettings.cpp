#include "WindowSettings.h"

#include "Imgui/imgui.h"
#include "Globals.h"
#include "Application.h"

#include "Imgui/imgui_impl_sdl_gl3.h"


WindowSettings::WindowSettings()
{
	//style = ImGui::GetStyle();
	//ImVec4 colorin(0, 0, 0, 0);
	//style.Colors[2] = colorin;
}


WindowSettings::~WindowSettings()
{
}

void WindowSettings::draw()
{

	ImGuiStyle &style = ImGui::GetStyle();
	// You can pass in a reference ImGuiStyle structure to compare to, revert to and save to (else it compares to the default style)
	const ImGuiStyle default_style; // Default style
	if (ImGui::Button("Revert Style"))

		ImGui::SetNextWindowSize(ImVec2(300, 600), ImGuiSetCond_FirstUseEver);

	if (ImGui::TreeNode("Rendering"))
	{
		ImGui::Checkbox("Anti-aliased lines", &style.AntiAliasedLines);
		ImGui::Checkbox("Anti-aliased shapes", &style.AntiAliasedShapes);
		ImGui::PushItemWidth(100);
		ImGui::DragFloat("Curve Tessellation Tolerance", &style.CurveTessellationTol, 0.02f, 0.10f, FLT_MAX, NULL, 2.0f);
		if (style.CurveTessellationTol < 0.0f) style.CurveTessellationTol = 0.10f;
		ImGui::DragFloat("Global Alpha", &style.Alpha, 0.005f, 0.20f, 1.0f, "%.2f"); // Not exposing zero here so user doesn't "lose" the UI (zero alpha clips all widgets). But application code could have a toggle to switch between zero and non-zero.
		ImGui::PopItemWidth();
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Sizes"))
	{
		ImGui::SliderFloat2("WindowPadding", (float*)&style.WindowPadding, 0.0f, 20.0f, "%.0f");
		ImGui::SliderFloat("WindowRounding", &style.WindowRounding, 0.0f, 16.0f, "%.0f");
		ImGui::SliderFloat("ChildWindowRounding", &style.ChildWindowRounding, 0.0f, 16.0f, "%.0f");
		ImGui::SliderFloat2("FramePadding", (float*)&style.FramePadding, 0.0f, 20.0f, "%.0f");
		ImGui::SliderFloat("FrameRounding", &style.FrameRounding, 0.0f, 16.0f, "%.0f");
		ImGui::SliderFloat2("ItemSpacing", (float*)&style.ItemSpacing, 0.0f, 20.0f, "%.0f");
		ImGui::SliderFloat2("ItemInnerSpacing", (float*)&style.ItemInnerSpacing, 0.0f, 20.0f, "%.0f");
		ImGui::SliderFloat2("TouchExtraPadding", (float*)&style.TouchExtraPadding, 0.0f, 10.0f, "%.0f");
		ImGui::SliderFloat("IndentSpacing", &style.IndentSpacing, 0.0f, 30.0f, "%.0f");
		ImGui::SliderFloat("ScrollbarSize", &style.ScrollbarSize, 1.0f, 20.0f, "%.0f");
		ImGui::SliderFloat("ScrollbarRounding", &style.ScrollbarRounding, 0.0f, 16.0f, "%.0f");
		ImGui::SliderFloat("GrabMinSize", &style.GrabMinSize, 1.0f, 20.0f, "%.0f");
		ImGui::SliderFloat("GrabRounding", &style.GrabRounding, 0.0f, 16.0f, "%.0f");
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Colors"))
	{
		static int output_dest = 0;
		static bool output_only_modified = false;
		if (ImGui::Button("Copy Colors"))
		{
			if (output_dest == 0)
				ImGui::LogToClipboard();
			else
				ImGui::LogToTTY();
			ImGui::LogText("ImGuiStyle& style = ImGui::GetStyle();");
			for (int i = 0; i < ImGuiCol_COUNT; i++)
			{
				const ImVec4& col = style.Colors[i];
				const char* name = ImGui::GetStyleColName(i); 			}
			ImGui::LogFinish();
		}
		ImGui::SameLine(); ImGui::PushItemWidth(120); ImGui::Combo("##output_type", &output_dest, "To Clipboard\0To TTY\0"); ImGui::PopItemWidth();
		ImGui::SameLine(); ImGui::Checkbox("Only Modified Fields", &output_only_modified);

		static ImGuiColorEditMode edit_mode = ImGuiColorEditMode_RGB;
		ImGui::RadioButton("RGB", &edit_mode, ImGuiColorEditMode_RGB);
		ImGui::SameLine();
		ImGui::RadioButton("HSV", &edit_mode, ImGuiColorEditMode_HSV);
		ImGui::SameLine();
		ImGui::RadioButton("HEX", &edit_mode, ImGuiColorEditMode_HEX);
		//ImGui::Text("Tip: Click on colored square to change edit mode.");

		static ImGuiTextFilter filter;
		filter.Draw("Filter colors", 200);

		ImGui::BeginChild("#colors", ImVec2(0, 300), true, ImGuiWindowFlags_AlwaysVerticalScrollbar);
		ImGui::PushItemWidth(-160);
		ImGui::ColorEditMode(edit_mode);
		
		for (int i = 0; i < ImGuiCol_COUNT; i++)
		{
			const char* name = ImGui::GetStyleColName(i);
			if (!filter.PassFilter(name))
				continue;
			ImGui::PushID(i);
			ImGui::ColorEdit4(name, (float*)&style.Colors[i], true);
			ImGui::PopID();
		}
		ImGui::PopItemWidth();
		ImGui::EndChild();

		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Fonts", "Fonts (%d)", ImGui::GetIO().Fonts->Fonts.Size))
	{

		ImFontAtlas* atlas = ImGui::GetIO().Fonts;
		if (ImGui::TreeNode("Atlas texture", "Atlas texture (%dx%d pixels)", atlas->TexWidth, atlas->TexHeight))
		{
			ImGui::Image(atlas->TexID, ImVec2((float)atlas->TexWidth, (float)atlas->TexHeight), ImVec2(0, 0), ImVec2(1, 1), ImColor(255, 255, 255, 255), ImColor(255, 255, 255, 128));
			ImGui::TreePop();
		}
		ImGui::PushItemWidth(100);
		for (int i = 0; i < atlas->Fonts.Size; i++)
		{
			ImFont* font = atlas->Fonts[i];
			ImGui::BulletText("Font %d: \'%s\', %.2f px, %d glyphs", i, font->ConfigData ? font->ConfigData[0].Name : "", font->FontSize, font->Glyphs.Size);
			ImGui::TreePush((void*)(intptr_t)i);
			if (i > 0) { ImGui::SameLine(); if (ImGui::SmallButton("Set as default")) { atlas->Fonts[i] = atlas->Fonts[0]; atlas->Fonts[0] = font; } }
			ImGui::PushFont(font);
			ImGui::Text("The quick brown fox jumps over the lazy dog");
			ImGui::PopFont();
			if (ImGui::TreeNode("Details"))
			{
				ImGui::DragFloat("font scale", &font->Scale, 0.005f, 0.3f, 2.0f, "%.1f");             // scale only this font
				ImGui::Text("Ascent: %f, Descent: %f, Height: %f", font->Ascent, font->Descent, font->Ascent - font->Descent);
				ImGui::Text("Fallback character: '%c' (%d)", font->FallbackChar, font->FallbackChar);
				for (int config_i = 0; config_i < font->ConfigDataCount; config_i++)
				{
					ImFontConfig* cfg = &font->ConfigData[config_i];
					ImGui::BulletText("Input %d: \'%s\'\nOversample: (%d,%d), PixelSnapH: %d", config_i, cfg->Name, cfg->OversampleH, cfg->OversampleV, cfg->PixelSnapH);
				}
				ImGui::TreePop();
			}
			ImGui::TreePop();
		}
		static float window_scale = 1.0f;
		ImGui::DragFloat("this window scale", &window_scale, 0.005f, 0.3f, 2.0f, "%.1f");              // scale only this window
		ImGui::DragFloat("global scale", &ImGui::GetIO().FontGlobalScale, 0.005f, 0.3f, 2.0f, "%.1f"); // scale everything
		ImGui::PopItemWidth();
		ImGui::SetWindowFontScale(window_scale);
		ImGui::TreePop();
	}


}
