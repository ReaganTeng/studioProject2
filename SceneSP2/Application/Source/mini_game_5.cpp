#include "mini_game_5.h"
#include "GL\glew.h"

#include "shader.hpp"
#include "Mtx44.h"
#include "Utility.h"
#include "Application.h"
#include "MeshBuilder.h"
#include "MatrixStack.h"
#include "LoadTGA.h"
#include "timer.h"

#include <sstream>
#include <cstdlib>
#include <ctime>

mini_game_5::mini_game_5()
{
}

mini_game_5::~mini_game_5()
{
}

void mini_game_5::InitObjects()
{
	//position of objects
	//phone1
	{
		phonePos.push_back(-580);
		phonePos.push_back(7);
		phonePos.push_back(-300);

		tablePos.push_back(-580);
		tablePos.push_back(0);
		tablePos.push_back(-300);
	}
	//phone2
	{
		phonePos.push_back(-500);
		phonePos.push_back(7);
		phonePos.push_back(620);

		tablePos.push_back(-500);
		tablePos.push_back(0);
		tablePos.push_back(620);
	}
	//phone3
	{
		phonePos.push_back(120);
		phonePos.push_back(7);
		phonePos.push_back(-300);

		tablePos.push_back(120);
		tablePos.push_back(0);
		tablePos.push_back(-300);
	}
	//phone4
	{
		phonePos.push_back(630);
		phonePos.push_back(7);
		phonePos.push_back(550);

		tablePos.push_back(630);
		tablePos.push_back(0);
		tablePos.push_back(550);
	}
	//sizes of objects
	//phones
	for (int i = 0;i < 4;i++)
	{
		tableSize.push_back(20);
		tableSize.push_back(20);
		tableSize.push_back(10);
	}
	for (int i = 0;i < 4;i++)
	{
		phoneSize.push_back(0.5);
		phoneSize.push_back(0.5);
		phoneSize.push_back(0.5);
	}

}

void mini_game_5::IfNearPhone()
{
	float halftable = 10;
	//check if at phone1
	if ((tablePos[0] + halftable > camera.position.x-11 && tablePos[0] - halftable < camera.position.x+11 &&
		tablePos[2] + halftable > camera.position.z -11&& tablePos[2] - halftable < camera.position.z+11) && complete1 == false)
	{
		atPhone1 = true;
	}
	else
	{
		atPhone1 = false;
	}
	//check if at phone 2
	if ((tablePos[3] + halftable > camera.position.x-11 && tablePos[3] - halftable < camera.position.x+11 &&
		tablePos[5] + halftable > camera.position.z-11 && tablePos[5] - halftable < camera.position.z+11) && complete2 == false)
	{
		atPhone2 = true;
	}
	else
	{
		atPhone2 = false;
	}
	//check if at phone 3
	if ((tablePos[6] + halftable > camera.position.x-11 && tablePos[6] - halftable < camera.position.x+11 &&
		tablePos[8] + halftable > camera.position.z-11 && tablePos[8] - halftable < camera.position.z+11) && complete3 == false)
	{
		atPhone3 = true;
	}
	else
	{
		atPhone3 = false;
	}
	//check if at phone 4
	if ((tablePos[9] + halftable > camera.position.x-11 && tablePos[9] - halftable < camera.position.x+11 &&
		tablePos[11] + halftable > camera.position.z-11 && tablePos[11] - halftable < camera.position.z+11) && complete4 == false)
	{
		atPhone4 = true;
	}
	else
	{
		atPhone4 = false;
	}
}

