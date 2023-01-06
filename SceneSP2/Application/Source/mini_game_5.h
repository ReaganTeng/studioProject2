#ifndef mini_game_5_H
#define mini_game_5_H

#include "Scene.h"
#include "Camera_Game5.h"
#include "Mesh.h"
#include "MatrixStack.h"
#include "Light.h"
#include <vector>

class mini_game_5 : public Scene
{

	MS modelStack, viewStack, projectionStack;

	enum GEOMETRY_TYPE
	{
		GEO_TEXT,

		//skybox
		GEO_LEFT,
		GEO_RIGHT,
		GEO_TOP,
		GEO_BOTTOM,
		GEO_FRONT,
		GEO_BACK,

		//buildings
		GEO_BUILDING1,
		GEO_BUILDING2,
		GEO_BUILDING3,
		GEO_BUILDING4,

		//Groundmesh
		GEO_GROUND,

		//roads
		GEO_ROAD1,
		GEO_ROAD2,

		//phone
		GEO_PHONE,
		GEO_TABLE,

		//naruto
		GEO_POLICE,

		//tutorial
		GEO_BG,

		//progress bar
		GEO_BAROUTER,
		GEO_BARINNER,

		// Police model
		GEO_POLICEHELMET,
		GEO_HEAD,
		GEO_LIMBS,
		GEO_FEET,
		GEO_PALM,
		GEO_EYES,
		GEO_MOUTH,

		//progression bar

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

		U_LIGHTENABLED,
		U_NUMLIGHTS,

		U_LIGHT0_POSITION,
		U_LIGHT0_COLOR,
		U_LIGHT0_POWER,
		U_LIGHT0_KC,
		U_LIGHT0_KL,
		U_LIGHT0_KQ,
		U_LIGHT0_TYPE,
		U_LIGHT0_SPOTDIRECTION,
		U_LIGHT0_COSCUTOFF,
		U_LIGHT0_COSINNER,
		U_LIGHT0_EXPONENT,

		U_LIGHT1_POSITION,
		U_LIGHT1_COLOR,
		U_LIGHT1_POWER,
		U_LIGHT1_KC,
		U_LIGHT1_KL,
		U_LIGHT1_KQ,
		U_LIGHT1_TYPE,
		U_LIGHT1_SPOTDIRECTION,
		U_LIGHT1_COSCUTOFF,
		U_LIGHT1_COSINNER,
		U_LIGHT1_EXPONENT,

		U_LIGHT2_POSITION,
		U_LIGHT2_COLOR,
		U_LIGHT2_POWER,
		U_LIGHT2_KC,
		U_LIGHT2_KL,
		U_LIGHT2_KQ,
		U_LIGHT2_TYPE,
		U_LIGHT2_SPOTDIRECTION,
		U_LIGHT2_COSCUTOFF,
		U_LIGHT2_COSINNER,
		U_LIGHT2_EXPONENT,

		U_LIGHT3_POSITION,
		U_LIGHT3_COLOR,
		U_LIGHT3_POWER,
		U_LIGHT3_KC,
		U_LIGHT3_KL,
		U_LIGHT3_KQ,
		U_LIGHT3_TYPE,
		U_LIGHT3_SPOTDIRECTION,
		U_LIGHT3_COSCUTOFF,
		U_LIGHT3_COSINNER,
		U_LIGHT3_EXPONENT,

		U_COLOR_TEXTURE_ENABLED,
		U_COLOR_TEXTURE,

		U_TEXT_ENABLED,
		U_TEXT_COLOR,


		U_TOTAL
	};
public:
	mini_game_5();
	~mini_game_5();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();
	

private:
	unsigned m_vertexArrayID;
	Mesh* meshList[NUM_GEOMETRY];

	unsigned m_programID;
	unsigned m_parameters[U_TOTAL];
	//init objects
	void InitObjects();
	//update objects
	void IfNearPhone();
	void PoliceChase(double delta);
	void IfNearPolice();
	void UpdateLose();
	void UpdateWin();
	//render objects and features
	void RenderSkybox();
	void RenderPhones();
	void RenderTables();
	void RenderTutorial();
	void RenderProgress();
	void RenderTimer();
	void RenderLoseScreen();
	void RenderComplete();	
	void RenderWin();
	//render meshes
	void RenderMesh(Mesh* mesh, bool enableLight);
	void RenderText(Mesh* mesh, std::string text, Color color);
	void RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y);
	void RenderMeshOnScreen(Mesh* mesh, int x, int y, int sizex, int sizey);
	//interaction with phones
	bool atPhone1, atPhone2, atPhone3, atPhone4;

	//update progress
	bool complete1, complete2, complete3, complete4;
	float timePhone1, timePhone2, timePhone3, timePhone4;
	float barsize1, barsize2, barsize3, barsize4;
	float translatebar1, translatebar2, translatebar3, translatebar4;
	float completecalls;

	//police chasing
	float translatex, translatey, translatez;
	float rotatePolice;
	float policetimer1, policetimer2, policetimer3, policetimer4;
	bool timer1, timer2, timer3, timer4;
	bool escape1, escape2, escape3, escape4;
	bool gameEnd;

	//tutorial
	bool tutorialscreen;
	//rendering
	std::vector<float> phoneSize;
	std::vector<float> phonePos;
	std::vector<float> tableSize;
	std::vector<float> tablePos;
	std::vector<float> policePos;

	float fps;
	Vector3 cameraPos;
	Camera_Game5 camera;
	Light light[1];
};

#endif