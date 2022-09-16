#include "CompleteMessTest.h"
using namespace CompleteMessConstants;

static Curve2D* Smoothify(const Curve2D* curve)
{
	int originalPointsCount = curve->GetPointsCount();
	int amount = (originalPointsCount - 1) * 2;
	Vector2f* points = (Vector2f*)alloca(amount * sizeof(Vector2f));

	for (int i = 1; i < originalPointsCount; i++)
	{
		Vector2f originalPoint = curve->GetPoint(i);
		Vector2f previousOriginalPoint = curve->GetPoint(i - 1);
		Vector2f d = originalPoint - previousOriginalPoint;

		float t = 0.25f;
		points[2 * i - 2] = previousOriginalPoint + d * t;
		points[2 * i - 1] = previousOriginalPoint + d * (1 - t);
	}

	return new Curve2D(points, amount);
}

CompleteMessTest::CompleteMessTest()
{
	projection = Matrix4x4::CreateOrthographicProjection(0, 640, 0, 480, -1.5f, 1.5f);
	translation = Matrix4x4::CreateTranslation(300, 200, 0);
	scale = Matrix4x4::CreateScale(400);

	float square[] = {
	-0.5f, -0.5f, 1, 1, 1, 0, 0,
	-0.5f, 0.5f, 1, 1, 0, 0, 1,
	0.5f, 0.5f, 1, 0, 0, 1, 1,
	0.5f, -0.5f, 0, 0, 0, 1, 0
	};

	uint squareIndices[] = {
		0, 1, 3, 3, 2, 1
	};

	BufferLayout squareLayout = {
		{ ShaderDataType::Vec2, false },
		{ ShaderDataType::Vec3, false },
		{ ShaderDataType::Vec2, false }
	};

	squareVB = new Buffer(square, sizeof(square));
	squareIB = new Buffer(squareIndices, sizeof(squareIndices));

	squareVA = new VertexArray;
	squareVA->AddVertexBuffer(squareVB, squareLayout);
	squareVA->SetIndexBuffer(squareIB);
	squareVA->Unbind();

	float triangle[] = {
		0, 0.75f, 1, 0.3f, 0.3f,
		-0.5f, -0.5f, 0.3f, 1, 0.3f,
		0.5f, -0.5f, 0.3f, 0.3f, 1
	};

	uint triangleIndices[] = {
		0, 1, 2
	};

	BufferLayout triangleLayout = {
		{ ShaderDataType::Vec2, false },
		{ ShaderDataType::Vec3, false }
	};

	triangleVB = new Buffer(triangle, sizeof(triangle));
	triangleIB = new Buffer(triangleIndices, sizeof(triangleIndices));

	triangleVA = new VertexArray;
	triangleVA->AddVertexBuffer(triangleVB, triangleLayout);
	triangleVA->SetIndexBuffer(triangleIB);
	triangleVA->Unbind();

	texture = Texture::Load("Resources/Textures/Pixel Test.png");
	shader = Shader::Load("Resources/Shaders/Test.shader");

	shader->Bind();
	texture->Bind(0);
	shader->SetUniformInt("u_Texture", 0);

	Renderer::SetClearColor(0, 0, 1);

	basic = Shader::Load("Resources/Shaders/Basic.shader");
	basic->Bind();

	testier = new Curve2D{ originScaleTest };

	curve = new Curve2D{
		//{ -0.75, -0.5 },
		//{ -0.5, -0.75 },
		//{ -0.25, -0.25 },
		//{ -0.5, 0.25 },
		//{ 0, 0.5 },
		//{ 0.25, -0.25 },
		//{ 0.5, -0.5 },
		//{ 0.5, -0.75 },
		{ 0.3f, 0.65f },
		{ 0.3f, 0.75f },
		{ 0.f, 0.75f },
		{ 0.f, -0.75f },
		{ -0.3f, -0.75f },
		{ -0.3f, -0.65f },
	};
	smooths[0] = Smoothify(curve);
	for (int i = 1; i < SMOOTHS_AMOUNT; i++)
		smooths[i] = Smoothify(smooths[i - 1]);

	Vector3f cubeVertices[] =
	{
		Vector3f(-1, -1, -1),
		Vector3f(1, -1, -1),
		Vector3f(1, 1, -1),
		Vector3f(-1, 1, -1),
		Vector3f(-1, -1, 1),
		Vector3f(1, -1, 1),
		Vector3f(1, 1, 1),
		Vector3f(-1, 1, 1)
	};

	int cubeIndices[] =
	{
		0, 1, 3, 3, 1, 2,
		1, 5, 2, 2, 5, 6,
		5, 4, 6, 6, 4, 7,
		4, 0, 7, 7, 0, 3,
		3, 2, 7, 7, 2, 6,
		4, 5, 0, 0, 5, 1
	};

	BufferLayout cubeBL = { { ShaderDataType::Vec3, false } };
	cubeVB = new Buffer(cubeVertices, sizeof(cubeVertices));
	cubeIB = new Buffer(cubeIndices, sizeof(cubeIndices));
	cube = new VertexArray();
	cube->AddVertexBuffer(cubeVB, cubeBL);
	cube->SetIndexBuffer(cubeIB);
}

CompleteMessTest::~CompleteMessTest()
{
	delete testier;
	delete cube;
	delete cubeVB;
	delete cubeIB;
	for (int i = 0; i < SMOOTHS_AMOUNT; i++)
		delete smooths[i];

	delete curve;
	delete texture;
	delete shader;
	delete squareVA;
	delete triangleVA;

	delete squareVB;
	delete squareIB;
	delete triangleVB;
	delete triangleIB;
}