void mini_game_5::Init()
{
	//Initialise values
	//interaction with phones
	atPhone1 = atPhone2 = atPhone3 = atPhone4 = false;

	//update progress	
	complete1 = complete2 = complete3 = complete4 = false;//to update value of complete calls
	timePhone1 = timePhone2 = timePhone3 = timePhone4 = 0;//progression of calls
	//for progression bar
	barsize1 = barsize2 = barsize3 = barsize4 = 0.2;
	translatebar1 = translatebar2 = translatebar3 = translatebar4 = 27;
	completecalls = 0;//Number of complete calls

	//police obj
	translatex = 0;
	translatey = -200;
	translatez = 0;
	rotatePolice = 0;

	//to render tutorial screen
	tutorialscreen = true;

	//for police chase
	srand(time(0));
	policetimer1 = rand() % 3 + 3;
	policetimer2 = rand() % 3 + 3;
	policetimer3 = rand() % 3 + 3;
	policetimer4 = rand() % 3 + 3;
	escape1 = escape2 = escape3 = escape4 = true;
	timer1 = timer2 = timer3 = timer4 = false;
	gameEnd = false;

	InitObjects();
	//EVERYTHING NEEDED, DON'T TOUCH
	{
		// Init VBO here
		Mtx44 projection;
		projection.SetToPerspective(45.f, 4.f / 3.f, 0.1f, 3000.f);
		projectionStack.LoadMatrix(projection);

		//set background color
		glClearColor(0.0f, 0.0f, 1.0f, 0.0f);

		//Enable depth test
		glEnable(GL_DEPTH_TEST);

		//Enable blending
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);		

		//Enable depth buffer and depth testing
		glEnable(GL_CULL_FACE);

		camera.Init(Vector3(25, 20, 95), Vector3(0, 0, 0), Vector3(0, 1, 0));

		light[0].type = Light::LIGHT_DIRECTIONAL;
		light[0].position.Set(900, 20, 900);
		light[0].color.Set(1, 1, 1);
		light[0].power = 1;
		light[0].kC = 1.f;
		light[0].kL = 0.01f;
		light[0].kQ = 0.001f;
		light[0].cosCutoff = cos(Math::DegreeToRadian(45));
		light[0].cosInner = cos(Math::DegreeToRadian(30));
		light[0].exponent = 3.f;
		light[0].spotDirection.Set(0.f, 1.f, 0.f);

		

		//generate default VAO for now
		glGenVertexArrays(1, &m_vertexArrayID);
		glBindVertexArray(m_vertexArrayID);

		//load vertex and fragment shaders
		m_programID = LoadShaders("Shader//Texture.vertexshader", "Shader//Text.fragmentshader");

		m_parameters[U_NUMLIGHTS] = glGetUniformLocation(m_programID, "numLights");
		m_parameters[U_MVP] = glGetUniformLocation(m_programID, "MVP");
		m_parameters[U_MODELVIEW] = glGetUniformLocation(m_programID, "MV");
		m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE] = glGetUniformLocation(m_programID, "MV_inverse_transpose");
		m_parameters[U_MATERIAL_AMBIENT] = glGetUniformLocation(m_programID, "material.kAmbient");
		m_parameters[U_MATERIAL_DIFFUSE] = glGetUniformLocation(m_programID, "material.kDiffuse");
		m_parameters[U_MATERIAL_SPECULAR] = glGetUniformLocation(m_programID, "material.kSpecular");
		m_parameters[U_MATERIAL_SHININESS] = glGetUniformLocation(m_programID, "material.kShininess");
		m_parameters[U_LIGHTENABLED] = glGetUniformLocation(m_programID, "lightEnabled");
		// Get a handle for our "colorTexture" uniform
		m_parameters[U_COLOR_TEXTURE_ENABLED] = glGetUniformLocation(m_programID, "colorTextureEnabled");
		m_parameters[U_COLOR_TEXTURE] = glGetUniformLocation(m_programID, "colorTexture");
		m_parameters[U_TEXT_ENABLED] = glGetUniformLocation(m_programID, "textEnabled");
		m_parameters[U_TEXT_COLOR] = glGetUniformLocation(m_programID, "textColor");

		//light[0]
		m_parameters[U_LIGHT0_TYPE] = glGetUniformLocation(m_programID, "lights[0].type");
		m_parameters[U_LIGHT0_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[0].spotDirection");
		m_parameters[U_LIGHT0_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[0].cosCutoff");
		m_parameters[U_LIGHT0_COSINNER] = glGetUniformLocation(m_programID, "lights[0].cosInner");
		m_parameters[U_LIGHT0_EXPONENT] = glGetUniformLocation(m_programID, "lights[0].exponent");
		m_parameters[U_LIGHT0_POSITION] = glGetUniformLocation(m_programID, "lights[0].position_cameraspace");
		m_parameters[U_LIGHT0_COLOR] = glGetUniformLocation(m_programID, "lights[0].color");
		m_parameters[U_LIGHT0_POWER] = glGetUniformLocation(m_programID, "lights[0].power");
		m_parameters[U_LIGHT0_KC] = glGetUniformLocation(m_programID, "lights[0].kC");
		m_parameters[U_LIGHT0_KL] = glGetUniformLocation(m_programID, "lights[0].kL");
		m_parameters[U_LIGHT0_KQ] = glGetUniformLocation(m_programID, "lights[0].kQ");

	

		//use our shader
		glUseProgram(m_programID);

		// Make sure you pass uniform parameters after glUseProgram()
		glUniform1i(m_parameters[U_NUMLIGHTS], 1);

		//light[0].type = Light::LIGHT_DIRECTIONAL;
		glUniform1i(m_parameters[U_LIGHT0_TYPE], light[0].type);
		glUniform3fv(m_parameters[U_LIGHT0_COLOR], 1, &light[0].color.r);
		glUniform1f(m_parameters[U_LIGHT0_POWER], light[0].power);
		//light[0].type = Light::LIGHT_POINT;
		glUniform1f(m_parameters[U_LIGHT0_KC], light[0].kC);
		glUniform1f(m_parameters[U_LIGHT0_KL], light[0].kL);
		glUniform1f(m_parameters[U_LIGHT0_KQ], light[0].kQ);
		//light[0].type = Light::LIGHT_SPOT;
		glUniform1f(m_parameters[U_LIGHT0_COSCUTOFF], light[0].cosCutoff);
		glUniform1f(m_parameters[U_LIGHT0_COSINNER], light[0].cosInner);
		glUniform1f(m_parameters[U_LIGHT0_EXPONENT], light[0].exponent);

		//light[1]type = Light::LIGHT_DIRECTIONAL;
		glUniform1i(m_parameters[U_LIGHT1_TYPE], light[1].type);
		glUniform3fv(m_parameters[U_LIGHT1_COLOR], 1, &light[1].color.r);
		glUniform1f(m_parameters[U_LIGHT1_POWER], light[1].power);
		//light[1].type = Light::LIGHT_POINT;
		glUniform1f(m_parameters[U_LIGHT1_KC], light[1].kC);
		glUniform1f(m_parameters[U_LIGHT1_KL], light[1].kL);
		glUniform1f(m_parameters[U_LIGHT1_KQ], light[1].kQ);
		//light[1].type = Light::LIGHT_SPOT;
		glUniform1f(m_parameters[U_LIGHT1_COSCUTOFF], light[1].cosCutoff);
		glUniform1f(m_parameters[U_LIGHT1_COSINNER], light[1].cosInner);
		glUniform1f(m_parameters[U_LIGHT1_EXPONENT], light[1].exponent);
									   
		//Get a handle for our "MVP" uniform
		m_parameters[U_MVP] = glGetUniformLocation(m_programID, "MVP");
	}

//LOAD THE MODELS
	for (int i = 0; i < NUM_GEOMETRY; ++i)
	{
		meshList[i] = nullptr;
	}
	//Skybox
	{
		meshList[GEO_FRONT] = MeshBuilder::GenerateQuad("front", Color(1, 1, 1), 1.f);
		meshList[GEO_FRONT]->textureID = LoadTGA("Images//exosystem_ft.tga");
		meshList[GEO_BACK] = MeshBuilder::GenerateQuad("back", Color(1, 1, 1), 1.f);
		meshList[GEO_BACK]->textureID = LoadTGA("Images//exosystem_bk.tga");
		meshList[GEO_TOP] = MeshBuilder::GenerateQuad("top", Color(1, 1, 1), 1.f);
		meshList[GEO_TOP]->textureID = LoadTGA("Images//exosystem_up.tga");
		meshList[GEO_BOTTOM] = MeshBuilder::GenerateQuad("bottom", Color(1, 1, 1), 1.f);
		meshList[GEO_BOTTOM]->textureID = LoadTGA("Images//exosystem_dn.tga");
		meshList[GEO_LEFT] = MeshBuilder::GenerateQuad("left", Color(1, 1, 1), 1.f);
		meshList[GEO_LEFT]->textureID = LoadTGA("Images//exosystem_rt.tga");
		meshList[GEO_RIGHT] = MeshBuilder::GenerateQuad("right", Color(1, 1, 1), 1.f);
		meshList[GEO_RIGHT]->textureID = LoadTGA("Images//exosystem_lf.tga");
	}
	//GroundMesh
	{
		meshList[GEO_GROUND] = MeshBuilder::GenerateQuad("groundmesh", Color(1, 1, 1), 1.f);
		meshList[GEO_GROUND]->textureID = LoadTGA("Images//game1_floor.tga");
	}//Buildings
	{
		meshList[GEO_BUILDING1] = MeshBuilder::GenerateOBJMTL("building", "OBJ//skyscraperB.obj", "OBJ//skyscraperB.mtl");
		meshList[GEO_BUILDING2] = MeshBuilder::GenerateOBJMTL("building", "OBJ//skyscraperC.obj", "OBJ//skyscraperC.mtl");
		meshList[GEO_BUILDING3] = MeshBuilder::GenerateOBJMTL("building", "OBJ//skyscraperD.obj", "OBJ//skyscraperD.mtl");
		meshList[GEO_BUILDING4] = MeshBuilder::GenerateOBJMTL("building", "OBJ//skyscraperA.obj", "OBJ//skyscraperA.mtl");
	}
	//Roads
	{
		meshList[GEO_ROAD1] = MeshBuilder::GenerateOBJMTL("intersection", "OBJ//road_intersectionPath.obj", "OBJ//road_intersectionPath.mtl");
		meshList[GEO_ROAD2] = MeshBuilder::GenerateOBJMTL("road", "OBJ//road_straight.obj", "OBJ//road_straight.mtl");
	}
	//Models
	{
		meshList[GEO_PHONE] = MeshBuilder::GenerateOBJMTL("Phone", "OBJ//phone.obj", "OBJ//phone.mtl");
		meshList[GEO_TABLE] = MeshBuilder::GenerateOBJMTL("table", "OBJ//stall.obj", "OBJ//stall.mtl");
		meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
		meshList[GEO_TEXT]->textureID = LoadTGA("Image//MS_gothic.tga");
		meshList[GEO_POLICE] = MeshBuilder::GenerateOBJMTL("police", "OBJ//naruto.obj", "OBJ//naruto.mtl");
		meshList[GEO_BG] = MeshBuilder::GenerateQuad("tutorial", Color(1, 1, 1), 1.f);
		meshList[GEO_BG]->textureID = LoadTGA("Images//game1_UI.tga");
	}
	//Progress Bar
	{
		meshList[GEO_BARINNER] = MeshBuilder::GenerateQuad("bar progression", Color(1, 1, 1), 1.f);
		meshList[GEO_BAROUTER] = MeshBuilder::GenerateQuad("bar outer", Color(0, 0, 0), 1.f);
		meshList[GEO_BAROUTER]->textureID = LoadTGA("Images//progress_bar.tga");
	}


	Mesh::SetMaterialLoc(m_parameters[U_MATERIAL_AMBIENT],
		m_parameters[U_MATERIAL_DIFFUSE],
		m_parameters[U_MATERIAL_SPECULAR],
		m_parameters[U_MATERIAL_SHININESS]);
}

