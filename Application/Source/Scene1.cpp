#include "Scene1.h"
#include "GL\glew.h"

#include "shader.hpp"
#include "Application.h"
#include "Mtx44.h"
#include "MeshBuilder.h"

Scene1::Scene1()
{
}

Scene1::~Scene1()
{
}

void Scene1::Init()
{
	Mtx44 projection;
	projection.SetToPerspective(45.f, 16.f / 9.f, 0.1f, 1000.f);
	projectionStack.LoadMatrix(projection);
	wardrobe_move = 0;
	//give plane pos an initial value of (0,1,0)
	planePos.Set(0.f, 1.f, 0.f);
	//init angleY to 0
	angleY = 0;
	//init scaling to 1.1.1
	scaling.Set(1.f, 1.f, 1.f);
	//init
	sceneTime = 0;

	//Enable depth buffer and depth testing
	glEnable(GL_DEPTH_TEST);

	//Enable back face culling
	glEnable(GL_CULL_FACE);

	//Default to fill mode
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// Generate a default VAO for now
	glGenVertexArrays(1, &m_vertexArrayID);
	glBindVertexArray(m_vertexArrayID);

	// Load the shader programs
	m_programID = LoadShaders("Shader//TransformVertexShader.vertexshader",
		"Shader//SimpleFragmentShader.fragmentshader");
	glUseProgram(m_programID);

	// Get a handle for our "MVP" uniform
	m_parameters[U_MVP] = glGetUniformLocation(m_programID, "MVP");


	// Initialise the camera
	camera.Init(Vector3(10, 30, 20), Vector3(0, 0, 0), Vector3(0, 1, 0));

	// Init VBO here
	for (int i = 0; i < NUM_GEOMETRY; ++i)
	{
		meshList[i] = nullptr;
	}
	
	meshList[GEO_AXES] = MeshBuilder::GenerateAxes("reference", 1000, 1000, 1000);
	//Wardrobe
	meshList[GEO_WARDROBE] = MeshBuilder::GenerateCube("quad", Color(0.42f, 0.15f, 0.02f), 1);
	meshList[GEO_WARDROBE_CASE] = MeshBuilder::GenerateCube("quad", Color(0.52f, 0.26f, 0.02f), 1);
	meshList[GEO_WARDROBE_CASE_SUPPORT] = MeshBuilder::GenerateCube("quad", Color(0.39f, 0.19f, 0.01f), 1);
	
	// Window
	meshList[Windows_Frame] = MeshBuilder::GenerateCube("quad", Color(0.5f, 0.5f, 0.5f), 1);
	meshList[Glass_Panes] = MeshBuilder::GenerateCube("quad", Color(0.27f, 0.71f, 1.f), 1);
	meshList[Window_Acc] = MeshBuilder::GenerateCube("quad", Color(1.f, 1.f, 1.f), 1);
	meshList[Window_Handel] = MeshBuilder::GenerateCube("quad", Color(0.f, 0.28f, 0.7f), 1);
	
	// shevels
	meshList[shevles_Case] = MeshBuilder::GenerateCube("quad", Color(0.2, 0.6f, 1.f), 1);
	meshList[Shevlevs_C] = MeshBuilder::GenerateCube("quad", Color(0.f, 0.4f, 0.8f), 1);
	meshList[GEO_PURPLE_QUAD] = MeshBuilder::GenerateQuad("quad", Color(0.4f, 0.4f, 1.f), 1); 
	
	//Table
	meshList[Table] = MeshBuilder::GenerateCube("quad", Color(1, 1, 1), 1);
	meshList[Table_Legs] = MeshBuilder::GenerateCylinder("cylinder", Color(0.4f, 0.4f, 0.4f), 360, 1);
	
	//PC
	meshList[PC_BOX] = MeshBuilder::GenerateCube("quad", Color(0, 0, 0), 1);
	meshList[Monitor] = MeshBuilder::GenerateCube("quad", Color(1, 1, 1), 1);
	// fan
	meshList[fan_body] = MeshBuilder::GenerateCylinder("cylinder", Color(1, 1, 1), 360, 1);
	meshList[fan_blades] = MeshBuilder::GenerateCube("quad", Color(0.5f, 0.5f, 0.5f), 1);
	

	// Chair
	meshList[Chair_body] = MeshBuilder::GenerateCube("quad", Color(0.5f, 0.5f, 0.5f), 1);
	meshList[Chair_mids] = MeshBuilder::GenerateCube("quad", Color(1.f, 1.5f, 1.5f), 1);
	meshList[Chair_support] = MeshBuilder::GenerateCube("quad", Color(0.f, 0.f, 0.f), 1);
	meshList[Chair_wheels] = MeshBuilder::GenerateSphere("quad", Color(1.f, 1.f, 1.f), 360, 360, 1);
	//Bed
	meshList[GEO_BED_LEGS] = MeshBuilder::GenerateCube("legs", Color(0, 0, 0), 1);
	meshList[GEO_BED_SUPPORT_CUBE] = MeshBuilder::GenerateCube("legs", Color(0.7, 0.4, 0), 1);
	meshList[GEO_BED_BACK] = MeshBuilder::GenerateCube("Back", Color(0.7f, 0.4f, 0), 1);

	meshList[GEO_QUAD] = MeshBuilder::GenerateQuad("quad", Color(1, 1, 1), 1);
	meshList[GEO_RED_QUAD] = MeshBuilder::GenerateQuad("quad", Color(1, 0, 0), 1); // Generate a red quad (Color is R, G, B from 0-1)
	meshList[GEO_ORANGE_QUAD] = MeshBuilder::GenerateQuad("quad", Color(0.88f, 0.88f, 0.58f), 1); // Generate a red quad (Color is R, G, B from 0-1)
	meshList[GEO_CUBE] = MeshBuilder::GenerateCube("cube", Color(1, 1, 1), 1);
	meshList[GEO_REDCUBE] = MeshBuilder::GenerateCube("cube", Color(1, 0, 0), 1);
	meshList[GEO_CYLINDER] = MeshBuilder::GenerateCylinder("cylinder", Color(1, 1, 1), 360, 1);
	meshList[GEO_SPHERE] = MeshBuilder::GenerateSphere("sphere", Color(1, 0, 0), 360, 360, 1); 
	meshList[GEO_LIGHTS] = MeshBuilder::GenerateHemisphere("sphere", Color(1, 0, 0), 360, 360, 1);
	
}
float fan_speed = 1;
void Scene1::Update(double dt)
{
	camera.Update(dt);
	sceneTime += dt; //keep track of how much time hs passed
	planePos.y = fabs(sin(sceneTime));
	// Check for key press, you can add more for interaction
	HandleKeyPress();

	//fan
	fan_rotate += fan_speed;
	//window
	window_rotate+=0.1;
	if (Application::IsKeyPressed('P'))
	{
		fan_speed += dt + 0.3f;
	}

	if (Application::IsKeyPressed('O'))
	{
		fan_speed -= dt + 0.3f;
	}
	// chair 

	if (Application::IsKeyPressed(VK_UP))
	{
		chair_move_z -= dt + 0.3f;
		chair_rotate = 180;
	}

	if (Application::IsKeyPressed(VK_DOWN))
	{
		chair_move_z += dt + 0.3f;
		chair_rotate = 0;
	}

	if (Application::IsKeyPressed(VK_RIGHT))
	{
		chair_move_x += dt + 0.3f;
		chair_rotate = 90;
	}

	if (Application::IsKeyPressed(VK_LEFT))
	{
		chair_move_x -= dt + 0.3f;
		chair_rotate = 270;
	}

	//wardrobe
	if (Application::IsKeyPressed('J'))
	{
		wardrobe_move -= dt + 0.3f;
	}


	if (Application::IsKeyPressed('L'))
	{
		wardrobe_move  += dt + 0.3f;
	}

	wardrobe_move = Math::Clamp(wardrobe_move, 0.f, 4.f);

	if (Application::IsKeyPressed('W'))
	{
		planePos.z -= dt + 0.3f;
	}

	if (Application::IsKeyPressed('S'))
	{
		planePos.z += dt + 0.3f;
	}
	//rotate plane abt  y-axis
	if (Application::IsKeyPressed(VK_LEFT))
	{
		angleY += dt * 100.f;
	}
	if (Application::IsKeyPressed(VK_RIGHT))
	{
		angleY -= dt * 100.f;
	}
	// Update the camera object
	//camera.Update(dt);

	//scale plane uniformly
	/*if (Application::IsKeyPressed(VK_UP))
	{
		scaling.x += dt * 2.f;
		scaling.y += dt * 2.f;
		scaling.z += dt * 2.f;
	}
	if (Application::IsKeyPressed(VK_DOWN))
	{
		scaling.x -= dt * 2.f;
		scaling.y -= dt * 2.f;
		scaling.z -= dt * 2.f;
	}*/
}

