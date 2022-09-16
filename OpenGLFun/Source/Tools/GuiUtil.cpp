#include "GuiUtil.h"
#include "Vendor/ImGui/ImGui_internal.h"

bool GuiUtil::ClampedFloatSlider(const char* label, float* v, float speed, float min, float max, bool allowMin, bool allowMax)
{
	float value = *v;
	if (ImGui::DragFloat(label, &value, speed, min, max))
	{
		if (value <= min)
			value = allowMin ? min : min + speed;

		if (value >= max)
			value = allowMax ? max : max - speed;

		*v = value;
		return true;
	}
	return false;
}


bool GuiUtil::MultiFloatSlider(const char* label, float* v, int components, const float* v_min, const float* v_max, const char* format, ImGuiSliderFlags flags)
{
	using namespace ImGui;

	ImGuiWindow* window = GetCurrentWindow();
	if (window->SkipItems)
		return false;

	ImGuiContext& g = *GImGui;
	bool value_changed = false;
	BeginGroup();
	PushID(label);
	PushMultiItemsWidths(components, CalcItemWidth());
	size_t type_size = sizeof(float);
	for (int i = 0; i < components; i++)
	{
		PushID(i);
		if (i > 0)
			SameLine(0, g.Style.ItemInnerSpacing.x);
		value_changed |= SliderScalar("", ImGuiDataType_Float, v, v_min + i, v_max + i, format, flags);
		PopID();
		PopItemWidth();
		v += 1;
	}
	PopID();

	const char* label_end = FindRenderedTextEnd(label);
	if (label != label_end)
	{
		SameLine(0, g.Style.ItemInnerSpacing.x);
		TextEx(label, label_end);
	}

	EndGroup();
	return value_changed;
}