void mini_game_5::PoliceChase(double delta)
{
	if (timer1 == true)
	{
		policetimer1 -= (float)(1 * delta);
		if (policetimer1 < 0)
		{
			policetimer1 = 0;
		}
	}
	if (timer2 == true)
	{
		policetimer2 -= (float)(1 * delta);
		if (policetimer2 < 0)
		{
			policetimer2 = 0;
		}
	}
	if (timer3 == true)
	{
		policetimer3 -= (float)(1 * delta);
		if (policetimer3 < 0)
		{
			policetimer3 = 0;
		}
	}
	if (timer4 == true)
	{
		policetimer4 -= (float)(1 * delta);
		if (policetimer4 < 0)
		{
			policetimer4 = 0;
		}
	}
}

void mini_game_5::IfNearPolice()
{
	float runDistance = 200;
	if (policetimer1 == 0)
	{
		if ((tablePos[0] + runDistance > camera.position.x && tablePos[0] - runDistance < camera.position.x &&
			tablePos[2] + runDistance > camera.position.z && tablePos[2] - runDistance < camera.position.z))
		{
			escape1 = false;
			gameEnd = true;
			timer1 = false;
		}
		else
		{
			escape1 = true;
			policetimer1 = rand() % 2 + 3;
			timer1 = false;
		}
	}
	if (policetimer2 == 0)
	{
		if ((tablePos[3] + runDistance > camera.position.x && tablePos[3] - runDistance < camera.position.x &&
			tablePos[5] + runDistance > camera.position.z && tablePos[5] - runDistance < camera.position.z))
		{
			escape2 = false;
			gameEnd = true;
			timer2 = false;
		}
		else
		{
			escape2 = true;
			policetimer2 = rand() % 2 + 3;
			timer2 = false;
		}

	}
	if (policetimer3 == 0)
	{
		if ((tablePos[6] + runDistance > camera.position.x && tablePos[6] - runDistance < camera.position.x &&
			tablePos[8] + runDistance > camera.position.z && tablePos[8] - runDistance < camera.position.z))
		{
			escape3 = false;
			gameEnd = true;
			timer3 = false;
		}
		else
		{
			escape3 = true;
			policetimer3 = rand() % 2 + 3;
			timer3 = false;
		}
	}
	if (policetimer4 == 0)
	{
		if ((tablePos[9] + runDistance > camera.position.x && tablePos[9] - runDistance < camera.position.x &&
			tablePos[11] + runDistance > camera.position.z && tablePos[11] - runDistance < camera.position.z))
		{
			escape4 = false;
			gameEnd = true;
			timer4 = false;
		}
		else
		{
			escape4 = true;
			policetimer4 = rand() % 2 + 3;
			timer4 = false;
		}
	}
}

void mini_game_5::UpdateLose()
{
	if ((escape1 == false || escape2 == false || escape3 == false || escape4 == false) && Application::IsKeyPressed('R'))
	{
		//interaction with phones
		atPhone1 = atPhone2 = atPhone3 = atPhone4 = false;

		//update progress	
		complete1 = complete2 = complete3 = complete4 = false;//to update value of complete calls
		timePhone1 = timePhone2 = timePhone3 = timePhone4 = 0;//progression of calls
		//for progression bar
		barsize1 = barsize2 = barsize3 = barsize4 = 0.2;
		translatebar1 = translatebar2 = translatebar3 = translatebar4 = 27;
		completecalls = 0;//Number of complete calls

		//police obj
		translatex = 0;
		translatey = -200;
		translatez = 0;
		rotatePolice = 0;

		//to render tutorial screen
		tutorialscreen = true;

		//for police chase
		srand(time(NULL));
		policetimer1 = rand() % 3 + 3;
		policetimer2 = rand() % 3 + 3;
		policetimer3 = rand() % 3 + 3;
		policetimer4 = rand() % 3 + 3;
		escape1 = escape2 = escape3 = escape4 = true;
		timer1 = timer2 = timer3 = timer4 = false;
		gameEnd = false;
		Application::setScene(0);
	}

}

