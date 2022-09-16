#pragma once

#include <Tools/Math/Math.h>
#include <Tools/Events/KeyCode.h>
#include <Tools/Events/MouseButton.h>
#include <vector>
#include <Delegate.h>

typedef Delegate<void, int, const Vector2f&> OnPositionChangedFunction;

class ControlPointsEditor
{
	std::vector<Vector2f> positions;

	class Buffer* vb;
	class VertexArray* va;
	class Shader* shader;

	float radius = 10;
	int selectedPoint = -1;
	bool editingPoint = false;

	Vector2f cameraPosition;
	bool movingCamera = false;
	float cameraSpeed = 0.5f;
	float zoomLevel = 1;
	float scrollAmount = 0;
	float zoomScrollSpeed = 200;
	static constexpr float MIN_ZOOM = 0.01f;
	static constexpr float MAX_ZOOM = 20;

	Vector2f oldMousePos;
	Matrix4x4 cameraTransform;

public:
	struct Settings
	{
		Color defaultColor = Color::Red;
		Color hoveredColor = Color::Blue;
		Color movingColor = Color::Cyan;

		KeyCode moveCameraKey = KeyCode::LeftShift;
		MouseButton moveCameraButton = MouseButton::Left;
		MouseButton movePointButton = MouseButton::Left;
	} settings;
	OnPositionChangedFunction onPositionChanged;

public:
	ControlPointsEditor();
	~ControlPointsEditor();

	void Render();
	void Update(TimeSpan dt);
	void DrawGUI();

public:
	inline Matrix4x4 GetCameraTransform() const { return cameraTransform; }
	inline const Vector2f& GetPoint(int index) const { return positions[index]; }
	inline const Vector2f& operator[](int index) const { return positions[index]; }
	inline const Vector2f& GetLastPoint() const { return positions[positions.size() - 1]; }
	
	inline int PointsCount() const { return positions.size(); }
	inline const Vector2f* PositionsArray() const { return positions.data(); }

public:
	void AddPoints(const std::initializer_list<Vector2f>& points);
	inline void AddPoints(const Vector2f* points, int count) { AddPoints(std::initializer_list(points, points + count)); }
	void Delete(int amount);
	void ResetPointColors();

private:
	void UpdateBufferData();
	void RecolorControlPoint(int index, const Color& color);
	void RepositionControlPoint(int index, const Vector2f& position);
	void SyncPointRadius(int index);

private:
	void OnMouseButtonPressed(MouseButton button);
	void OnMouseButtonReleased(MouseButton button);
	void OnScroll(float amount);
};