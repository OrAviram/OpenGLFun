#pragma once
#include "Vendor/ImGUI/imgui.h"
#include "Math/Vector.h"

namespace GuiUtil
{
	bool ClampedFloatSlider(const char* label, float* v, float speed, float min, float max, bool allowMin = true, bool allowMax = true);

	bool MultiFloatSlider(const char* label, float* v, int components, const float* minimums, const float* maximums, const char* format = (const char*)0, ImGuiSliderFlags flags = 0);
	inline bool Float3Slider(const char* label, float v[3], const float minimums[3], const float maximums[3], const char* format = (const char*)0, ImGuiSliderFlags flags = 0)
	{
		return MultiFloatSlider(label, v, 3, minimums, maximums, format, flags);
	}
	inline bool Vec3Slider(const char* label, Vector3f& value, const Vector3f& minimum, const Vector3f& maximum, const char* format = (const char*)0, ImGuiSliderFlags flags = 0)
	{
		return MultiFloatSlider(label, value.elements, 3, minimum.elements, maximum.elements, format, flags);
	}
}