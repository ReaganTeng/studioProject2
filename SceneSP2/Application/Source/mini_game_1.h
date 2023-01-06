#ifndef mini_game_1_H
#define mini_game_1_H

#include "Scene.h"
#include "Camera_Game1.h"
#include "Mesh.h"
#include "MatrixStack.h"
#include "Application.h"
#include "Light.h"
#include <vector>

class mini_game_1 : public Scene
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
		GEO_DESK,
		GEO_VICTIM,

		//For scam ads:
		GEO_JOBSCAM,
		GEO_PHONESCAM,
		GEO_GIRLFRIENDSCAM,
		GEO_LOTTERYSCAM,
		GEO_MONEYSCAM,

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
	mini_game_1();
	~mini_game_1();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

private:
	unsigned m_vertexArrayID;
	Mesh* meshList[NUM_GEOMETRY];

	unsigned m_programID;
	unsigned m_parameters[U_TOTAL];

	std::vector<float> tablePos;
	std::vector<float> tableSize;
	std::vector<float> NPCPos;
	std::vector<float> NPCSize;
	std::vector<float> scamPos;
	std::vector<float> scamSize;
	void innitPos();
	void innitSize();

	void RenderSkybox();
	void RenderMesh(Mesh* mesh, bool enableLight);
	void RenderText(Mesh* mesh, std::string text, Color color);
	void RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y);
	void RenderMeshOnScreen(Mesh* mesh, int x, int y, float sizex, float sizey, float rotate);
	void RenderRoom();
	void RenderDesk();
	void RenderVictim();
	void RenderScams();
	void RenderScamInHand();
	void restart();

	//For camera collision:
	Vector3 cameraPos;
	void checkCamCollision(Vector3 cameraPos);

	//For tutorial
	bool tutorialStart;
	bool tutorial2, tutorial3;
	//For interactions (In general)
	void checkforInteraction(); //Check for all interactions
	void RenderInteractionText();
	int successes;
	int failures;
	int attempts;
	void RenderScore();
	//For interactions (scam):
	bool enterInteraction;
	bool nearScam1, nearScam2, nearScam3, nearScam4, nearScam5;
	bool pickedScam1, pickedScam2, pickedScam3, pickedScam4, pickedScam5;
	bool holdingAScam;
	//For interactions (NPC);
	bool nearNPC1, nearNPC2, nearNPC3, nearNPC4, nearNPC5;
	bool spawn1, spawn2, spawn3, spawn4, spawn5;
	bool talking;
	bool talking1, talking2, talking3, talking4, talking5;
	bool canGiveScam;
	bool success;
	bool addToScore;
	int whatScam;
	int whatNPC;
	bool renderResult;
	float textTimer;
	void RenderNPCSpeech();
	void RenderScamResult(bool success);
	void CheckScamGiven();
	//For interactions (winning and losing):
	bool won, lost;
	bool lose();
	int win();
	void RenderEndGame();
	void UpdateEndgame();
	float endTimer;
	bool end;

	void RenderDay();

	float translate_x;
	float translate_z;

	float fps;

	Camera_Game1 camera;
	Light light[1];
};

#endif