void mini_game_5::UpdateWin()
{
	if (complete1 == true && complete2 == true && complete3 == true && complete4 == true)
	{
		gameEnd = true;
	}
}

void  mini_game_5::Update(double dt)
{
	fps = 1.f / dt;
	IfNearPhone();
	{

		if (Application::IsKeyPressed('1')) //enable back face culling
			glEnable(GL_CULL_FACE);
		if (Application::IsKeyPressed('2')) //disable back face culling
			glDisable(GL_CULL_FACE);
		if (Application::IsKeyPressed('3'))
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); //default fill mode
		if (Application::IsKeyPressed('4'))
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //wireframe mode

		float LSPEED = 30.0f;
		//MOVE LIGHT TO NEGATIVE SIDE OF Z AXIS
		if (Application::IsKeyPressed('I'))
			light[0].position.z -= (float)(LSPEED * dt);
		//MOVE LIGHT TO POSITIVE SIDE OF Z AXIS
		if (Application::IsKeyPressed('K'))
			light[0].position.z += (float)(LSPEED * dt);
		//MOVE LIGHT TO NEGATIVE SIDE OF x AXIS
		if (Application::IsKeyPressed('J'))
			light[0].position.x -= (float)(LSPEED * dt);
		//MOVE LIGHT TO POSITIVE SIDE OF X AXIS
		if (Application::IsKeyPressed('L'))
			light[0].position.x += (float)(LSPEED * dt);
		//GO DOWN
		if (Application::IsKeyPressed('O'))
			light[0].position.y -= (float)(LSPEED * dt);
		//GO UP
		if (Application::IsKeyPressed('P'))
			light[0].position.y += (float)(LSPEED * dt);


		if (Application::IsKeyPressed('5'))
		{
			//to do: switch light type to POINT and pass the information to shader
			light[0].type = Light::LIGHT_POINT;
		}
		else if (Application::IsKeyPressed('6'))
		{
			//to do: switch light type to DIRECTIONAL and pass the information to shader
			light[0].type = Light::LIGHT_DIRECTIONAL;
		}
		else if (Application::IsKeyPressed('7'))
		{
			//to do: switch light type to SPOT and pass the information to shader
			light[0].type = Light::LIGHT_SPOT;
		}


		//MOVE LIGHT TO NEGATIVE SIDE OF Z AXIS
		if (Application::IsKeyPressed('I'))
			light[0].position.z -= (float)(LSPEED * dt);
		//MOVE LIGHT TO POSITIVE SIDE OF Z AXIS
		if (Application::IsKeyPressed('K'))
			light[0].position.z += (float)(LSPEED * dt);
		//MOVE LIGHT TO NEGATIVE SIDE OF x AXIS
		if (Application::IsKeyPressed('J'))
			light[0].position.x -= (float)(LSPEED * dt);
		//MOVE LIGHT TO POSITIVE SIDE OF X AXIS
		if (Application::IsKeyPressed('L'))
			light[0].position.x += (float)(LSPEED * dt);
		//GO DOWN
		if (Application::IsKeyPressed('O'))
			light[0].position.y -= (float)(LSPEED * dt);
		//GO UP
		if (Application::IsKeyPressed('P'))
			light[0].position.y += (float)(LSPEED * dt);

	}
	//phone progression
	if (atPhone1 == true && Application::IsKeyPressed('C'))
	{
		timePhone1 += (float)(1 * dt);
		translatebar1 += (float)(12.1 / 13 * dt);
		barsize1 += (float)(22.5/ 12 * dt);
		if (timePhone1 > 2 && timePhone1 < 2.1)
		{	
			timer1 = true;
		}
		else if (timePhone1 > 9 && timePhone1 < 10)
		{
			timer1 = true;
		}
		else if (timePhone1 >= 15)
		{
			complete1 = true;
			completecalls += 1;
		}
	}
	if (atPhone2 == true && Application::IsKeyPressed('C'))
	{
		timePhone2 += (float)(1 * dt);
		translatebar2 += (float)(12.1 / 13 * dt);
		barsize2 += (float)(22.5 / 12 * dt);
		if (timePhone2 > 2 && timePhone2 < 3)
		{
			timer2 = true;
		}
		else if (timePhone2 > 9 && timePhone2 < 10)
		{
			timer2 = true;
		}
		else if (timePhone2 >= 15)
		{
			complete2 = true;
			completecalls += 1;
		}
	}
	if (atPhone3 == true && Application::IsKeyPressed('C'))
	{
		timePhone3 += (float)(1 * dt);
		translatebar3 += (float)(12.1 / 13 * dt);
		barsize3 += (float)(22.5 / 12 * dt);
		if (timePhone3 > 2 && timePhone3 < 2.1)
		{
			timer3 = true;
		}
		else if (timePhone3 > 9 && timePhone3 < 10)
		{
			timer3 = true;
		}
		else if (timePhone3 >= 15)
		{
			complete3 = true;
			completecalls += 1;
		}
	}
	if (atPhone4 == true && Application::IsKeyPressed('C'))
	{
		timePhone4 += (float)(1 * dt);
		translatebar4 += (float)(12.1 / 13 * dt);
		barsize4 += (float)(22.5 / 12 * dt);
		if (timePhone4 > 2 && timePhone4 < 2.1)
		{
			timer4 = true;
		}
		else if (timePhone4 > 9 && timePhone4 < 10)
		{
			timer4 = true;
		}
		else if (timePhone4 >= 15)
		{
			complete4 = true;
			completecalls += 1;
		}
	}

	PoliceChase(dt);
	IfNearPolice();
	UpdateLose();
	UpdateWin();

	if (gameEnd == false)
	{
		camera.Update(tablePos, dt);
	}
}


void  mini_game_5::RenderMesh(Mesh* mesh, bool enableLight)
{
	Mtx44 MVP, modelView, modelView_inverse_transpose;

	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	modelView = viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MODELVIEW], 1, GL_FALSE, &modelView.a[0]);
	if (enableLight)
	{
		glUniform1i(m_parameters[U_LIGHTENABLED], 1);
		modelView_inverse_transpose = modelView.GetInverse().GetTranspose();
		glUniformMatrix4fv(m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE], 1, GL_FALSE, &modelView_inverse_transpose.a[0]);

		//load material
		glUniform3fv(m_parameters[U_MATERIAL_AMBIENT], 1, &mesh->material.kAmbient.r);
		glUniform3fv(m_parameters[U_MATERIAL_DIFFUSE], 1, &mesh->material.kDiffuse.r);
		glUniform3fv(m_parameters[U_MATERIAL_SPECULAR], 1, &mesh->material.kSpecular.r);
		glUniform1f(m_parameters[U_MATERIAL_SHININESS], mesh->material.kShininess);
	}
	else
	{
		glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	}

	if (mesh->textureID > 0)
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mesh->textureID);
		glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	}
	else
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 0);
	}


	mesh->Render(); //this line should only be called once

	if (mesh->textureID > 0)
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

