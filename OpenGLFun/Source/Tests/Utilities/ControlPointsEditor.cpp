#include "ControlPointsEditor.h"

#include <Common.h>
#include <Tools/Graphics/Buffer.h>
#include <Tools/Graphics/VertexArray.h>
#include <Tools/Graphics/Shader.h>
#include <Tools/Renderer.h>
#include <Tools/Input.h>
#include <Vendor/ImGUI/imgui.h>
#include <iostream>

struct Disk
{
	Vector2f position;
	float radius;
	Color color;
};

ControlPointsEditor::ControlPointsEditor()
{
	BufferLayout layout =
	{
		{ ShaderDataType::Vec2, false },
		{ ShaderDataType::Float, false },
		{ ShaderDataType::Vec4, false },
	};

	vb = new Buffer(positions.size() * sizeof(Disk), BufferUsage::DynamicDraw);
	va = new VertexArray();
	va->AddVertexBuffer(vb, layout);

	shader = Shader::Load("Resources/Shaders/Disk.shader");

	Renderer::AddMouseButtonPressedFunction(
		OnMouseButtonPressedFunction::FromMethod<ControlPointsEditor, &ControlPointsEditor::OnMouseButtonPressed>(this));

	Renderer::AddMouseButtonReleasedFunction(
		OnMouseButtonReleasedFunction::FromMethod<ControlPointsEditor, &ControlPointsEditor::OnMouseButtonReleased>(this));

	Renderer::AddOnScrollFunction(OnScrollFunction::FromMethod<ControlPointsEditor, &ControlPointsEditor::OnScroll>(this));
}

ControlPointsEditor::~ControlPointsEditor()
{
	Renderer::RemoveMouseButtonPressedFunction(
		OnMouseButtonPressedFunction::FromMethod<ControlPointsEditor, &ControlPointsEditor::OnMouseButtonPressed>(this));

	Renderer::RemoveMouseButtonReleasedFunction(
		OnMouseButtonReleasedFunction::FromMethod<ControlPointsEditor, &ControlPointsEditor::OnMouseButtonReleased>(this));

	Renderer::RemoveOnScrollFunction(OnScrollFunction::FromMethod<ControlPointsEditor, &ControlPointsEditor::OnScroll>(this));

	delete vb;
	delete va;
	delete shader;
}

void ControlPointsEditor::Render()
{
	shader->Bind();
	shader->SetUniformMat4("u_Projection", cameraTransform);
	Renderer::DrawArray(*va, DrawPrimitive::Points);
}

void ControlPointsEditor::Update(TimeSpan dt)
{
	zoomLevel = MathUtil::Clamp(zoomLevel + zoomScrollSpeed * scrollAmount * dt, MIN_ZOOM, MAX_ZOOM);
	scrollAmount = 0;

	const WindowInfo& windowInfo = Renderer::GetWindow()->GetState();

	cameraTransform = Matrix4x4::CreateTranslation(-cameraPosition.x, -cameraPosition.y, 0);
	cameraTransform = Matrix4x4::CreateOrthographicProjection((float)windowInfo.width, (float)windowInfo.height, -1, 1) * cameraTransform;
	cameraTransform = Matrix4x4::CreateScale(zoomLevel) * cameraTransform;

	Vector2f mousePos = Input::GetCursorPosition();
	mousePos.x -= windowInfo.width / 2;
	mousePos.y = windowInfo.height / 2 - mousePos.y;
	mousePos /= zoomLevel;

	Vector2f mouseChange = mousePos - oldMousePos;
	oldMousePos = mousePos;

	if (movingCamera)
		cameraPosition -= cameraSpeed * mouseChange;

	if (editingPoint)
		RepositionControlPoint(selectedPoint, positions[selectedPoint] + mouseChange);
	else
	{
		int oldSelectedPoint = selectedPoint;
		selectedPoint = -1;

		for (size_t i = 0; i < positions.size(); i++)
		{
			Vector2f screenMousePos = mousePos + cameraPosition;
			float sqrDistance = (positions[i] - screenMousePos).SqrLength();
			float sqrRadius = radius * radius;

			if (sqrDistance <= sqrRadius)
			{
				selectedPoint = i;
				break;
			}
		}

		if (selectedPoint != oldSelectedPoint)
		{
			if (oldSelectedPoint != -1)
				RecolorControlPoint(oldSelectedPoint, settings.defaultColor);

			if (selectedPoint != -1)
				RecolorControlPoint(selectedPoint, settings.hoveredColor);
		}
	}
}

void ControlPointsEditor::DrawGUI()
{
	ImGui::DragFloat("Zoom Level", &zoomLevel, 0.1f, MIN_ZOOM, MAX_ZOOM);
	ImGui::DragFloat("Zoom Scroll Speed", &zoomScrollSpeed, 0.1f, 50, 300);
	ImGui::DragFloat("Camera Speed", &cameraSpeed, 0.01f, 0.1f, 5);
	ImGui::DragFloat2("Camera Position", cameraPosition.elements, 0.1f);

	if (ImGui::DragFloat("Control Points Radius", &radius, 0.1f, 1, 50))
	{
		for (size_t i = 0; i < positions.size(); i++)
			SyncPointRadius(i);
	}
}

void ControlPointsEditor::AddPoints(const std::initializer_list<Vector2f>& points)
{
	positions.insert(positions.end(), points);
	UpdateBufferData();
}

void ControlPointsEditor::Delete(int amount)
{
	const auto& end = positions.end();
	positions.erase(end - amount, end);
	UpdateBufferData();
}

void ControlPointsEditor::ResetPointColors()
{
	for (size_t i = 0; i < positions.size(); i++)
		RecolorControlPoint(i, settings.defaultColor);
}

void ControlPointsEditor::UpdateBufferData()
{
	Disk* data = Stackalloc(Disk, positions.size());
	for (size_t i = 0; i < positions.size(); i++)
	{
		data[i].position = positions[i];
		data[i].radius = radius;
		data[i].color = settings.defaultColor;
	}
	vb->SetData(data, positions.size() * sizeof(Disk));
}

void ControlPointsEditor::RepositionControlPoint(int index, const Vector2f& position)
{
	positions[index] = position;
	vb->SetSubData(&position, index * sizeof(Disk), sizeof(Vector2f));

	if (!onPositionChanged.IsNull())
		onPositionChanged(index, position);
}

void ControlPointsEditor::SyncPointRadius(int index)
{
	vb->SetSubData(&radius, index * sizeof(Disk) + sizeof(Vector2f), sizeof(float));
}

void ControlPointsEditor::RecolorControlPoint(int index, const Color& color)
{
	vb->SetSubData(&color, index * sizeof(Disk) + sizeof(Vector2f) + sizeof(float), sizeof(Color));
}

void ControlPointsEditor::OnMouseButtonPressed(MouseButton button)
{
	if (Input::IsKeyDown(settings.moveCameraKey) && button == settings.moveCameraButton)
		movingCamera = true;
	else if (button == settings.movePointButton && selectedPoint != -1)
	{
		editingPoint = true;
		RecolorControlPoint(selectedPoint, settings.movingColor);
	}
}

void ControlPointsEditor::OnMouseButtonReleased(MouseButton button)
{
	movingCamera = false;
	if (button == settings.movePointButton && editingPoint)
	{
		editingPoint = false;
		RecolorControlPoint(selectedPoint, settings.hoveredColor);
	}
}

void ControlPointsEditor::OnScroll(float amount)
{
	scrollAmount += amount;
}