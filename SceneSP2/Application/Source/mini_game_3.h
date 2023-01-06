#ifndef mini_game_3_H
#define mini_game_3_H

#include "Scene.h"
#include "Camera_Game3.h"
#include "Mesh.h"
#include "MatrixStack.h"
#include "Light.h"
#include <vector>

class mini_game_3 : public Scene
{

	MS modelStack, viewStack, projectionStack;

	enum GEOMETRY_TYPE
	{

		
		GEO_TEXT,

		//for skybox
		GEO_FRONT,
		GEO_BACK,
		GEO_TOP,
		GEO_BOTTOM,
		GEO_LEFT,
		GEO_RIGHT,

		GEO_FLOOR,
		GEO_ROOMWALL,
		GEO_LIGHTBALL,

		//Models:
		GEO_MAZEWALL,
		GEO_DIGIT0,
		GEO_DIGIT1,
		GEO_DIGIT2,
		GEO_DIGIT3,
		GEO_DIGIT4,
		GEO_DIGIT5,
		GEO_DIGIT6,
		GEO_DIGIT7,
		GEO_DIGIT8,
		GEO_DIGIT9,
		GEO_PORTAL,

		//For interactions and UI:
		GEO_UI,


		NUM_GEOMETRY,
	};
	enum UNIFORM_TYPE
	{
		U_MVP = 0,
		U_MODELVIEW,
		U_MODELVIEW_INVERSE_TRANSPOSE,
		U_MATERIAL_AMBIENT,
		U_MATERIAL_DIFFUSE,
		U_MATERIAL_SPECULAR,
		U_MATERIAL_SHININESS,

		U_LIGHT0_POSITION,
		U_LIGHT0_COLOR,
		U_LIGHT0_POWER,
		U_LIGHT0_KC,
		U_LIGHT0_KL,
		U_LIGHT0_KQ,
		U_LIGHTENABLED,
		U_LIGHT0_TYPE,
		U_LIGHT0_SPOTDIRECTION,
		U_LIGHT0_COSCUTOFF,
		U_LIGHT0_COSINNER,
		U_LIGHT0_EXPONENT,


		U_NUMLIGHTS,
		U_COLOR_TEXTURE_ENABLED,
		U_COLOR_TEXTURE,


		U_TEXT_ENABLED,
		U_TEXT_COLOR,


		U_TOTAL
	};
public:
	mini_game_3();
	~mini_game_3();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();


private:
	unsigned m_vertexArrayID;
	Mesh* meshList[NUM_GEOMETRY];

	unsigned m_programID;
	unsigned m_parameters[U_TOTAL];
	void restart();
	std::vector<float> mazeWallPos;
	std::vector<float> mazeWallRotate;
	std::vector<float> mazeWallSize;
	std::vector<float> digitPos;
	std::vector<float> digitSize;
	std::vector<float> portalPos;
	std::vector<float> portalSize;
	//For passwords, digit collection
	std::vector<float> digitsCollected;
	std::vector<float> password;
	void innitPos();
	void innitRotate();
	void innitSize();

	//Functions for rendering stuff
	void RenderMesh(Mesh* mesh, bool enableLight);
	void RenderText(Mesh* mesh, std::string text, Color color);
	void RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y);
	void RenderMeshOnScreen(Mesh* mesh, int x, int y, float sizex, float sizey, float rotate);
	void RenderSkybox();
	void RenderFloor();
	void RenderMaze();
	void RenderDigits();
	void RenderPassword();


	//For camera collision:
	Vector3 cameraPos;

	//For tutorial
	bool start;
	//For interactions (In general)
	bool tutorialStart, tutorialFinish;
	void RenderTutorialText();
	void updateTutorial();

	//For interactions (collecting digits)
	bool canRenderDigitInteractions; //Bool to check if can render digit interactions
	//Bools to see which digit player is near specifically
	bool near1, near2, near3, near4, near5, near6, near7, near8, near9, near0;
	void RenderDigitInteractions();
	bool collected, wrong;
	void updateIfNearDigit();
	bool picked1, picked2, picked3, picked4, picked5, picked6, picked7, picked8, picked9, picked0;
	void updateDigitCollection();

	//For warning system
	float warnings;
	float warningTimer;
	void RenderWarnings();

	//For win/lose
	bool win;
	bool lose;
	bool canSpawnResult;
	int test;
	void updateEndGame();
	void RenderGameResult();

	//For spawning portal
	bool canSpawnPortal;
	void RenderPortal();


	float translate_x;
	float translate_z;

	void RenderDay();

	float fps;

	//Timers for button pressing
	float passwordTimer;
	float tutorialTimer;
	float collectingTimer;

	Camera_Game3 camera;
	Light light[1];
};

#endif