void mini_game_5::RenderSkybox()
{
	const float OFFSET = 998;

	modelStack.PushMatrix();
	modelStack.Translate(0, 0, -OFFSET);
	modelStack.Scale(2000, 2000, 2000);
	RenderMesh(meshList[GEO_FRONT], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0, 0, OFFSET);
	modelStack.Rotate(180, 0, 1, 0);
	modelStack.Scale(2000, 2000, 2000);
	RenderMesh(meshList[GEO_BACK], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0, OFFSET, 0);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Rotate(90, 1, 0, 0);
	modelStack.Scale(2000, 2000, 2000);
	RenderMesh(meshList[GEO_TOP], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0, -OFFSET, 0);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Rotate(-90, 1, 0, 0);
	modelStack.Scale(2000, 2000, 2000);
	RenderMesh(meshList[GEO_BOTTOM], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-OFFSET, 0, 0);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Scale(2000, 2000, 2000);
	RenderMesh(meshList[GEO_LEFT], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(OFFSET, 0, 0);
	modelStack.Rotate(-90, 0, 1, 0);
	modelStack.Scale(2000, 2000, 2000);
	RenderMesh(meshList[GEO_RIGHT], false);
	modelStack.PopMatrix();
}

void mini_game_5::RenderPhones()
{
	for (int i = 0;i < 12;i += 3)
	{
		modelStack.PushMatrix();
		{
			modelStack.Translate(phonePos[i], phonePos[i + 1], phonePos[i + 2]);
			modelStack.Rotate(90, -1, 0, 0);
			modelStack.Scale(phoneSize[i], phoneSize[i + 1], phoneSize[i + 2]);
			RenderMesh(meshList[GEO_PHONE], true);
		}
		modelStack.PopMatrix();
	}
}

void mini_game_5::RenderTables()
{
	for (int i = 0;i < 12;i += 3)
	{
		modelStack.PushMatrix();
		{
			modelStack.Translate(tablePos[i], tablePos[i + 1], tablePos[i + 2]);
			modelStack.Rotate(0, 0, 0, 1);
			modelStack.Scale(tableSize[i], tableSize[i + 1], tableSize[i + 2]);
			RenderMesh(meshList[GEO_TABLE], true);
		}
		modelStack.PopMatrix();
	}
}

void mini_game_5::RenderTutorial()
{
	if (tutorialscreen == true)
	{
		RenderMeshOnScreen(meshList[GEO_BG], 40, 30, 70, 50);
		RenderTextOnScreen(meshList[GEO_TEXT], "Press C to make a call.", Color(0, 1, 0), 2, 16, 40);
		RenderTextOnScreen(meshList[GEO_TEXT], "Run away when a timer pops up.", Color(0, 1, 0), 2, 16, 30);
		RenderTextOnScreen(meshList[GEO_TEXT], "Press [F] to continue.", Color(0, 1, 0), 2, 16, 20);
	}
	if (Application::IsKeyPressed('F'))
	{
		tutorialscreen = false;
	}
}

void mini_game_5::RenderProgress()
{
	if (atPhone1 == true)
	{	
		RenderMeshOnScreen(meshList[GEO_BARINNER], translatebar1, 15, barsize1, 1);
		RenderMeshOnScreen(meshList[GEO_BAROUTER], 40, 15, 33, 6);
		RenderTextOnScreen(meshList[GEO_TEXT], "Progress", Color(0, 0, 0), 1.5, 26, 18);
		RenderTextOnScreen(meshList[GEO_TEXT],"Hold [C] to call", Color(0,0,0), 1.2, 40, 11.5);
	}
	if (atPhone2 == true)
	{	
		RenderMeshOnScreen(meshList[GEO_BARINNER], translatebar2, 15, barsize2, 1);
		RenderMeshOnScreen(meshList[GEO_BAROUTER], 40, 15, 33, 6);
		RenderTextOnScreen(meshList[GEO_TEXT], "Progress", Color(0, 0, 0), 1.5, 26, 18);
		RenderTextOnScreen(meshList[GEO_TEXT], "Hold [C] to call", Color(0, 0, 0), 1.2, 40, 11.5);
	}
	if (atPhone3 == true)
	{	
		RenderMeshOnScreen(meshList[GEO_BARINNER], translatebar3, 15, barsize3, 1);
		RenderMeshOnScreen(meshList[GEO_BAROUTER], 40, 15, 33, 6);
		RenderTextOnScreen(meshList[GEO_TEXT], "Progress", Color(0, 0, 0), 1.5, 26, 18);
		RenderTextOnScreen(meshList[GEO_TEXT], "Hold [C] to call", Color(0, 0, 0), 1.2, 40, 11.5);
	}
	if (atPhone4 == true)
	{	
		RenderMeshOnScreen(meshList[GEO_BARINNER], translatebar4, 15, barsize4, 1);
		RenderMeshOnScreen(meshList[GEO_BAROUTER], 40, 15, 33, 6);
		RenderTextOnScreen(meshList[GEO_TEXT], "Progress", Color(0, 0, 0), 1.5, 26, 18);
		RenderTextOnScreen(meshList[GEO_TEXT], "Hold [C] to call", Color(0, 0, 0), 1.2, 40, 11.5);
	}
}

void mini_game_5::RenderTimer()
{
	if (timer1 == true)
	{
		std::ostringstream time;
		time << round(policetimer1);
		time.str();
		RenderMeshOnScreen(meshList[GEO_BG], 40, 22, 50, 10);
		RenderTextOnScreen(meshList[GEO_TEXT], "Police coming in:", Color(0, 1, 0), 2, 18, 22);
		RenderTextOnScreen(meshList[GEO_TEXT], time.str(), Color(0, 1, 0), 2, 53, 22);
	}
	if (timer2 == true)
	{
		std::ostringstream time;
		time << round(policetimer2);
		time.str();
		RenderMeshOnScreen(meshList[GEO_BG], 40, 22, 50, 10);
		RenderTextOnScreen(meshList[GEO_TEXT], "Police coming in:", Color(0, 1, 0), 2, 18, 22);
		RenderTextOnScreen(meshList[GEO_TEXT], time.str(), Color(0, 1, 0), 2, 53, 22);
	}
	if (timer3 == true)
	{
		std::ostringstream time;
		time << round(policetimer3);
		time.str();
		RenderMeshOnScreen(meshList[GEO_BG], 40, 22, 50, 10);
		RenderTextOnScreen(meshList[GEO_TEXT], "Police coming in:", Color(0, 1, 0), 2, 18, 22);
		RenderTextOnScreen(meshList[GEO_TEXT], time.str(), Color(0, 1, 0), 2, 53, 22);
	}
	if (timer4 == true)
	{
		std::ostringstream time;
		time << round(policetimer4);
		time.str();
		RenderMeshOnScreen(meshList[GEO_BG], 40, 22, 50, 10);
		RenderTextOnScreen(meshList[GEO_TEXT], "Police coming in:", Color(0, 1, 0), 2, 18, 22);
		RenderTextOnScreen(meshList[GEO_TEXT], time.str(), Color(0, 1, 0), 2, 53, 22);
	}
}

void mini_game_5::RenderLoseScreen()
{
	if (escape1 == false || escape2 == false || escape3 == false || escape4 == false)
	{
		RenderMeshOnScreen(meshList[GEO_BG], 40, 30, 70, 50);
		RenderTextOnScreen(meshList[GEO_TEXT], "Congratualions!", Color(0, 1, 0), 2, 18, 30);
		RenderTextOnScreen(meshList[GEO_TEXT], "You have been caught!", Color(0, 1, 0), 2, 18, 25);
		RenderTextOnScreen(meshList[GEO_TEXT], "[R] to restart.", Color(0, 1, 0), 2, 18, 20);
	}
}

void mini_game_5::RenderComplete()
{
	std::ostringstream ss;
	ss << completecalls;
	ss.str();
	RenderTextOnScreen(meshList[GEO_TEXT], "Calls completed:", Color(1, 0, 0), 2, 40, 55);
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 0, 0), 2, 73, 55);
	RenderTextOnScreen(meshList[GEO_TEXT], "/4", Color(1, 0, 0), 2, 75, 55);
}

