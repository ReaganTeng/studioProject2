#ifndef Scene_mini_game2_H
#define Scene_mini_game2_H

#include "Scene.h"
#include "Camera3.h"
#include "Mesh.h"
#include "MatrixStack.h"
#include "Light.h"

#include "QnA.h"

class mini_game_2 : public Scene
{
	MS modelStack, viewStack, projectionStack;

	enum GEOMETRY_TYPE
	{
		
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
		GEO_COMPUTER,
		GEO_DOOR,
		GEO_QUAD,
		GEO_MONITOR,
		GEO_COMPUTER_CASE,

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
	mini_game_2();
	~mini_game_2();

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

	void RenderSkybox();
	void RenderMesh(Mesh* mesh, bool enableLight);
	void RenderText(Mesh* mesh, std::string text, Color color);
	void RenderTextOnScreen(Mesh* mesh, std::string text, Color
		color, float size, float x, float y);
	void RenderMeshOnScreen(Mesh* mesh, int x, int y, float
		sizex, float sizey);
	void RenderPhysicalWorld();
	void RenderUI();
	



	//VARIABLES TO ADD TO LIST
	QnA card1;
	QnA card2;
	QnA card3;
	QnA card4;
	QnA card5;
	QnA card6;
	QnA card7;
	QnA card8;
	QnA card9;
	QnA card10;
	QnA card11;
	QnA card12;
	QnA card13;

	QnA card_1;
	QnA card_2;
	QnA card_3;
	bool game_over;
	std::string possible_ans_qn1[3];
	std::string possible_ans_qn2[3];
	std::string possible_ans_qn3[3];

	bool tutorial_page_1;

	//CORRECT ANSWER FOR QUESTION 1
	std::string correct_ans_qn1;
	//CORRECT ANSWER FOR QUESTION 2
	std::string correct_ans_qn2;
	//CORRECT ANSWER FOR QUESTION 3
	std::string correct_ans_qn3;

	std::string question_section_1;
	std::string question_section_2;
	std::string question_section_3;
	std::string correct_ans;

	
	int warnings;
	float scale_translate_skybox;
	float scale_ground;
	bool password_game;

	int random_generator_1;
	int random_generator_2;
	int random_generator_3;

	//QnA random_cards[];
	//QnA random_cards[];

	int question_number;

	float text_x_coordinate;
	float word_size;

	bool win_mode;
	

	Camera3 camera;
	Light light[1];
};

#endif