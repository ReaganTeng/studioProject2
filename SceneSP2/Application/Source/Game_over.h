#ifndef SceneGameover_H
#define SceneGameover_H

#include "Scene.h"
#include "Camera3.h"
#include "Mesh.h"
#include "MatrixStack.h"
#include "Light.h"

#include "QnA.h"

class Game_over : public Scene
{
	MS modelStack, viewStack, projectionStack;

	enum GEOMETRY_TYPE
	{
		GEO_AXES,

		GEO_TEXT,

		GEO_TOP,
		GEO_BOTTOM,
		GEO_LEFT,
		GEO_RIGHT,
		GEO_FRONT,
		GEO_BACK,

		GEO_DESK,
		GEO_HOUSE,
		GEO_GROUND,

		GEO_LIGHTBALL,
		GEO_PLAYERSPHERE,

		// Police model
		GEO_POLICEHELMET,
		GEO_HEAD,
		GEO_LIMBS,
		GEO_FEET,
		GEO_PALM,
		GEO_EYES,
		GEO_MOUTH,

		GEO_MONITOR,
		GEO_COMPUTER_CASE,
		GEO_COP,
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
	Game_over();
	~Game_over();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();
private:
	unsigned m_vertexArrayID;
	Mesh* meshList[NUM_GEOMETRY];

	unsigned m_programID;
	unsigned m_parameters[U_TOTAL];

	void RenderSkybox();
	void RenderMesh(Mesh* mesh, bool enableLight);
	void RenderText(Mesh* mesh, std::string text, Color color);
	void RenderTextOnScreen(Mesh* mesh, std::string text, Color
		color, float size, float x, float y);
	void RenderMeshOnScreen(Mesh* mesh, int x, int y, float
		sizex, float sizey);
	void RenderPhysicalWorld();
	void RenderUI();
	void RenderPolice();

	float translate_x;
	float translate_z;

	float scale_translate_skybox;
	float scale_ground;

	int random_generator_1;
	int random_generator_2;
	int random_generator_3;

	
	

	Camera3 camera;
	Light light[1];
};

#endif