void mini_game_5::RenderWin()
{	
	if (complete1 == true && complete2 == true && complete3 == true && complete4 == true)
	{
		RenderMeshOnScreen(meshList[GEO_BG], 40, 30, 60, 50);
		RenderTextOnScreen(meshList[GEO_TEXT], "Congratualions!", Color(0, 1, 0), 2, 18, 30);
		RenderTextOnScreen(meshList[GEO_TEXT], "You have succesfully", Color(0, 1, 0), 2, 18, 25);
		RenderTextOnScreen(meshList[GEO_TEXT], "scammed 4 people!", Color(0, 1, 0), 2, 18, 20);
		RenderTextOnScreen(meshList[GEO_TEXT], "[Enter] to continue.", Color(0, 1, 0), 2, 18, 15);
		if (gameEnd == true && Application::IsKeyPressed(VK_RETURN))
		{
			Application::setScene(-1);
		}
	}
}

void mini_game_5::RenderText(Mesh* mesh, std::string text, Color color)
{
	if (!mesh || mesh->textureID <= 0) //Proper error check
		return;
	glDisable(GL_DEPTH_TEST); //uncomment for RenderTextOnScreen
	glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
	glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID);
	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	for (unsigned i = 0; i < text.length(); ++i)
	{
		Mtx44 characterSpacing;
		characterSpacing.SetToTranslation(i * 1.0f, 0, 0); //1.0f
		//is the spacing of each character, you may change this value
			Mtx44 MVP = projectionStack.Top() * viewStack.Top() *
			modelStack.Top() * characterSpacing;
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
			&MVP.a[0]);
		mesh->Render((unsigned)text[i] * 6, 6);
	}

	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
	glEnable(GL_DEPTH_TEST);
}

void mini_game_5::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y)
{
	if (!mesh || mesh->textureID <= 0) //Proper error check
		return;
	//glDisable(GL_DEPTH_TEST); //uncomment for RenderTextOnScreen


	Mtx44 ortho;
	ortho.SetToOrtho(0, 80, 0, 60, -10, 10); //size of screen UI
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity(); //No need camera for ortho mode
	modelStack.PushMatrix();
	modelStack.LoadIdentity(); //Reset modelStack
	modelStack.Translate(x, y, 0);
	modelStack.Scale(size, size, size);


	glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
	glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID);
	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	for (unsigned i = 0; i < text.length(); ++i)
	{
		Mtx44 characterSpacing;
		characterSpacing.SetToTranslation(0.5f + i * 1.0f, 0.5f, 0);		//is the spacing of each character, you may change this value
		Mtx44 MVP = projectionStack.Top() * viewStack.Top() *
			modelStack.Top() * characterSpacing;
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
			&MVP.a[0]);
		mesh->Render((unsigned)text[i] * 6, 6);
	}

	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
	//glEnable(GL_DEPTH_TEST);

	projectionStack.PopMatrix();
	viewStack.PopMatrix();
	modelStack.PopMatrix();
}

void mini_game_5::RenderMeshOnScreen(Mesh* mesh, int x, int y, int sizex, int sizey)
{
	glDisable(GL_DEPTH_TEST);
	Mtx44 ortho;
	ortho.SetToOrtho(0, 80, 0, 60, -10, 10); //size of screen UI
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity(); //No need camera for ortho mode
	modelStack.PushMatrix();
	modelStack.LoadIdentity();
	//to do: scale and translate accordingly
	modelStack.Translate(x, y, 0);
	modelStack.Scale(sizex, sizey, 0);

	RenderMesh(mesh, false); //UI should not have light
	projectionStack.PopMatrix();
	viewStack.PopMatrix();
	modelStack.PopMatrix();
	glEnable(GL_DEPTH_TEST);
}

