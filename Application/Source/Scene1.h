#ifndef SCENE_1_H
#define SCENE_1_H

#include "Scene.h"
#include "Camera.h"
#include "Mesh.h"
#include "MatrixStack.h"

class Scene1 : public Scene
{
public:
	float fan_rotate;
	float wardrobe_move;

	float chair_move_x;
	float chair_move_z;

	float chair_rotate;

	float window_rotate;
	enum GEOMETRY_TYPE
	{
		GEO_AXES,
		GEO_PURPLE_QUAD, // Scene walls
		GEO_CYLINDER,
		GEO_LIGHTS,
		GEO_BED_LEGS,
		GEO_WARDROBE,
		GEO_WARDROBE_CASE,
		GEO_WARDROBE_CASE_SUPPORT,

		Windows_Frame,
		Glass_Panes,
		Window_Acc,
		Window_Handel,

		shevles_Case,
		Shevlevs_C,

		Table,
		Table_Legs,

	
		fan_body,
		fan_blades,

		PC_BOX,
		Monitor,

		Chair_body,
		Chair_mids,
		Chair_Back,
		Chair_wheels,
		Chair_support,
		

		GEO_BED_SUPPORT_CUBE,
		GEO_BED_BACK,
		GEO_QUAD,
		GEO_ORANGE_QUAD,
		GEO_RED_QUAD,
		GEO_CUBE,
		GEO_REDCUBE,
		GEO_SPHERE,
		NUM_GEOMETRY,
	};

	enum UNIFORM_TYPE
	{
		U_MVP = 0,
		U_TOTAL,
	};

	Scene1();
	~Scene1();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

private:
	void HandleKeyPress();

	MS modelStack, viewStack, projectionStack;

	unsigned m_vertexArrayID;
	Mesh* meshList[NUM_GEOMETRY];

	unsigned m_programID;
	unsigned m_parameters[U_TOTAL];

	Camera camera;
	Vector3 planePos; //stores position of plane/quade
	float angleY; // angle of plane about the y-axis
	Vector3 scaling;
	float sceneTime; // how long since the scene is started
};

#endif