void Scene1::Render() //render means to "draw"
{

	 /*Define the matrices to handle transformation*/
	Mtx44 model;
	Mtx44 view;
	Mtx44 projection;

	// Always set the model matrix to identity
	// i.e. placed at origin, scale 1 and rotation 0
       Mtx44:
	model.SetToIdentity();


	// Use the camera as the view matrix
	/*view.SetToLookAt(
		camera.position.x, camera.position.y, camera.position.z,
		camera.target.x, camera.target.y, camera.target.z,
		camera.up.x, camera.up.y, camera.up.z
	);*/

	viewStack.LoadIdentity();
	viewStack.LookAt(camera.position.x, camera.position.y,
		camera.position.z, camera.target.x, camera.target.y,
		camera.target.z, camera.up.x, camera.up.y, camera.up.z);

	// FOV, Aspect Ratio, Near plane, Far plane
	/*projection.SetToPerspective(45.0f, 4.0f / 3.0f, 0.1f, 1000.0f);*/

	// Clear color buffer every frame
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Model Matrix contains scale,translate & rotation
	/*Mtx44 MVP = projection * view * model;*/
	Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	/*glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);*/

	// Render VBO here

	meshList[GEO_AXES]->Render();
	model.SetToIdentity();

	// Scene walls
	
	modelStack.PushMatrix();
	{
	/*	modelStack.Translate(planePos.x, planePos.y, planePos.z);*/
		modelStack.Scale(10.f, 10.f, 10.f);
		//floor
		modelStack.PushMatrix();
		{
		modelStack.Translate(0.f, 0.f, 0.f);
		modelStack.Scale(2.f, 2.f, 2.f);
		MVP = projectionStack.Top() * viewStack.Top() *modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,&MVP.a[0]);
		meshList[GEO_ORANGE_QUAD]->Render();
		modelStack.PopMatrix();

		}
		//left side
		modelStack.PushMatrix();
		{
			modelStack.Translate(-1.f, 1.f, 0.f);
			modelStack.Rotate(-90.f, 0.f, 0.f, 10.f); // degree/angle then x y z
			modelStack.Scale(2.f, 2.f, 2.f);
			MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
			meshList[GEO_PURPLE_QUAD]->Render();
			modelStack.PopMatrix();

		}
		//right side
		modelStack.PushMatrix();
		{
		modelStack.Translate(0.f, 1.f, -1.f);
		modelStack.Scale(2.f, 2.f, 2.f);
		modelStack.Rotate(90.f, 1.f, 0.f, 0.f); // degree/angle then x y z
		MVP = projectionStack.Top() * viewStack.Top() *modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,&MVP.a[0]);
		meshList[GEO_PURPLE_QUAD]->Render();
		modelStack.PopMatrix();

		}




		modelStack.PopMatrix();
	}

	// Bed
	modelStack.PushMatrix();
	{
		modelStack.Scale(3, 3, 3);
		modelStack.Translate(-2.82f, -2.f, -1.5f);
		modelStack.PushMatrix();
		{
			modelStack.Translate(0.f, 2.7f, 0.f);
			modelStack.Scale(1.f, 0.4f, 2.f);
			MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
			meshList[GEO_REDCUBE]->Render();
			modelStack.PopMatrix();
		}

		modelStack.PushMatrix();
		{
			modelStack.Translate(0.f, 2.7f, -1.25f);
			modelStack.Scale(1.f, 0.4f, 0.5f);
			MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
			meshList[GEO_CUBE]->Render(); 
			modelStack.PopMatrix();
		}

		modelStack.PushMatrix();
		{
			modelStack.Translate(0.f, 2.4f, -0.25f);
			modelStack.Scale(1.f, 0.2f, 2.5f);
			MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
			meshList[GEO_BED_SUPPORT_CUBE]->Render();
			modelStack.PopMatrix();
		}

		modelStack.PushMatrix(); // bottom right
		{
			modelStack.Translate(0.4f, 2.15f, 0.9f);
			modelStack.Scale(0.2f, 0.3f, 0.2f);
			MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
			meshList[GEO_BED_LEGS]->Render();
			modelStack.PopMatrix();
		}

		modelStack.PushMatrix();// bottom left
		{
			modelStack.Translate(-0.4f, 2.15f, 0.9f);
			modelStack.Scale(0.2f, 0.3f, 0.2f);
			MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
			meshList[GEO_BED_LEGS]->Render();
			modelStack.PopMatrix();
		}

		modelStack.PushMatrix();//top left
		{
			modelStack.Translate(-0.4f, 2.15f, -1.4f);
			modelStack.Scale(0.2f, 0.3f, 0.2f);
			MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
			meshList[GEO_BED_LEGS]->Render();
			modelStack.PopMatrix();
		}
		
		modelStack.PushMatrix();// top right
		{
			modelStack.Translate(0.4f, 2.15f, -1.4f);
			modelStack.Scale(0.2f, 0.3f, 0.2f);
			MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
			meshList[GEO_BED_LEGS]->Render();
			modelStack.PopMatrix();
		}

		modelStack.PushMatrix();// bed back
		{
			modelStack.Translate(0.f, 2.8f, -1.6f);
			modelStack.Scale(1.f, 1.f, 0.2f);
			MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
			meshList[GEO_BED_BACK]->Render();
			modelStack.PopMatrix();
		}
	}
	modelStack.PopMatrix();
	

	modelStack.PushMatrix(); // Wardrobe
	{
		modelStack.Translate(5.5f, 0.f, -8.f);
		
		modelStack.PushMatrix(); // Main box
		{
			modelStack.Translate(0.f, 8.f, 0.f);
			modelStack.Scale(8.f, 16.f, 4.f);
			MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
			meshList[GEO_WARDROBE]->Render();
			modelStack.PopMatrix();
		}
		
		modelStack.PushMatrix();
		{
			modelStack.Translate(wardrobe_move, 0, 0);
		modelStack.PushMatrix(); // left door
		{
			modelStack.Translate(-2.f, 8.25f, 2.76f);

			modelStack.Scale(3.f, 15.5f, 0.5f);
			MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
			meshList[GEO_WARDROBE_CASE]->Render();
			modelStack.PopMatrix();
		}

		modelStack.PushMatrix(); // left door top long pice
		{
			modelStack.Translate(-2.f, 16.27f, 2.76f);
			modelStack.Scale(3.f, 0.5f, 0.5f);
			MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
			meshList[GEO_WARDROBE_CASE_SUPPORT]->Render();
			modelStack.PopMatrix();
		}

		modelStack.PushMatrix(); // left door top side peice long 
		{
			modelStack.Translate(-3.75f, 8.25f, 2.76f);
			modelStack.Scale(0.5f, 15.5f, 0.5f);
			MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
			meshList[GEO_WARDROBE_CASE_SUPPORT]->Render();
			modelStack.PopMatrix();
		}

		modelStack.PushMatrix(); // left door left side peice cube
		{
			modelStack.Translate(-3.75f, 16.27f, 2.76f);
			modelStack.Scale(0.5f, 0.5f, 0.5f);
			MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
			meshList[GEO_WARDROBE_CASE]->Render();
			modelStack.PopMatrix();
		}

		modelStack.PushMatrix(); // left door right side peice cube
		{
			modelStack.Translate(-0.25f, 16.27f, 2.76f);
			modelStack.Scale(0.5f, 0.5f, 0.5f);
			MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
			meshList[GEO_WARDROBE_CASE]->Render();
			modelStack.PopMatrix();
		}

		modelStack.PushMatrix(); // left door right side peice long 
		{
			modelStack.Translate(-0.25f, 8.25f, 2.76f);
			modelStack.Scale(0.5f, 15.5f, 0.5f);
			MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
			meshList[GEO_WARDROBE_CASE_SUPPORT]->Render();
			modelStack.PopMatrix();
		}

		modelStack.PushMatrix(); // left door bottom long pice
		{
			modelStack.Translate(-2.f, 0.25f, 2.76f);
			modelStack.Scale(3.f, 0.5f, 0.5f);
			MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
			meshList[GEO_WARDROBE_CASE_SUPPORT]->Render();
			modelStack.PopMatrix();
		}

		modelStack.PushMatrix(); // left door left side peice cube
		{
			modelStack.Translate(-3.75f, 0.25f, 2.76f);
			modelStack.Scale(0.5f, 0.5f, 0.5f);
			MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
			meshList[GEO_WARDROBE_CASE]->Render();
			modelStack.PopMatrix();
		}

		modelStack.PushMatrix(); // left door right side peice cube
		{
			modelStack.Translate(-0.25f, 0.25f, 2.76f);
			modelStack.Scale(0.5f, 0.5f, 0.5f);
			MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
			meshList[GEO_WARDROBE_CASE]->Render();
			modelStack.PopMatrix();
		}

		}
		modelStack.PopMatrix();
		//modelStack.PushMatrix(); // left door
		//{
		//	modelStack.Translate(-2.f, 8.25f, 2.76f);
		//	modelStack.Scale(3.f, 15.5f, 0.5f);
		//	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
		//	meshList[GEO_WARDROBE_CASE]->Render();
		//	modelStack.PopMatrix();
		//}

		//modelStack.PushMatrix(); // left door top long pice
		//{
		//	modelStack.Translate(-2.f, 16.27f, 2.76f);
		//	modelStack.Scale(3.f, 0.5f, 0.5f);
		//	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
		//	meshList[GEO_WARDROBE_CASE_SUPPORT]->Render();
		//	modelStack.PopMatrix();
		//}

		//modelStack.PushMatrix(); // left door top side peice long 
		//{
		//	modelStack.Translate(-3.75f, 8.25f, 2.76f);
		//	modelStack.Scale(0.5f, 15.5f, 0.5f);
		//	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
		//	meshList[GEO_WARDROBE_CASE_SUPPORT]->Render();
		//	modelStack.PopMatrix();
		//}

		//modelStack.PushMatrix(); // left door left side peice cube
		//{
		//	modelStack.Translate(-3.75f, 16.27f, 2.76f);
		//	modelStack.Scale(0.5f, 0.5f, 0.5f);
		//	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
		//	meshList[GEO_WARDROBE_CASE]->Render();
		//	modelStack.PopMatrix();
		//}

		//modelStack.PushMatrix(); // left door right side peice cube
		//{
		//	modelStack.Translate(-0.25f, 16.27f, 2.76f);
		//	modelStack.Scale(0.5f, 0.5f, 0.5f);
		//	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
		//	meshList[GEO_WARDROBE_CASE]->Render();
		//	modelStack.PopMatrix();
		//}

		//modelStack.PushMatrix(); // left door right side peice long 
		//{
		//	modelStack.Translate(-0.25f, 8.25f, 2.76f);
		//	modelStack.Scale(0.5f, 15.5f, 0.5f);
		//	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
		//	meshList[GEO_WARDROBE_CASE_SUPPORT]->Render();
		//	modelStack.PopMatrix();
		//}

		//modelStack.PushMatrix(); // left door bottom long pice
		//{
		//	modelStack.Translate(-2.f, 0.25f, 2.76f);
		//	modelStack.Scale(3.f, 0.5f, 0.5f);
		//	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
		//	meshList[GEO_WARDROBE_CASE_SUPPORT]->Render();
		//	modelStack.PopMatrix();
		//}

		//modelStack.PushMatrix(); // left door left side peice cube
		//{
		//	modelStack.Translate(-3.75f, 0.25f, 2.76f);
		//	modelStack.Scale(0.5f, 0.5f, 0.5f);
		//	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
		//	meshList[GEO_WARDROBE_CASE]->Render();
		//	modelStack.PopMatrix();
		//}

		//modelStack.PushMatrix(); // left door right side peice cube
		//{
		//	modelStack.Translate(-0.25f, 0.25f, 2.76f);
		//	modelStack.Scale(0.5f, 0.5f, 0.5f);
		//	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
		//	meshList[GEO_WARDROBE_CASE]->Render();
		//	modelStack.PopMatrix();
		//}

		modelStack.PushMatrix(); //left door support
		{
			modelStack.Translate(-4.24f, 8.27f, 2.26f);
			modelStack.Scale(0.5f, 15.5f, 0.5f);
			MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
			meshList[GEO_WARDROBE_CASE_SUPPORT]->Render();
			modelStack.PopMatrix();
		}

		modelStack.PushMatrix(); //left door support CUBE bottom
		{
			modelStack.Translate(-4.24f, 0.25f, 2.26f);
			modelStack.Scale(0.5f, 0.5f, 0.5f);
			MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
			meshList[GEO_WARDROBE_CASE]->Render();
			modelStack.PopMatrix();
		}

		modelStack.PushMatrix(); //left door support CUBE TOP
		{
			modelStack.Translate(-4.24f, 16.27f, 2.26f);
			modelStack.Scale(0.5f, 0.5f, 0.5f);
			MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
			meshList[GEO_WARDROBE_CASE]->Render();
			modelStack.PopMatrix();
		}

		modelStack.PushMatrix(); //left door support CUBE bottom
		{
			modelStack.Translate(4.24f, 0.25f, 2.26f);
			modelStack.Scale(0.5f, 0.5f, 0.5f);
			MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
			meshList[GEO_WARDROBE_CASE]->Render();
			modelStack.PopMatrix();
		}

		modelStack.PushMatrix(); //right door
		{
			modelStack.Translate(2.f, 8.25f, 2.26f);
			modelStack.Scale(4.f, 15.5f, 0.5f);
			MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
			meshList[GEO_WARDROBE_CASE]->Render();
			modelStack.PopMatrix();
		}

		modelStack.PushMatrix(); //right door bottom peice long
		{
			modelStack.Translate(2.f, 0.25f, 2.26f);
			modelStack.Scale(4.f, 0.5f, 0.5f);
			MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
			meshList[GEO_WARDROBE_CASE_SUPPORT]->Render();
			modelStack.PopMatrix();
		}
	

		modelStack.PushMatrix(); //left door support CUBE TOP
		{
			modelStack.Translate(4.24f, 16.27f, 2.26f);
			modelStack.Scale(0.5f, 0.5f, 0.5f);
			MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
			meshList[GEO_WARDROBE_CASE]->Render();
			modelStack.PopMatrix();
		}

		modelStack.PushMatrix(); //right door support
		{
			modelStack.Translate(4.24f, 8.27f, 2.26f);
			modelStack.Scale(0.5f, 15.5f, 0.5f);
			MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
			meshList[GEO_WARDROBE_CASE_SUPPORT]->Render();
			modelStack.PopMatrix();
		}

		modelStack.PushMatrix(); // top
		{
			modelStack.Translate(0.f, 16.27f, 0.f);
			modelStack.Scale(8.f, 0.5f, 4.f);
			MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
			meshList[GEO_WARDROBE_CASE]->Render();
			modelStack.PopMatrix();
		}

		modelStack.PushMatrix(); // top support
		{
			modelStack.Translate(0.f, 16.27f, 2.26f);
			modelStack.Scale(8.f, 0.5f, 0.5f);
			MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
			meshList[GEO_WARDROBE_CASE_SUPPORT]->Render();
			modelStack.PopMatrix();
		}

		modelStack.PushMatrix(); // right side
		{
			modelStack.Translate(4.24f, 8.25f, 0.f);
			modelStack.Scale(0.5f, 15.49f, 4.f);
			MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
			meshList[GEO_WARDROBE_CASE]->Render();
			modelStack.PopMatrix();
		}

		modelStack.PushMatrix(); // right side support top
		{
			modelStack.Translate(4.24f, 16.25f, 0.f);
			modelStack.Scale(0.5f, 0.51f, 4.f);
			MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
			meshList[GEO_WARDROBE_CASE_SUPPORT]->Render();
			modelStack.PopMatrix();
		}

		modelStack.PushMatrix(); // right side support bottom
		{
			modelStack.Translate(4.24f, 0.25f, 0.f);
			modelStack.Scale(0.5f, 0.51f, 4.f);
			MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
			meshList[GEO_WARDROBE_CASE_SUPPORT]->Render();
			modelStack.PopMatrix();
		}

		modelStack.PushMatrix(); // left side
		{
			modelStack.Translate(-4.24f, 8.25f, 0.f);
			modelStack.Scale(0.5f, 15.49f, 4.f);
			MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
			meshList[GEO_WARDROBE_CASE]->Render();
			modelStack.PopMatrix();
		}

		modelStack.PushMatrix(); // left side support top
		{
			modelStack.Translate(-4.24f, 16.25f, 0.f);
			modelStack.Scale(0.5f, 0.51f, 4.f);
			MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
			meshList[GEO_WARDROBE_CASE_SUPPORT]->Render();
			modelStack.PopMatrix();
		}

		modelStack.PushMatrix(); // left side support bottom
		{
			modelStack.Translate(-4.24f, 0.25f, 0.f);
			modelStack.Scale(0.5f, 0.51f, 4.f);
			MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
			meshList[GEO_WARDROBE_CASE_SUPPORT]->Render();
			modelStack.PopMatrix();
		}
	}
	modelStack.PopMatrix();

	modelStack.PushMatrix(); // window
	{
		modelStack.Translate(-9.73f, 0.f, 0.f);

		modelStack.PushMatrix(); // top window frame
		{

			modelStack.Translate(0.f, 18.f, 0.f);
			modelStack.Scale(0.5f, 0.5f, 16.f);
			MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
			meshList[Windows_Frame]->Render();
			modelStack.PopMatrix();
		}

		modelStack.PushMatrix(); // bottom window frame
		{

			modelStack.Translate(0.f, 4.f, 0.f);
			modelStack.Scale(0.5f, 0.5f, 16.f);
			MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
			meshList[Windows_Frame]->Render();
			modelStack.PopMatrix();
		}

		modelStack.PushMatrix(); // window frame middle
		{

			modelStack.Translate(0.f, 11.25f, 0.f);
			modelStack.Scale(0.5f, 14.f, 0.5f);
			MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
			meshList[Windows_Frame]->Render();
			modelStack.PopMatrix();
		}

		modelStack.PushMatrix(); // window frame middle left
		{

			modelStack.Translate(0.f, 11.25f, 4.f);
			modelStack.Scale(0.5f, 14.f, 0.5f);
			MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
			meshList[Windows_Frame]->Render();
			modelStack.PopMatrix();
		}

		modelStack.PushMatrix(); // window frame middle right
		{

			modelStack.Translate(0.f, 11.25f, -4.f);
			modelStack.Scale(0.5f, 14.f, 0.5f);
			MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
			meshList[Windows_Frame]->Render();
			modelStack.PopMatrix();
		}

		modelStack.PushMatrix(); // window frame right
		{

			modelStack.Translate(0.f, 11.25f, -7.75f);
			modelStack.Scale(0.5f, 14.f, 0.5f);
			MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
			meshList[Windows_Frame]->Render();
			modelStack.PopMatrix();
		}

		modelStack.PushMatrix(); // window frame left
		{

			modelStack.Translate(0.f, 11.25f, 7.75f);
			modelStack.Scale(0.5f, 14.f, 0.5f);
			MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
			meshList[Windows_Frame]->Render();
			modelStack.PopMatrix();
		}
		
		modelStack.PushMatrix(); // bottom window frame cross middle
		{

			modelStack.Translate(0.f, 11.f, 0.f);
			modelStack.Scale(0.5f, 0.5f, 16.f);
			MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
			meshList[Windows_Frame]->Render();
			modelStack.PopMatrix();
		}
		
		modelStack.PushMatrix(); // glass panes R1 Q2
		{

			modelStack.Translate(0.f, 14.5f, 2.f);
			modelStack.Scale(0.1f, 6.5f, 3.5f);
			MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
			meshList[Glass_Panes]->Render();
			modelStack.PopMatrix();
		}

		modelStack.PushMatrix(); // glass support panes left
		{

			modelStack.Translate(0.15f, 14.5f, 3.6f);
			modelStack.Scale(0.17f, 6.5f, 0.3f);
			MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
			meshList[Window_Acc]->Render();
			modelStack.PopMatrix();
		}

		modelStack.PushMatrix(); // glass support panes right
		{

			modelStack.Translate(0.15f, 14.5f, 0.4f);
			modelStack.Scale(0.17f, 6.5f, 0.3f);
			MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
			meshList[Window_Acc]->Render();
			modelStack.PopMatrix();
		}

		modelStack.PushMatrix(); // glass support panes bottom
		{

			modelStack.Translate(0.15f, 11.4f, 2.f);
			modelStack.Scale(0.17f, 0.3f, 3.f);
			MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
			meshList[Window_Acc]->Render();
			modelStack.PopMatrix();
		}

		modelStack.PushMatrix(); // glass support panes top
		{

			modelStack.Translate(0.15f, 17.6f, 2.f);
			modelStack.Scale(0.17f, 0.3f, 3.f);
			MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
			meshList[Window_Acc]->Render();
			modelStack.PopMatrix();
		}

		modelStack.PushMatrix(); // glass support panes right handel
		{

			modelStack.Translate(0.1f, 14.7f, 3.3f);
			modelStack.Scale(0.1f, 0.2f, 0.7f);
			MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
			meshList[Window_Handel]->Render();
			modelStack.PopMatrix();
		}

		modelStack.PushMatrix(); // glass support panes right handel
		{

			modelStack.Translate(0.1f, 14.5f, 2.8f);
			modelStack.Rotate(135, 1, 0, 0);
			modelStack.Scale(0.1f, 0.2f, 0.7f);
			MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
			meshList[Window_Handel]->Render();
			modelStack.PopMatrix();
		}
		/// <summary>
		///****************************///
		/// </summary>
		modelStack.PushMatrix();
		{

			modelStack.Translate(0.f, 14.5f, 7.5f);
			modelStack.Rotate(-(sin(window_rotate)*45)-45, 0, 1, 0);
			modelStack.Translate(0, 0, -1.5);
			modelStack.PushMatrix(); // glass panes R1 Q1 // Rotate
		{

			
			modelStack.Scale(0.1f, 6.5f, 3.5f);
			MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
			meshList[Glass_Panes]->Render();
			modelStack.PopMatrix();
		}
		

		modelStack.PushMatrix(); // glass support panes left handel
		{

			modelStack.Translate(0.1f, 0.2f, -1.3f);
			modelStack.Scale(0.1f, 0.2f, 0.7f);
			MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
			meshList[Window_Handel]->Render();
			modelStack.PopMatrix();
		}

		modelStack.PushMatrix(); // glass support panes left handel
		{

			modelStack.Translate(0.1f, 0.f, -0.8f);
			modelStack.Rotate(45, 1, 0, 0);
			modelStack.Scale(0.1f, 0.2f, 0.7f);
			MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
			meshList[Window_Handel]->Render();
			modelStack.PopMatrix();
		}

			


		}

		modelStack.PopMatrix();
		//modelStack.PushMatrix(); // glass panes R1 Q1 // Rotate
		//{

		//	modelStack.Translate(0.f, 14.5f, 6.f);
		//	modelStack.Scale(0.1f, 6.5f, 3.5f);
		//	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
		//	meshList[Glass_Panes]->Render();
		//	modelStack.PopMatrix();
		//}
		//

		//modelStack.PushMatrix(); // glass support panes left handel
		//{

		//	modelStack.Translate(0.1f, 14.7f, 4.7f);
		//	modelStack.Scale(0.1f, 0.2f, 0.7f);
		//	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
		//	meshList[Window_Handel]->Render();
		//	modelStack.PopMatrix();
		//}

		//modelStack.PushMatrix(); // glass support panes left handel
		//{

		//	modelStack.Translate(0.1f, 14.5f, 5.2f);
		//	modelStack.Rotate(45, 1, 0, 0);
		//	modelStack.Scale(0.1f, 0.2f, 0.7f);
		//	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
		//	meshList[Window_Handel]->Render();
		//	modelStack.PopMatrix();
		//}

		modelStack.PushMatrix(); // glass support panes left 
		{

			modelStack.Translate(0.15f, 14.5f, 7.35f);
			modelStack.Scale(0.17f, 6.5f, 0.3f);
			MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
			meshList[Window_Acc]->Render();
			modelStack.PopMatrix();
		}
		modelStack.PushMatrix(); // glass support panes right
		{

			modelStack.Translate(0.15f, 14.5f, 4.36f);
			modelStack.Scale(0.17f, 6.5f, 0.3f);
			MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
			meshList[Window_Acc]->Render();
			modelStack.PopMatrix();
		}

		modelStack.PushMatrix(); // glass support panes bottom
		{

			modelStack.Translate(0.15f, 11.4f, 5.85f);
			modelStack.Scale(0.17f, 0.3f, 2.74f);
			MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
			meshList[Window_Acc]->Render();
			modelStack.PopMatrix();
		}

		modelStack.PushMatrix(); // glass support panes top
		{

			modelStack.Translate(0.15f, 17.6f, 5.85f);
			modelStack.Scale(0.17f, 0.3f, 2.74f);
			MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
			meshList[Window_Acc]->Render();
			modelStack.PopMatrix();
		}

		/// <summary>
		///****************************///
		/// </summary>
		modelStack.PushMatrix(); // glass panes R2 Q1
		{

			modelStack.Translate(0.f, 7.5f, 6.f);
			modelStack.Scale(0.1f, 6.5f, 3.5f);
			MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
			meshList[Glass_Panes]->Render();
			modelStack.PopMatrix();
		}
		modelStack.PushMatrix(); // glass support panes left
		{

			modelStack.Translate(0.15f, 7.5f, 7.35f);
			modelStack.Scale(0.17f, 6.5f, 0.3f);
			MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
			meshList[Window_Acc]->Render();
			modelStack.PopMatrix();
		}

		modelStack.PushMatrix(); // glass support panes right
		{

			modelStack.Translate(0.15f, 7.5f, 4.36f);
			modelStack.Scale(0.17f, 6.5f, 0.3f);
			MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
			meshList[Window_Acc]->Render();
			modelStack.PopMatrix();
		}

		modelStack.PushMatrix(); // glass support panes bottom
		{

			modelStack.Translate(0.15f, 4.4f, 5.85f);
			modelStack.Scale(0.17f, 0.3f, 2.74f);
			MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
			meshList[Window_Acc]->Render();
			modelStack.PopMatrix();
		}

		modelStack.PushMatrix(); // glass support panes top
		{

			modelStack.Translate(0.15f, 10.6f, 5.85f);
			modelStack.Scale(0.17f, 0.3f, 2.74f);
			MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
			meshList[Window_Acc]->Render();
			modelStack.PopMatrix();
		}

		/// <summary>
		///****************************///
		/// </summary>
		modelStack.PushMatrix(); // glass panes R2 Q2
		{

			modelStack.Translate(0.f, 7.5f, 2.f);
			modelStack.Scale(0.1f, 6.5f, 3.5f);
			MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
			meshList[Glass_Panes]->Render();
			modelStack.PopMatrix();
		}

		modelStack.PushMatrix(); // glass support panes left
		{

			modelStack.Translate(0.15f, 7.5f, 3.6f);
			modelStack.Scale(0.17f, 6.5f, 0.3f);
			MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
			meshList[Window_Acc]->Render();
			modelStack.PopMatrix();
		}

		modelStack.PushMatrix(); // glass support panes right
		{

			modelStack.Translate(0.15f, 7.5f, 0.4f);
			modelStack.Scale(0.17f, 6.5f, 0.3f);
			MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
			meshList[Window_Acc]->Render();
			modelStack.PopMatrix();
		}

		modelStack.PushMatrix(); // glass support panes bottom
		{

			modelStack.Translate(0.15f, 4.4f, 2.f);
			modelStack.Scale(0.17f, 0.3f, 3.f);
			MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
			meshList[Window_Acc]->Render();
			modelStack.PopMatrix();
		}

		modelStack.PushMatrix(); // glass support panes top
		{

			modelStack.Translate(0.15f, 10.6f, 2.f);
			modelStack.Scale(0.17f, 0.3f, 3.f);
			MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
			meshList[Window_Acc]->Render();
			modelStack.PopMatrix();
		}
		/// <summary>
		///****************************///
		/// </summary>
		modelStack.PushMatrix(); // glass panes R1 Q4
		{

			modelStack.Translate(0.f, 14.5f, -6.f);
			modelStack.Scale(0.1f, 6.5f, 3.5f);
			MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
			meshList[Glass_Panes]->Render();
			modelStack.PopMatrix();
		}
		modelStack.PushMatrix(); // glass support panes left
		{

			modelStack.Translate(0.15f, 14.5f, -7.35f);
			modelStack.Scale(0.17f, 6.5f, 0.3f);
			MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
			meshList[Window_Acc]->Render();
			modelStack.PopMatrix();
		}

		modelStack.PushMatrix(); // glass support panes right
		{

			modelStack.Translate(0.15f, 14.5f, -4.36f);
			modelStack.Scale(0.17f, 6.5f, 0.3f);
			MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
			meshList[Window_Acc]->Render();
			modelStack.PopMatrix();
		}

		modelStack.PushMatrix(); // glass support panes bottom
		{

			modelStack.Translate(0.15f, 11.4f, -5.85f);
			modelStack.Scale(0.17f, 0.3f, 2.74f);
			MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
			meshList[Window_Acc]->Render();
			modelStack.PopMatrix();
		}

		modelStack.PushMatrix(); // glass support panes top
		{

			modelStack.Translate(0.15f, 17.6f, -5.85f);
			modelStack.Scale(0.17f, 0.3f, 2.74f);
			MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
			meshList[Window_Acc]->Render();
			modelStack.PopMatrix();
		}

		modelStack.PushMatrix(); // glass support panes left handel
		{

			modelStack.Translate(0.1f, 14.7f, -4.7f);
			modelStack.Scale(0.1f, 0.2f, 0.7f);
			MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
			meshList[Window_Handel]->Render();
			modelStack.PopMatrix();
		}

		modelStack.PushMatrix(); // glass support panes left handel
		{

			modelStack.Translate(0.1f, 14.5f, -5.2f);
			modelStack.Rotate(135, 1, 0, 0);
			modelStack.Scale(0.1f, 0.2f, 0.7f);
			MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
			meshList[Window_Handel]->Render();
			modelStack.PopMatrix();
		}

		/// <summary>
		///****************************///
		/// </summary>
		modelStack.PushMatrix(); // glass panes R2 Q3
		{

			modelStack.Translate(0.f, 7.5f, -2.f);
			modelStack.Scale(0.1f, 6.5f, 3.5f);
			MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
			meshList[Glass_Panes]->Render();
			modelStack.PopMatrix();
		}

		modelStack.PushMatrix(); // glass support panes left
		{

			modelStack.Translate(0.15f, 7.5f, -3.6f);
			modelStack.Scale(0.17f, 6.5f, 0.3f);
			MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
			meshList[Window_Acc]->Render();
			modelStack.PopMatrix();
		}

		modelStack.PushMatrix(); // glass support panes right
		{

			modelStack.Translate(0.15f, 7.5f, -0.4f);
			modelStack.Scale(0.17f, 6.5f, 0.3f);
			MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
			meshList[Window_Acc]->Render();
			modelStack.PopMatrix();
		}

		modelStack.PushMatrix(); // glass support panes bottom
		{

			modelStack.Translate(0.15f, 4.4f, -2.f);
			modelStack.Scale(0.17f, 0.3f, 3.f);
			MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
			meshList[Window_Acc]->Render();
			modelStack.PopMatrix();
		}

		modelStack.PushMatrix(); // glass support panes top
		{

			modelStack.Translate(0.15f, 10.6f, -2.f);
			modelStack.Scale(0.17f, 0.3f, 3.f);
			MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
			meshList[Window_Acc]->Render();
			modelStack.PopMatrix();
		}

		/// <summary>
		///****************************///
		/// </summary>
		modelStack.PushMatrix(); // glass panes R2 Q4
		{

			modelStack.Translate(0.f, 7.5f, -6.f);
			modelStack.Scale(0.1f, 6.5f, 3.5f);
			MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
			meshList[Glass_Panes]->Render();
			modelStack.PopMatrix();
		}
		modelStack.PushMatrix(); // glass support panes left
		{

			modelStack.Translate(0.15f, 7.5f, -7.35f);
			modelStack.Scale(0.17f, 6.5f, 0.3f);
			MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
			meshList[Window_Acc]->Render();
			modelStack.PopMatrix();
		}

		modelStack.PushMatrix(); // glass support panes right
		{

			modelStack.Translate(0.15f, 7.5f, -4.36f);
			modelStack.Scale(0.17f, 6.5f, 0.3f);
			MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
			meshList[Window_Acc]->Render();
			modelStack.PopMatrix();
		}

		modelStack.PushMatrix(); // glass support panes bottom
		{

			modelStack.Translate(0.15f, 4.4f, -5.85f);
			modelStack.Scale(0.17f, 0.3f, 2.74f);
			MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
			meshList[Window_Acc]->Render();
			modelStack.PopMatrix();
		}

		modelStack.PushMatrix(); // glass support panes top
		{

			modelStack.Translate(0.15f, 10.6f, -5.85f);
			modelStack.Scale(0.17f, 0.3f, 2.74f);
			MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
			meshList[Window_Acc]->Render();
			modelStack.PopMatrix();
		}
		/// <summary>
		/// ////
		/// </summary>
		modelStack.PushMatrix(); // glass panes R1 Q3
		{

			modelStack.Translate(0.f, 14.5f, -2.f);
			modelStack.Scale(0.1f, 6.5f, 3.5f);
			MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
			meshList[Glass_Panes]->Render();
			modelStack.PopMatrix();
		}

		modelStack.PushMatrix(); // glass support panes left
		{

			modelStack.Translate(0.15f, 14.5f, -3.6f);
			modelStack.Scale(0.17f, 6.5f, 0.3f);
			MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
			meshList[Window_Acc]->Render();
			modelStack.PopMatrix();
		}

		modelStack.PushMatrix(); // glass support panes right
		{

			modelStack.Translate(0.15f, 14.5f, -0.4f);
			modelStack.Scale(0.17f, 6.5f, 0.3f);
			MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
			meshList[Window_Acc]->Render();
			modelStack.PopMatrix();
		}

		modelStack.PushMatrix(); // glass support panes bottom
		{

			modelStack.Translate(0.15f, 11.4f, -2.f);
			modelStack.Scale(0.17f, 0.3f, 3.f);
			MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
			meshList[Window_Acc]->Render();
			modelStack.PopMatrix();
		}

		modelStack.PushMatrix(); // glass support panes top
		{

			modelStack.Translate(0.15f, 17.6f, -2.f);
			modelStack.Scale(0.17f, 0.3f, 3.f);
			MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
			meshList[Window_Acc]->Render();
			modelStack.PopMatrix();
		}

		modelStack.PushMatrix(); // glass support panes left handel
		{

			modelStack.Translate(0.1f, 14.7f, -3.3f);
			modelStack.Scale(0.1f, 0.2f, 0.7f);
			MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
			meshList[Window_Handel]->Render();
			modelStack.PopMatrix();
		}

		modelStack.PushMatrix(); // glass support panes left handel
		{

			modelStack.Translate(0.1f, 14.5f, -2.8f);
			modelStack.Rotate(45, 1, 0, 0);
			modelStack.Scale(0.1f, 0.2f, 0.7f);
			MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
			meshList[Window_Handel]->Render();
			modelStack.PopMatrix();
		}

	}
	modelStack.PopMatrix();

	modelStack.PushMatrix(); // Sheveles 
	{
		modelStack.Translate(-5.5f, -8.5f, -9.7f);
		modelStack.Scale(1.25f, 1.25f, 1.25f);
		modelStack.PushMatrix();// right part shevle
		{
			modelStack.Translate(0.92f, 8.1f, 1.f);
			modelStack.Scale(0.15f, 2.2f, 2.2f);
			MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
			meshList[Shevlevs_C]->Render();

			modelStack.PopMatrix();
		}

		modelStack.PushMatrix();// left part shevle
		{
			modelStack.Translate(-0.92f, 8.1f, 1.f);
			modelStack.Scale(0.15f, 2.2f, 2.2f);
			MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
			meshList[shevles_Case]->Render();

			modelStack.PopMatrix();
		}

		modelStack.PushMatrix();// top shevels
		{
			modelStack.Translate(0.f, 9.3f, 1.f);
			modelStack.Scale(2.f, 0.2f, 2.17f);

			MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
			meshList[shevles_Case]->Render();

			modelStack.PopMatrix();
		}

		modelStack.PushMatrix();// bottom shevels
		{
			modelStack.Translate(0.f, 6.9f, 1.f);
			modelStack.Scale(2.f, 0.2f, 2.17f);
			MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
			meshList[Shevlevs_C]->Render();

			modelStack.PopMatrix();
		}
		// right bottom
		modelStack.PushMatrix();// right part shevle 1 right 
		{
			modelStack.Translate(2.92f, 8.1f, 1.f);
			modelStack.Scale(0.15f, 2.2f, 2.2f);
			MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
			meshList[Shevlevs_C]->Render();

			modelStack.PopMatrix();
		}


		modelStack.PushMatrix();// top shevels
		{
			modelStack.Translate(2.f, 9.3f, 1.f);
			modelStack.Scale(2.f, 0.2f, 2.17f);

			MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
			meshList[Shevlevs_C]->Render();

			modelStack.PopMatrix();
		}

		modelStack.PushMatrix();// bottom shevels
		{
			modelStack.Translate(2.f, 6.9f, 1.f);
			modelStack.Scale(2.f, 0.2f, 2.17f);
			MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
			meshList[Shevlevs_C]->Render();

			modelStack.PopMatrix();
		}
		// right top

		modelStack.PushMatrix();// left part shevle
		{
			modelStack.Translate(0.92f, 10.5f, 1.f);
			modelStack.Scale(0.15f, 2.2f, 2.2f);
			MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
			meshList[shevles_Case]->Render();

			modelStack.PopMatrix();
		}

		modelStack.PushMatrix();// bottom shevels
		{
			modelStack.Translate(1.85f, 11.7f, 1.f);
			modelStack.Scale(2.f, 0.2f, 2.17f);
			MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
			meshList[shevles_Case]->Render();

			modelStack.PopMatrix();
		}

		modelStack.PushMatrix();// right part shevle 1 right 
		{
			modelStack.Translate(2.92f, 10.5f, 1.f);
			modelStack.Scale(0.15f, 2.2f, 2.2f);
			MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
			meshList[Shevlevs_C]->Render();

			modelStack.PopMatrix();
		}
		// right right
		modelStack.PushMatrix();// right part shevle
		{
			modelStack.Translate(4.92f, 8.1f, 1.f);
			modelStack.Scale(0.15f, 2.2f, 2.2f);
			MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
			meshList[Shevlevs_C]->Render();

			modelStack.PopMatrix();
		}

		modelStack.PushMatrix();// bottom shevels
		{
			modelStack.Translate(4.f, 6.9f, 1.f);
			modelStack.Scale(2.f, 0.2f, 2.17f);
			MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
			meshList[Shevlevs_C]->Render();

			modelStack.PopMatrix();
		}

		modelStack.PushMatrix();// top shevels
		{
			modelStack.Translate(4.f, 9.3f, 1.f);
			modelStack.Scale(2.f, 0.2f, 2.17f);

			MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
			meshList[Shevlevs_C]->Render();

			modelStack.PopMatrix();
		}

		modelStack.PushMatrix();// right part shevle 1 right 
		{
			modelStack.Translate(4.92f, 10.5f, 1.f);
			modelStack.Scale(0.15f, 2.2f, 2.2f);
			MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
			meshList[Shevlevs_C]->Render();

			modelStack.PopMatrix();
		}

		modelStack.PushMatrix();// top shevels
		{
			modelStack.Translate(4.f, 11.5f, 1.f);
			modelStack.Scale(2.f, 0.2f, 2.17f);

			MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
			meshList[Shevlevs_C]->Render();

			modelStack.PopMatrix();
		}
		// right most top

		modelStack.PushMatrix();// right part shevle 1 right 
		{
			modelStack.Translate(2.92f, 12.7f, 1.f);
			modelStack.Scale(0.15f, 2.2f, 2.2f);
			MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
			meshList[shevles_Case]->Render();

			modelStack.PopMatrix();
		}

		modelStack.PushMatrix();// right part shevle 1 right 
		{
			modelStack.Translate(4.92f, 12.5f, 1.f);
			modelStack.Scale(0.15f, 2.2f, 2.2f);
			MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
			meshList[Shevlevs_C]->Render();

			modelStack.PopMatrix();
		}

		modelStack.PushMatrix();// top shevels
		{
			modelStack.Translate(4.f, 13.7f, 1.f);
			modelStack.Scale(2.f, 0.2f, 2.17f);

			MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
			meshList[shevles_Case]->Render();

			modelStack.PopMatrix();
		}
	}
	modelStack.PopMatrix();

	modelStack.PushMatrix(); // PC and Table
	{
		modelStack.Translate(-7.9f, -3.7f, 5.f);
		modelStack.PushMatrix(); // PC BOX
		{
			modelStack.Translate(0.f, 9.3f, -2.4f);
			modelStack.Scale(2.f, 2.f, 1.f);
			MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
			meshList[PC_BOX]->Render();
			modelStack.PopMatrix();
		}

			modelStack.PushMatrix(); // Monitor leg
		{
			modelStack.Translate(-1.f, 8.3f, 0.f);
			modelStack.Scale(0.2f, 0.1f, 1.f);
			MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
			meshList[PC_BOX]->Render();
			modelStack.PopMatrix();
		}

		modelStack.PushMatrix(); // Monitor stand
		{
			modelStack.Translate(-1.f, 8.4f, 0.f);
			modelStack.Scale(0.2f, 1.f, 0.3f);
			MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
			meshList[PC_BOX]->Render();
			modelStack.PopMatrix();
		}

		modelStack.PushMatrix(); // Monitor frame bottom
		{
			modelStack.Translate(-1.f, 8.9f, 0.f);
			modelStack.Scale(0.2f, 0.1f, 3.5f);
			MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
			meshList[PC_BOX]->Render();
			modelStack.PopMatrix();
		}

		modelStack.PushMatrix(); // Monitor frame bottom
		{
			modelStack.Translate(-1.f, 11.5f, 0.f);
			modelStack.Scale(0.2f, 0.1f, 3.5f);
			MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
			meshList[PC_BOX]->Render();
			modelStack.PopMatrix();
		}

		modelStack.PushMatrix(); // Monitor screen
		{
			modelStack.Translate(-1.f, 10.2f, 0.f);
			modelStack.Scale(0.2f, 2.52f, 3.3f);
			MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
			meshList[Table]->Render();
			modelStack.PopMatrix();
		}

		modelStack.PushMatrix(); // Monitor frame left
		{
			modelStack.Translate(-1.f, 10.2f, 1.7f);
			modelStack.Scale(0.2f, 2.5f, 0.1f);
			MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
			meshList[PC_BOX]->Render();
			modelStack.PopMatrix();
		}

		modelStack.PushMatrix(); // Monitor frame right
		{
			modelStack.Translate(-1.f, 10.2f, -1.7f);
			modelStack.Scale(0.2f, 2.5f, 0.1f);
			MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
			meshList[PC_BOX]->Render();
			modelStack.PopMatrix();
		}

		modelStack.PushMatrix(); // tabel top
		{
			modelStack.Translate(0.f, 8.f, 0.f);
			modelStack.Scale(3.f, 0.5f, 6.f);
			MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
			meshList[Table]->Render();
			modelStack.PopMatrix();
		}

		modelStack.PushMatrix(); //left leg bottom
		{
			modelStack.Translate(1.f, 5.7f, 2.5f);
			modelStack.Scale(0.2f, 2.f, 0.2f);
			MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
			meshList[Table_Legs]->Render();
			modelStack.PopMatrix();
		}

		modelStack.PushMatrix(); //left leg top
		{
			modelStack.Translate(-1.f, 5.7f, 2.5f);
			modelStack.Scale(0.2f, 2.f, 0.2f);
			MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
			meshList[Table_Legs]->Render();
			modelStack.PopMatrix();
		}

		modelStack.PushMatrix(); //right leg bottom
		{
			modelStack.Translate(1.f, 5.7f, -2.5f);
			modelStack.Scale(0.2f, 2.f, 0.2f);
			MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
			meshList[Table_Legs]->Render();
			modelStack.PopMatrix();
		}

		modelStack.PushMatrix(); //right leg top
		{
			modelStack.Translate(-1.f, 5.7f, -2.5f);
			modelStack.Scale(0.2f, 2.f, 0.2f);
			MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
			meshList[Table_Legs]->Render();
			modelStack.PopMatrix();
		}
		modelStack.PopMatrix();
	}

	
	modelStack.PushMatrix(); // Fan
	{
		modelStack.Translate(0.f, 9.5f, 0.f);
		modelStack.Scale(1.3f, 1.3f, 1.3f);
		modelStack.Rotate(fan_rotate, 0, 1, 0);
		modelStack.PushMatrix(); // Fan body
		{
			modelStack.Translate(0.f, 8.f, 0.f);
			modelStack.Scale(1.f, 0.3f, 1.f);
			MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
			meshList[fan_body]->Render();
			modelStack.PopMatrix();
		}

		modelStack.PushMatrix(); // Fan blade on positive z axis
		{
			modelStack.Translate(0.f, 8.f, 3.f);

			modelStack.Scale(0.5f, 0.5f, 5.5f);
			MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
			meshList[fan_blades]->Render();
			modelStack.PopMatrix();
		}

		modelStack.PushMatrix(); // Fan blade on posive x axis
		{
			modelStack.Translate(3.f, 8.f, 0.f);
			modelStack.Scale(5.5f, 0.5f, 0.5f);
			MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
			meshList[fan_blades]->Render();
			modelStack.PopMatrix();
		}

		modelStack.PushMatrix(); // Fan blade on negative z axis
		{
			modelStack.Translate(0.f, 8.f, -3.f);
			modelStack.Scale(0.5f, 0.5f, 5.5f);
			MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
			meshList[fan_blades]->Render();
			modelStack.PopMatrix();
		}

		modelStack.PushMatrix(); // Fan blade on neagative x axis
		{
			modelStack.Translate(-3.f, 8.f, 0.f);
			modelStack.Scale(5.5f, 0.5f, 0.5f);
			MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
			meshList[fan_blades]->Render();
			modelStack.PopMatrix();
		}

	
	}
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	{
		modelStack.Scale(2, 2, 2);
		
		modelStack.Translate(-2.f, -6.4f, 2.7f);
		modelStack.Translate(chair_move_x, 0, chair_move_z);
		


		modelStack.PushMatrix();
		{

			modelStack.Rotate(chair_rotate, 0, 1, 0);
			modelStack.PushMatrix(); // Chair seet
			{
				modelStack.Translate(0.f, 8.f, 0.f);
				modelStack.Scale(1.f, 0.25f, 1.f);
				MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
				meshList[Chair_body]->Render();
				modelStack.PopMatrix();
			}

			modelStack.PushMatrix(); // Chair back rest
			{
				modelStack.Translate(0.f, 8.87f, -0.5f);
				modelStack.Scale(1.f, 2.f, 0.25f);
				MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
				meshList[Chair_body]->Render();
				modelStack.PopMatrix();
			}


			modelStack.PushMatrix(); // Chair right
			{
				modelStack.Translate(0.63f, 8.12f, 0.f);
				modelStack.Scale(0.2f, 0.1f, 1.f);
				modelStack.Rotate(45, 0.f, 0.f, 1.f);
				MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
				meshList[Chair_mids]->Render();
				modelStack.PopMatrix();
			}

			modelStack.PushMatrix(); // Chair left
			{
				modelStack.Translate(-0.63f, 8.12f, 0.f);
				modelStack.Scale(0.2f, 0.1f, 1.f);
				modelStack.Rotate(-45, 0.f, 0.f, 1.f);
				MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
				meshList[Chair_mids]->Render();
				modelStack.PopMatrix();
			}

			modelStack.PushMatrix(); // Chair support
			{
				modelStack.Translate(0.f, 7.3f, 0.f);
				modelStack.Scale(0.2f, 1.f, 0.2f);
				MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
				meshList[Chair_support]->Render();
				modelStack.PopMatrix();
			}

			modelStack.PushMatrix(); // Chair arm rest support +ve X L
			{
				modelStack.Translate(-0.4f, 7.5f, 0.f);
				modelStack.Scale(1.f, 0.2f, 0.2f);
				MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
				meshList[Chair_support]->Render();
				modelStack.PopMatrix();
			}

			modelStack.PushMatrix(); // Chair arm rest support +ve X I
			{
				modelStack.Translate(-0.85f, 8.f, 0.f);
				modelStack.Scale(0.1f, 1.f, 0.2f);
				MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
				meshList[Chair_support]->Render();
				modelStack.PopMatrix();
			}

			modelStack.PushMatrix(); // Chair arm rest support +ve X ----
			{
				modelStack.Translate(-0.85f, 8.5f, 0.f);
				modelStack.Scale(0.3f, 0.2f, 1.f);
				MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
				meshList[Chair_support]->Render();
				modelStack.PopMatrix();
			}

			modelStack.PushMatrix(); // Chair arm rest support -ve X L
			{
				modelStack.Translate(-0.4f, 7.5f, 0.f);
				modelStack.Scale(1.f, 0.2f, 0.2f);
				MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
				meshList[Chair_support]->Render();
				modelStack.PopMatrix();
			}

			modelStack.PushMatrix(); // Chair arm rest support -ve X I
			{
				modelStack.Translate(-0.85f, 8.f, 0.f);
				modelStack.Scale(0.1f, 1.f, 0.2f);
				MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
				meshList[Chair_support]->Render();
				modelStack.PopMatrix();
			}

			modelStack.PushMatrix(); // Chair arm rest support -ve X ----
			{
				modelStack.Translate(-0.85f, 8.5f, 0.f);
				modelStack.Scale(0.3f, 0.2f, 1.f);
				MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
				meshList[Chair_support]->Render();
				modelStack.PopMatrix();
			}

			modelStack.PushMatrix(); // Chair arm rest support +ve X L
			{
				modelStack.Translate(0.4f, 7.5f, 0.f);
				modelStack.Scale(1.f, 0.2f, 0.2f);
				MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
				meshList[Chair_support]->Render();
				modelStack.PopMatrix();
			}

			modelStack.PushMatrix(); // Chair arm rest support +ve X I
			{
				modelStack.Translate(0.85f, 8.f, 0.f);
				modelStack.Scale(0.1f, 1.f, 0.2f);
				MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
				meshList[Chair_support]->Render();
				modelStack.PopMatrix();
			}

			modelStack.PushMatrix(); // Chair arm rest support +ve X ----
			{
				modelStack.Translate(0.85f, 8.5f, 0.f);
				modelStack.Scale(0.3f, 0.2f, 1.f);
				MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
				meshList[Chair_support]->Render();
				modelStack.PopMatrix();
			}

			modelStack.PushMatrix(); // Chair arm rest support -ve X L
			{
				modelStack.Translate(0.4f, 7.5f, 0.f);
				modelStack.Scale(1.f, 0.2f, 0.2f);
				MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
				meshList[Chair_support]->Render();
				modelStack.PopMatrix();
			}

			modelStack.PushMatrix(); // Chair arm rest support -ve X I
			{
				modelStack.Translate(0.85f, 8.f, 0.f);
				modelStack.Scale(0.1f, 1.f, 0.2f);
				MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
				meshList[Chair_support]->Render();
				modelStack.PopMatrix();
			}

			modelStack.PushMatrix(); // Chair arm rest support -ve X ----
			{
				modelStack.Translate(0.85f, 8.5f, 0.f);
				modelStack.Scale(0.3f, 0.2f, 1.f);
				MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
				meshList[Chair_support]->Render();
				modelStack.PopMatrix();
			}



		}

		modelStack.PopMatrix();
		//modelStack.PushMatrix(); // Chair seet
		//{
		//	modelStack.Translate(0.f, 8.f, 0.f);
		//	modelStack.Scale(1.f, 0.25f, 1.f);
		//	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
		//	meshList[Chair_body]->Render();
		//	modelStack.PopMatrix();
		//}

		//modelStack.PushMatrix(); // Chair back rest
		//{
		//	modelStack.Translate(0.f, 8.87f, -0.5f);
		//	modelStack.Scale(1.f, 2.f, 0.25f);
		//	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
		//	meshList[Chair_body]->Render();
		//	modelStack.PopMatrix();
		//}


		//modelStack.PushMatrix(); // Chair right
		//{
		//	modelStack.Translate(0.63f, 8.12f, 0.f);
		//	modelStack.Scale(0.2f, 0.1f, 1.f);
		//	modelStack.Rotate(45, 0.f, 0.f, 1.f);
		//	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
		//	meshList[Chair_mids]->Render();
		//	modelStack.PopMatrix();
		//}

		//modelStack.PushMatrix(); // Chair left
		//{
		//	modelStack.Translate(-0.63f, 8.12f, 0.f);
		//	modelStack.Scale(0.2f, 0.1f, 1.f);
		//	modelStack.Rotate(-45, 0.f, 0.f, 1.f);
		//	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
		//	meshList[Chair_mids]->Render();
		//	modelStack.PopMatrix();
		//}

		//modelStack.PushMatrix(); // Chair support
		//{
		//	modelStack.Translate(0.f, 7.3f, 0.f);
		//	modelStack.Scale(0.2f, 1.f, 0.2f);
		//	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
		//	meshList[Chair_support]->Render();
		//	modelStack.PopMatrix();
		//}

		//modelStack.PushMatrix(); // Chair arm rest support +ve X L
		//{
		//	modelStack.Translate(-0.4f, 7.5f, 0.f);
		//	modelStack.Scale(1.f, 0.2f, 0.2f);
		//	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
		//	meshList[Chair_support]->Render();
		//	modelStack.PopMatrix();
		//}

		//modelStack.PushMatrix(); // Chair arm rest support +ve X I
		//{
		//	modelStack.Translate(-0.85f, 8.f, 0.f);
		//	modelStack.Scale(0.1f, 1.f, 0.2f);
		//	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
		//	meshList[Chair_support]->Render();
		//	modelStack.PopMatrix();
		//}

		//modelStack.PushMatrix(); // Chair arm rest support +ve X ----
		//{
		//	modelStack.Translate(-0.85f, 8.5f, 0.f);
		//	modelStack.Scale(0.3f, 0.2f, 1.f);
		//	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
		//	meshList[Chair_support]->Render();
		//	modelStack.PopMatrix();
		//}

		//modelStack.PushMatrix(); // Chair arm rest support -ve X L
		//{
		//	modelStack.Translate(-0.4f, 7.5f, 0.f);
		//	modelStack.Scale(1.f, 0.2f, 0.2f);
		//	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
		//	meshList[Chair_support]->Render();
		//	modelStack.PopMatrix();
		//}

		//modelStack.PushMatrix(); // Chair arm rest support -ve X I
		//{
		//	modelStack.Translate(-0.85f, 8.f, 0.f);
		//	modelStack.Scale(0.1f, 1.f, 0.2f);
		//	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
		//	meshList[Chair_support]->Render();
		//	modelStack.PopMatrix();
		//}

		//modelStack.PushMatrix(); // Chair arm rest support -ve X ----
		//{
		//	modelStack.Translate(-0.85f, 8.5f, 0.f);
		//	modelStack.Scale(0.3f, 0.2f, 1.f);
		//	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
		//	meshList[Chair_support]->Render();
		//	modelStack.PopMatrix();
		//}

		//modelStack.PushMatrix(); // Chair arm rest support +ve X L
		//{
		//	modelStack.Translate(0.4f, 7.5f, 0.f);
		//	modelStack.Scale(1.f, 0.2f, 0.2f);
		//	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
		//	meshList[Chair_support]->Render();
		//	modelStack.PopMatrix();
		//}

		//modelStack.PushMatrix(); // Chair arm rest support +ve X I
		//{
		//	modelStack.Translate(0.85f, 8.f, 0.f);
		//	modelStack.Scale(0.1f, 1.f, 0.2f);
		//	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
		//	meshList[Chair_support]->Render();
		//	modelStack.PopMatrix();
		//}

		//modelStack.PushMatrix(); // Chair arm rest support +ve X ----
		//{
		//	modelStack.Translate(0.85f, 8.5f, 0.f);
		//	modelStack.Scale(0.3f, 0.2f, 1.f);
		//	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
		//	meshList[Chair_support]->Render();
		//	modelStack.PopMatrix();
		//}

		//modelStack.PushMatrix(); // Chair arm rest support -ve X L
		//{
		//	modelStack.Translate(0.4f, 7.5f, 0.f);
		//	modelStack.Scale(1.f, 0.2f, 0.2f);
		//	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
		//	meshList[Chair_support]->Render();
		//	modelStack.PopMatrix();
		//}

		//modelStack.PushMatrix(); // Chair arm rest support -ve X I
		//{
		//	modelStack.Translate(0.85f, 8.f, 0.f);
		//	modelStack.Scale(0.1f, 1.f, 0.2f);
		//	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
		//	meshList[Chair_support]->Render();
		//	modelStack.PopMatrix();
		//}

		//modelStack.PushMatrix(); // Chair arm rest support -ve X ----
		//{
		//	modelStack.Translate(0.85f, 8.5f, 0.f);
		//	modelStack.Scale(0.3f, 0.2f, 1.f);
		//	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
		//	meshList[Chair_support]->Render();
		//	modelStack.PopMatrix();
		//}
		modelStack.PushMatrix(); // Chair legs +ve Z
		{
			modelStack.Translate(0.f, 6.7f, 0.4f);
			modelStack.Scale(0.2f, 0.2f, 1.f);
			MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
			meshList[Chair_support]->Render();
			modelStack.PopMatrix();
		}

		modelStack.PushMatrix(); // Chair legs -ve Z
		{
			modelStack.Translate(0.f, 6.7f, -0.4f);
			modelStack.Scale(0.2f, 0.2f, 1.f);
			MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
			meshList[Chair_support]->Render();
			modelStack.PopMatrix();
		}

		modelStack.PushMatrix(); // Chair legs -ve x
		{
			modelStack.Translate(-0.4f, 6.7f, 0.f);
			modelStack.Scale(1.f, 0.2f, 0.2f);
			MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
			meshList[Chair_support]->Render();
			modelStack.PopMatrix();
		}

		modelStack.PushMatrix(); // Chair legs +ve x 
		{
			modelStack.Translate(0.4f, 6.7f, 0.f);
			modelStack.Scale(1.f, 0.2f, 0.2f);
			MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
			meshList[Chair_support]->Render();
			modelStack.PopMatrix();
		}

		modelStack.PushMatrix(); // Chair wheeals +ve x 
		{
			modelStack.Translate(0.8f, 6.5f, 0.f);
			modelStack.Scale(0.1f, 0.1f, 0.1f);
			MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
			meshList[Chair_wheels]->Render();
			modelStack.PopMatrix();
		}

		modelStack.PushMatrix(); // Chair wheeals -ve x 
		{
			modelStack.Translate(-0.8f, 6.5f, 0.f);
			modelStack.Scale(0.1f, 0.1f, 0.1f);
			MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
			meshList[Chair_wheels]->Render();
			modelStack.PopMatrix();
		}

		modelStack.PushMatrix(); // Chair wheels -ve Z
		{
			modelStack.Translate(0.f, 6.5f, -0.8f);
			modelStack.Scale(0.1f, 0.1f, 0.1f);
			MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
			meshList[Chair_wheels]->Render();
			modelStack.PopMatrix();
		}

		modelStack.PushMatrix(); // Chair wheels +ve Z
		{
			modelStack.Translate(0.f, 6.5f, 0.8f);
			modelStack.Scale(0.1f, 0.1f, 0.1f);
			MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
			meshList[Chair_wheels]->Render();
			modelStack.PopMatrix();
		}
	}
	modelStack.PopMatrix();
	//modelStack.PopMatrix();
	//Mtx44 translate, rotate, scale;
	//translate.SetToTranslation(0, 2, 1);
	//rotate.SetToRotation(45, 1, 0, 0);
	//scale.SetToScale(2, 2, 2);
	//model = translate * rotate * scale;
	//MVP = projection * view * model;
	//glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	///*meshList[GEO_QUAD]->Render();*/


	//Mtx44 translate, rotate, scale;
	///*scale.SetToScale(1.f, 1.f, 2.f);*/
	//scale.SetToScale(scaling.x, scaling.y, scaling.z);
	////scale.SetToIdentity(); //do not scale
	//// 45 is the angle 0,1,0 is XYZ etc.
	////rotate.SetToRotation(45.f, 0.f, 1.f, 0.f); //rotate 45deg abt y-axis
	//rotate.SetToRotation(angleY, 0.f, 1.f, 0.f);
	////rotate.SetToIdentity(); // do not rotate
	////translate.SetToTranslation(0.f, 1.f, 0.f);// move up 1 unit

	//translate.SetToTranslation(planePos.x, planePos.y, planePos.z);
	//model = translate * rotate * scale; // calc model matrix (world space)
	//MVP = projection * view * model; // compute MVP matrix
	//glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]); // send info to graphcis card
	//meshList[GEO_CUBE]->Render();

	// Chair
	//Mtx44 chairRotate;
	//chairRotate.SetToIdentity();
	//chairRotate.SetToRotation(angleY, 0.f, 1.f, 0.f);

	//Mtx44 chairScale;
	//chairScale.SetToIdentity();
	//chairScale.SetToScale(scaling.x, scaling.y, scaling.z);

	////SEAT
	//Mtx44 translate, rotate, scale;
	///*scale.SetToScale(1.f, 1.f, 2.f);*/
	//scale.SetToScale(1.f, 0.1, 1.f);
	////scale.SetToIdentity(); //do not scale // 45 is the angle 0,1,0 is XYZ etc.
	////rotate.SetToRotation(45.f, 0.f, 1.f, 0.f); //rotate 45deg abt y-axis
	////rotate.SetToIdentity(); // do not rotate
	////translate.SetToTranslation(0.f, 1.f, 0.f);// move up 1 unit

	//translate.SetToTranslation(0, 0, 0);
	//model = chairRotate * chairScale * translate * scale; // calc model matrix (world space)
	//MVP = projection * view * model; // compute MVP matrix
	//glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]); // send info to graphcis card
	//meshList[GEO_CUBE]->Render();

	// BACK
	/*scale.SetToScale(1.f, 1.f, 2.f);*/
	//scale.SetToScale(1.f, 1.f, 0.1);
	////scale.SetToIdentity(); //do not scale
	//// 45 is the angle 0,1,0 is XYZ etc.
	////rotate.SetToRotation(45.f, 0.f, 1.f, 0.f); //rotate 45deg abt y-axis
	////rotate.SetToIdentity(); // do not rotate
	////translate.SetToTranslation(0.f, 1.f, 0.f);// move up 1 unit

	//translate.SetToTranslation(0.f, 0.5, -0.45);
	//model = chairRotate * chairScale * translate * scale; // calc model matrix (world space)
	//MVP = projection * view * model; // compute MVP matrix
	//glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]); // send info to graphcis card
	//meshList[GEO_CUBE]->Render();

	//// Right arm rest
	//scale.SetToScale(0.1f, 0.5f, 0.5f);
	//translate.SetToTranslation(-0.45f, 0.3f, -0.15f);
	//model = chairRotate * chairScale * translate * scale; // calc model matrix (world space)
	//MVP = projection * view * model; // compute MVP matrix
	//glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]); // send info to graphcis card
	//meshList[GEO_REDCUBE]->Render();

	//// Left arm rest
	//scale.SetToScale(0.1f, 0.5f, 0.5f);
	//translate.SetToTranslation(0.45f, 0.3f, -0.15f);
	//model = chairRotate * chairScale * translate * scale; // calc model matrix (world space)
	//MVP = projection * view * model; // compute MVP matrix
	//glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]); // send info to graphcis card
	//meshList[GEO_REDCUBE]->Render();

	//// support pole
	//translate.SetToTranslation(0.f, -0.5f, 0.f);
	//scale.SetToScale(0.25f, 1.f, 0.25f);
	//model = chairRotate * chairScale * translate * scale; // calc model matrix (world space)
	//MVP = projection * view * model; // compute MVP matrix
	//glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]); // send info to graphcis card
	//meshList[GEO_REDCUBE]->Render();

	//// Base
	//scale.SetToScale(0.5f, 0.2f, 0.5f);
	//translate.SetToTranslation(0.f, -1.f, 0.f);
	//model = chairRotate * chairScale * translate * scale; // calc model matrix (world space)
	//MVP = projection * view * model; // compute MVP matrix
	//glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]); // send info to graphcis card
	//meshList[GEO_CUBE]->Render();


	// Chair
	//Mtx44 chairRotate;
	//chairRotate.SetToIdentity();
	//chairRotate.SetToRotation(angleY, 0.f, 1.f, 0.f);

	modelStack.PushMatrix();
	{
		//// Translate the entire chair
		//
		//modelStack.Translate(planePos.x, planePos.y, planePos.z);
		//modelStack.Scale(scaling.x, scaling.y, scaling.z);
		//
		//
		//

		////SEAT
		//modelStack.PushMatrix(); {
		//	
		//modelStack.Translate(0.f, 0.f, 0.f);
		//
		//MVP = projectionStack.Top() * viewStack.Top() *modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,&MVP.a[0]);
		//meshList[GEO_SPHERE]->Render();
		//modelStack.PopMatrix();
		//}

		////BACK
		//modelStack.PushMatrix(); {
		//	modelStack.Translate(0.f, 0.55f, -0.45f);
		//	modelStack.Scale(1.f, 1.f, 0.1f);
		//	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
		//	meshList[GEO_CUBE]->Render();
		//	modelStack.PopMatrix();
		//}

		////Right
		//modelStack.PushMatrix(); {
		//	modelStack.Translate(-0.45f, 0.3f, -0.15f);
		//	modelStack.Scale(0.1f, 0.5f, 0.5f);
		//	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
		//	meshList[GEO_CUBE]->Render();
		//	modelStack.PopMatrix();
		//}

		////Left
		//modelStack.PushMatrix(); {
		//	modelStack.Translate(0.45f, 0.3f, -0.15f);
		//	modelStack.Scale(0.1f, 0.5f, 0.5f);
		//	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
		//	meshList[GEO_CUBE]->Render();
		//	modelStack.PopMatrix();
		//}

		////Stand
		//modelStack.PushMatrix(); {
		//	modelStack.Translate(0.f, -0.5f, 0.f);
		//	modelStack.Scale(0.2f, 0.7f, 0.2f);
	
		//	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
		//	meshList[GEO_CUBE]->Render();
		//	modelStack.PopMatrix();
		//}

		////Base
		//modelStack.PushMatrix(); {
		//	modelStack.Translate(0.f, -1.f, 0.f);
		//	modelStack.Scale(0.5f, 0.2f, 0.5f);
		//	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
		//	meshList[GEO_REDCUBE]->Render();
		//	modelStack.PopMatrix();
		//}
	}
	modelStack.PopMatrix();
}


void Scene1::Exit()
{
	// Cleanup VBO here
	for (int i = 0; i < NUM_GEOMETRY; ++i)
	{
		if (meshList[i])
		{
			delete meshList[i];
		}
	}
	glDeleteVertexArrays(1, &m_vertexArrayID);
	glDeleteProgram(m_programID);
}

void Scene1::HandleKeyPress()
{
	if (Application::IsKeyPressed(0x31))
	{
		// Key press to enable culling
		glEnable(GL_CULL_FACE);
	}
	if (Application::IsKeyPressed(0x32))
	{
		// Key press to disable culling
		glDisable(GL_CULL_FACE);
	}
	if (Application::IsKeyPressed(0x33))
	{
		// Key press to enable fill mode for the polygon
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); //default fill mode
	}
	if (Application::IsKeyPressed(0x34))
	{
		// Key press to enable wireframe mode for the polygon
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //wireframe mode
	}
}