void mini_game_5 ::Render()
{
	// Render VBO here
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	//Set view matrix using camera settings
	viewStack.LoadIdentity();
	viewStack.LookAt(
		camera.position.x, camera.position.y, camera.position.z,
		camera.target.x, camera.target.y, camera.target.z,
		camera.up.x, camera.up.y, camera.up.z
	);
	modelStack.LoadIdentity();
	//light
	{
		if (light[0].type == Light::LIGHT_DIRECTIONAL)
		{
			Vector3 lightDir(light[0].position.x,
				light[0].position.y, light[0].position.z);
			Vector3 lightDirection_cameraspace = viewStack.Top() *
				lightDir;
			glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1,
				&lightDirection_cameraspace.x);
		}
		else if (light[0].type == Light::LIGHT_SPOT)
		{
			Position lightPosition_cameraspace = viewStack.Top() *
				light[0].position;
			glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1,
				&lightPosition_cameraspace.x);
			Vector3 spotDirection_cameraspace = viewStack.Top() *
				light[0].spotDirection;
			glUniform3fv(m_parameters[U_LIGHT0_SPOTDIRECTION], 1,
				&spotDirection_cameraspace.x);
		}
		else
		{
			Position lightPosition_cameraspace = viewStack.Top() *
				light[0].position;
			glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1,
				&lightPosition_cameraspace.x);
		}

		


	}
	//Render stuff	

	RenderSkybox();
	RenderPhones();
	RenderTables();

	//GroundMesh
	{
		//bottom left
		{
			modelStack.PushMatrix();
			modelStack.Translate(250, 0, 250);
			modelStack.Rotate(-90, 1, 0, 0);
			modelStack.Scale(500, 500, 500);
			RenderMesh(meshList[GEO_GROUND], true);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(750, 0, 250);
			modelStack.Rotate(-90, 1, 0, 0);
			modelStack.Scale(500, 500, 500);
			RenderMesh(meshList[GEO_GROUND], true);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(750, 0, 750);
			modelStack.Rotate(-90, 1, 0, 0);
			modelStack.Scale(500, 500, 500);
			RenderMesh(meshList[GEO_GROUND], true);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(250, 0, 750);
			modelStack.Rotate(-90, 1, 0, 0);
			modelStack.Scale(500, 500, 500);
			RenderMesh(meshList[GEO_GROUND], true);
			modelStack.PopMatrix();
		}
		//bottom right
		{
			modelStack.PushMatrix();
			modelStack.Translate(-250, 0, 250);
			modelStack.Rotate(-90, 1, 0, 0);
			modelStack.Scale(500, 500, 500);
			RenderMesh(meshList[GEO_GROUND], true);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(-750, 0, 250);
			modelStack.Rotate(-90, 1, 0, 0);
			modelStack.Scale(500, 500, 500);
			RenderMesh(meshList[GEO_GROUND], true);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(-750, 0, 750);
			modelStack.Rotate(-90, 1, 0, 0);
			modelStack.Scale(500, 500, 500);
			RenderMesh(meshList[GEO_GROUND], true);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(-250, 0, 750);
			modelStack.Rotate(-90, 1, 0, 0);
			modelStack.Scale(500, 500, 500);
			RenderMesh(meshList[GEO_GROUND], true);
			modelStack.PopMatrix();
		}
		//top right
		{
			modelStack.PushMatrix();
			modelStack.Translate(-250, 0, -250);
			modelStack.Rotate(-90, 1, 0, 0);
			modelStack.Scale(500, 500, 500);
			RenderMesh(meshList[GEO_GROUND], true);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(-750, 0, -250);
			modelStack.Rotate(-90, 1, 0, 0);
			modelStack.Scale(500, 500, 500);
			RenderMesh(meshList[GEO_GROUND], true);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(-750, 0, -750);
			modelStack.Rotate(-90, 1, 0, 0);
			modelStack.Scale(500, 500, 500);
			RenderMesh(meshList[GEO_GROUND], true);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(-250, 0,-750);
			modelStack.Rotate(-90, 1, 0, 0);
			modelStack.Scale(500, 500, 500);
			RenderMesh(meshList[GEO_GROUND], true);
			modelStack.PopMatrix();
		}
		//bottom left
		{
			modelStack.PushMatrix();
			modelStack.Translate(250, 0, -250);
			modelStack.Rotate(-90, 1, 0, 0);
			modelStack.Scale(500, 500, 500);
			RenderMesh(meshList[GEO_GROUND], true);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(750, 0, -250);
			modelStack.Rotate(-90, 1, 0, 0);
			modelStack.Scale(500, 500, 500);
			RenderMesh(meshList[GEO_GROUND], true);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(750, 0, -750);
			modelStack.Rotate(-90, 1, 0, 0);
			modelStack.Scale(500, 500, 500);
			RenderMesh(meshList[GEO_GROUND], true);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(250, 0, -750);
			modelStack.Rotate(-90, 1, 0, 0);
			modelStack.Scale(500, 500, 500);
			RenderMesh(meshList[GEO_GROUND], true);
			modelStack.PopMatrix();
		}
	}
	//buildings
	//1-4
	{
		{
			modelStack.PushMatrix();
			modelStack.Translate(-450, 0, -300);
			modelStack.Rotate(0, 0, 0, 1);

			modelStack.PushMatrix();
			modelStack.Translate(0, 0, 250);
			modelStack.Rotate(90, 0, 1, 0);
			modelStack.Scale(200, 100, 200);
			RenderMesh(meshList[GEO_BUILDING2], true);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(0, 0, 520);
			modelStack.Rotate(90, 0, 1, 0);
			modelStack.Scale(200, 100, 200);
			RenderMesh(meshList[GEO_BUILDING2], true);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(0, 0, 790);
			modelStack.Rotate(-180, 0, 1, 0);
			modelStack.Scale(200, 100, 200);
			RenderMesh(meshList[GEO_BUILDING3], true);
			modelStack.PopMatrix();

			modelStack.Scale(200, 100, 200);
			RenderMesh(meshList[GEO_BUILDING1], true);
			modelStack.PopMatrix();
		}
	}
	//5-8
	{
		modelStack.PushMatrix();
		modelStack.Translate(-200, 0, -300);
		modelStack.Rotate(0, 0, 0, 1);

		modelStack.PushMatrix();
		modelStack.Translate(0, 0, 250);
		modelStack.Rotate(-90, 0, 1, 0);
		modelStack.Scale(200, 100, 200);
		RenderMesh(meshList[GEO_BUILDING3], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(0, 0, 520);
		modelStack.Rotate(-90, 0, 1, 0);
		modelStack.Scale(200, 100, 200);
		RenderMesh(meshList[GEO_BUILDING1], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(0, 0, 790);
		modelStack.Rotate(-180, 0, 1, 0);
		modelStack.Scale(200, 100, 200);
		RenderMesh(meshList[GEO_BUILDING2], true);
		modelStack.PopMatrix();

		modelStack.Scale(200, 100, 200);
		RenderMesh(meshList[GEO_BUILDING4], true);
		modelStack.PopMatrix();
	}
	//9-12
	{
		modelStack.PushMatrix();
		modelStack.Translate(250, 0, 490);
		modelStack.Rotate(180, 0, 1, 0);

		modelStack.PushMatrix();
		modelStack.Translate(0, 0, 250);
		modelStack.Rotate(-90, 0, 1, 0);
		modelStack.Scale(200, 100, 200);
		RenderMesh(meshList[GEO_BUILDING3], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(0, 0, 520);
		modelStack.Rotate(-90, 0, 1, 0);
		modelStack.Scale(200, 100, 200);
		RenderMesh(meshList[GEO_BUILDING1], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(0, 0, 790);
		modelStack.Rotate(-180, 0, 1, 0);
		modelStack.Scale(200, 100, 200);
		RenderMesh(meshList[GEO_BUILDING2], true);
		modelStack.PopMatrix();

		modelStack.Scale(200, 100, 200);
		RenderMesh(meshList[GEO_BUILDING4], true);
		modelStack.PopMatrix();
	}
	//13-16
	{
		modelStack.PushMatrix();
		modelStack.Translate(500, 0, 490);
		modelStack.Rotate(180, 0, 1, 0);

		modelStack.PushMatrix();
		modelStack.Translate(0, 0, 250);
		modelStack.Rotate(90, 0, 1, 0);
		modelStack.Scale(200, 100, 200);
		RenderMesh(meshList[GEO_BUILDING2], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(0, 0, 520);
		modelStack.Rotate(90, 0, 1, 0);
		modelStack.Scale(200, 100, 200);
		RenderMesh(meshList[GEO_BUILDING2], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(0, 0, 790);
		modelStack.Rotate(-180, 0, 1, 0);
		modelStack.Scale(200, 100, 200);
		RenderMesh(meshList[GEO_BUILDING3], true);
		modelStack.PopMatrix();

		modelStack.Scale(200, 100, 200);
		RenderMesh(meshList[GEO_BUILDING1], true);
		modelStack.PopMatrix();
	}
	//background buildings
	{
		modelStack.PushMatrix();
		modelStack.Translate(-450, 0, -850);
		modelStack.Rotate(180, 0, 1, 0);

		modelStack.PushMatrix();
		modelStack.Translate(-260, 0, 0);
		modelStack.Rotate(0, 0, 0, 1);
		modelStack.Scale(200, 100, 200);
		RenderMesh(meshList[GEO_BUILDING1], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(-510, 0, 0);
		modelStack.Rotate(0, 0, 0, 1);
		modelStack.Scale(200, 100, 200);
		RenderMesh(meshList[GEO_BUILDING3], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(-780, 0, 0);
		modelStack.Rotate(0, 0, 0, 1);
		modelStack.Scale(200, 100, 200);
		RenderMesh(meshList[GEO_BUILDING2], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(-1050, 0, 0);
		modelStack.Rotate(0, 0, 0, 1);
		modelStack.Scale(200, 100, 200);
		RenderMesh(meshList[GEO_BUILDING4], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(-1300, 0, 0);
		modelStack.Rotate(0, 0, 0, 1);
		modelStack.Scale(200, 100, 200);
		RenderMesh(meshList[GEO_BUILDING3], true);
		modelStack.PopMatrix();

		modelStack.Scale(200, 100, 200);
		RenderMesh(meshList[GEO_BUILDING2], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(850, 0, -300);
		modelStack.Rotate(90, 0, 1, 0);

		modelStack.PushMatrix();
		modelStack.Translate(-250, 0, 0);
		modelStack.Rotate(0, 0, 0, 1);
		modelStack.Scale(200, 100, 200);
		RenderMesh(meshList[GEO_BUILDING1], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(-520, 0, 0);
		modelStack.Rotate(0, 0, 0, 1);
		modelStack.Scale(200, 100, 200);
		RenderMesh(meshList[GEO_BUILDING2], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(-790, 0, 0);
		modelStack.Rotate(0, 0, 0, 1);
		modelStack.Scale(200, 100, 200);
		RenderMesh(meshList[GEO_BUILDING3], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(-1060, 0, 0);
		modelStack.Rotate(0, 0, 0, 1);
		modelStack.Scale(200, 100, 200);
		RenderMesh(meshList[GEO_BUILDING1], true);
		modelStack.PopMatrix();

		modelStack.Scale(200, 100, 200);
		RenderMesh(meshList[GEO_BUILDING4], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(530, 0, 850);
		modelStack.Rotate(0, 0, 0, 1);

		modelStack.PushMatrix();
		modelStack.Translate(-250, 0, 0);
		modelStack.Rotate(0, 0, 0, 1);
		modelStack.Scale(200, 100, 200);
		RenderMesh(meshList[GEO_BUILDING2], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(-520, 0, 0);
		modelStack.Rotate(0, 0, 0, 1);
		modelStack.Scale(200, 100, 200);
		RenderMesh(meshList[GEO_BUILDING4], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(-790, 0, 0);
		modelStack.Rotate(0, 0, 0, 1);
		modelStack.Scale(200, 100, 200);
		RenderMesh(meshList[GEO_BUILDING3], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(-1060, 0, 0);
		modelStack.Rotate(0, 0, 0, 1);
		modelStack.Scale(200, 100, 200);
		RenderMesh(meshList[GEO_BUILDING3], true);
		modelStack.PopMatrix();

		modelStack.Scale(200, 100, 200);
		RenderMesh(meshList[GEO_BUILDING1], true);
		modelStack.PopMatrix();

	}
	//Roads
	{
		modelStack.PushMatrix();
		modelStack.Translate(-750, -1.9, -600);
		modelStack.Rotate(90, 0, 1, 0);
		modelStack.Scale(200, 200, 200);
		RenderMesh(meshList[GEO_ROAD1], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(-750, -1.9, 250);
		modelStack.Rotate(90, 0, 1, 0);
		modelStack.Scale(1500, 200, 200);
		RenderMesh(meshList[GEO_ROAD2], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(225, -1.9, -600);
		modelStack.Rotate(180, 0, 1, 0);
		modelStack.Scale(1750, 200, 200);
		RenderMesh(meshList[GEO_ROAD2], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(-750, -1.9, -850);
		modelStack.Rotate(90, 0, 1, 0);
		modelStack.Scale(300, 200, 200);
		RenderMesh(meshList[GEO_ROAD2], true);
		modelStack.PopMatrix();

	}

	//render UI
	RenderTutorial();
	RenderProgress();
	RenderComplete();
	RenderWin();
	RenderTimer();
	RenderLoseScreen();
	RenderTextOnScreen(meshList[GEO_TEXT], "DAY 5", Color(0, 1, 0), 3, 20, 55);
}

void  mini_game_5::Exit()
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