void CompleteMessTest::Render(TimeSpan deltaTime)
{
	elapsed += deltaTime;
	if (c > 1)
		dc = -v;
	else if (c < 0)
		dc = v;
	c += dc * deltaTime;

	if (p > 0.75f)
		dp = -s;
	else if (p < 0)
		dp = s;
	p += dp * deltaTime;

	angle += rotationRate * deltaTime;

	basic->Bind();
	Matrix4x4 m = Matrix4x4::CreateTranslation(0, 0, angle);
	m = m * Matrix4x4::CreateRotationX(angle * 2);
	m = m * Matrix4x4::CreateScale(0.5f);
	
	m = Matrix4x4::CreateView(Vector3f::Zero, Vector3f::UnitZ, Vector3f::UnitY) * m;
	
	m = /*Matrix4x4::CreateOrthographicProjection(-10, 10, -10, 10, 0.1f, 10) * m;*/ Matrix4x4::CreatePerspectiveProjection(70 * PI / 180, Renderer::GetWindow()->AspectRatio(), 0.1f, 20) * m;
	basic->SetUniformMat4("u_Matrix", m);
	Vector4f vvvvv = m * Vector4f(-1, -1, -1, 1);
	Renderer::DrawArrayIndexed(*cube);
	
	shader->Bind();
	Renderer::SetClearColor(c / 3, c / 3, c / 3, 1);
	//640.f / 480.f
	projection = Matrix4x4::CreatePerspectiveProjection(120 * PI / 180.f, 16.f/9.f, 0.1f, 30);
	translation = Matrix4x4::CreateTranslation(0, 0, 5);
	Matrix4x4 rotation = Matrix4x4::CreateRotationX(angle); //Matrix4x4::Identity; //Matrix4x4::CreateRotationZ(angle);
	scale = Matrix4x4::CreateScale(10);
	shader->SetUniformMat4("u_Rotation", rotation);
	shader->SetUniformMat4("u_Scale", scale);
	shader->SetUniformMat4("u_Translation", translation);
	shader->SetUniformMat4("u_Projection", projection);
	shader->SetUniformVec4("u_Color", c, 1 - c, 1 - c, 0.5f);
	
	//shader->SetUniformVec2("u_PositionOffset", -p, p / 2);
	shader->SetUniformBoolean("u_UseTexture", false);
	Renderer::DrawArrayIndexed(*triangleVA);
	
	shader->SetUniformVec2("u_PositionOffset", p / 2, -p);
	rotation = Matrix4x4::CreateRotationZ(angle * 3);
	shader->SetUniformMat4("u_Rotation", rotation);
	shader->SetUniformBoolean("u_UseTexture", true);
	Renderer::DrawArrayIndexed(*squareVA);

	basic->Bind();
	Matrix4x4 proj = Matrix4x4::CreateOrthographicProjection(0, 640, 0, 480, -1, 1);
	Matrix4x4 mat = proj;
	
	basic->SetUniformMat4("u_Matrix", mat);
	Renderer::DrawPointsOfCurve(*testier);
	
	Matrix4x4 scal = Matrix4x4::CreateScale(50);
	Matrix4x4 trans1 = Matrix4x4::CreateTranslation(300, 200, 0);
	Matrix4x4 scal2 = Matrix4x4::CreateScale(2 * abs(sin(angle * 2)), abs(sin(angle * 2)), 0, Vector3f(originScaleTest, 0));
	
	Matrix4x4 rot = Matrix4x4::CreateRotationZ(angle * 4, originScaleTest);
	mat = proj * rot * scal2 * trans1 * scal;
	basic->SetUniformMat4("u_Matrix", mat);
	
	Renderer::DrawArrayIndexed(*squareVA);
	
	pos += Vector3f(moveDir.x * deltaTime, moveDir.y * deltaTime, 0);
	basic->SetUniformMat4("u_Matrix", Matrix4x4::CreateTranslation(pos));
	Renderer::DrawPointsOfCurve(*curve);
	
	//smooths[0] = Smoothify(curve);
	//for (int i = 1; i < SMOOTHS_AMOUNT; i++)
	//	smooths[i] = Smoothify(smooths[i - 1]);
	
	Curve2D* c = curve;
	for (int i = 0; i < SMOOTHS_AMOUNT; i++)
	{
		if (elapsed > (i + 1) * 2)
			c = smooths[i];
	}
	Renderer::DrawCurve(*c);

	//for (int i = 0; i < SMOOTHS_AMOUNT; i++)
	//	delete smooths[i];

	//Renderer::DrawCurve(*smooths[0]);
	//Renderer::DrawCurve(*smooths[1]);
	//Renderer::DrawCurve(*smooths[SMOOTHS_AMOUNT - 1]);
}

void CompleteMessTest::OnKeyPressed(KeyCode key, bool repeat)
{
	if (key == KeyCode::A)
		moveDir.x = -1;
	else if (key == KeyCode::D)
		moveDir.x = 1;
	else if (key == KeyCode::W)
		moveDir.y = 1;
	else if (key == KeyCode::S)
		moveDir.y = -1;
}

void CompleteMessTest::OnKeyReleased(KeyCode key)
{
	if (key == KeyCode::A)
		moveDir.x = 0;
	else if (key == KeyCode::D)
		moveDir.x = 0;
	else if (key == KeyCode::W)
		moveDir.y = 0;
	else if (key == KeyCode::S)
		moveDir.y = 0;
}