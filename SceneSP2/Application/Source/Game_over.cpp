#include "Game_over.h"
#include "GL\glew.h"
#include "shader.hpp"
#include "Mtx44.h"
#include "Utility.h"
#include "Application.h"
#include "MeshBuilder.h"
#include "MatrixStack.h"
#include "LoadTGA.h"
#include <sstream>
#include <ctime>


Game_over::Game_over()
{
	srand((unsigned)time(NULL));

	scale_translate_skybox = 1000;
	scale_ground = 300;
}

void Game_over::Init()
{
	//EVERYTHING NEEDED, DON'T TOUCH
	{
		// Init VBO here
		Mtx44 projection;

		projection.SetToPerspective(45.f, 4.f / 3.f, 0.1f, 1000.f);
		projectionStack.LoadMatrix(projection);

		glEnable(GL_CULL_FACE);
		glEnable(GL_BLEND);

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		//Vector3(0, 60, 88), Vector3(0, 60, 0), Vector3(0, 1, 0)
		camera.Init(Vector3(5, 25, 0), Vector3(30, 17, 0), Vector3(0, 1, 0));

		light[0].type = Light::LIGHT_POINT;
		light[0].position.Set(0, 20, 0);
		light[0].color.Set(1, 1, 1);
		light[0].power = 10.f;
		light[0].kC = 0.5f;
		light[0].kL = 0.01f;
		light[0].kQ = 0.4f;
		light[0].cosCutoff = cos(Math::DegreeToRadian(45));
		light[0].cosInner = cos(Math::DegreeToRadian(30));
		light[0].exponent = 1.f;
		light[0].spotDirection.Set(0.f, 1.f, 0.f);

		//set background color
		glClearColor(0.0f, 1.0f, 1.0f, 0.0f);

		//generate default VAO for now
		glGenVertexArrays(1, &m_vertexArrayID);
		glBindVertexArray(m_vertexArrayID);

		//load vertex and fragment shaders
		m_programID = LoadShaders("Shader//Texture.vertexshader",
			"Shader//Text.fragmentshader");

		m_parameters[U_LIGHT0_TYPE] = glGetUniformLocation(m_programID, "lights[0].type");
		m_parameters[U_LIGHT0_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[0].spotDirection");
		m_parameters[U_LIGHT0_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[0].cosCutoff");
		m_parameters[U_LIGHT0_COSINNER] = glGetUniformLocation(m_programID, "lights[0].cosInner");
		m_parameters[U_LIGHT0_EXPONENT] = glGetUniformLocation(m_programID, "lights[0].exponent");
		m_parameters[U_NUMLIGHTS] = glGetUniformLocation(m_programID, "numLights");
		m_parameters[U_MVP] = glGetUniformLocation(m_programID, "MVP");
		m_parameters[U_MODELVIEW] = glGetUniformLocation(m_programID, "MV");
		m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE] = glGetUniformLocation(m_programID, "MV_inverse_transpose");
		m_parameters[U_MATERIAL_AMBIENT] = glGetUniformLocation(m_programID, "material.kAmbient");
		m_parameters[U_MATERIAL_DIFFUSE] = glGetUniformLocation(m_programID, "material.kDiffuse");
		m_parameters[U_MATERIAL_SPECULAR] = glGetUniformLocation(m_programID, "material.kSpecular");
		m_parameters[U_MATERIAL_SHININESS] = glGetUniformLocation(m_programID, "material.kShininess");
		m_parameters[U_LIGHT0_POSITION] = glGetUniformLocation(m_programID, "lights[0].position_cameraspace");
		m_parameters[U_LIGHT0_COLOR] = glGetUniformLocation(m_programID, "lights[0].color");
		m_parameters[U_LIGHT0_POWER] = glGetUniformLocation(m_programID, "lights[0].power");
		m_parameters[U_LIGHT0_KC] = glGetUniformLocation(m_programID, "lights[0].kC");
		m_parameters[U_LIGHT0_KL] = glGetUniformLocation(m_programID, "lights[0].kL");
		m_parameters[U_LIGHT0_KQ] = glGetUniformLocation(m_programID, "lights[0].kQ");
		m_parameters[U_LIGHTENABLED] = glGetUniformLocation(m_programID, "lightEnabled");

		// Get a handle for our "colorTexture" uniform
		m_parameters[U_COLOR_TEXTURE_ENABLED] = glGetUniformLocation(m_programID, "colorTextureEnabled");
		m_parameters[U_COLOR_TEXTURE] = glGetUniformLocation(m_programID, "colorTexture");
		m_parameters[U_TEXT_ENABLED] =
			glGetUniformLocation(m_programID, "textEnabled");
		m_parameters[U_TEXT_COLOR] = glGetUniformLocation(m_programID,
			"textColor");

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


		//Enable depth test
		glEnable(GL_DEPTH_TEST);

		//Get a handle for our "MVP" uniform
		m_parameters[U_MVP] = glGetUniformLocation(m_programID, "MVP");
	}

	meshList[GEO_AXES] = MeshBuilder::GenerateAxes("reference", 1000, 1000, 1000);

	//LOAD THE MOPDELS
	{

		meshList[GEO_LIGHTBALL] = MeshBuilder::GenerateSphere("lightball", Color(1, 1, 1), 50, 50, 1.0f);

		//GenerateQuad(const std::string& meshName, Color color, float length = 1.f);
		meshList[GEO_TOP] = MeshBuilder::GenerateQuad("top", (1.f, 1.f, 1.f), 1.f);
		meshList[GEO_TOP]->textureID = LoadTGA("Image//SceneMain//indoors-skyboxes//zeus_up.tga");
		meshList[GEO_TOP]->position.Set(0, scale_translate_skybox / 2, 0);

		meshList[GEO_BOTTOM] = MeshBuilder::GenerateQuad("bottom", (1.f, 1.f, 1.f), 1.f);
		meshList[GEO_BOTTOM]->textureID = LoadTGA("Image//SceneMain//indoors-skyboxes//zeus_dn.tga");
		meshList[GEO_BOTTOM]->position.Set(0, -scale_translate_skybox / 2, 0);

		meshList[GEO_LEFT] = MeshBuilder::GenerateQuad("left", (1.f, 1.f, 1.f), 1.f);
		meshList[GEO_LEFT]->textureID = LoadTGA("Image//SceneMain//indoors-skyboxes//zeus_lf.tga");
		meshList[GEO_LEFT]->position.Set(scale_translate_skybox / 2, 0, 0);

		meshList[GEO_RIGHT] = MeshBuilder::GenerateQuad("right", (1.f, 1.f, 1.f), 1.f);
		meshList[GEO_RIGHT]->textureID = LoadTGA("Image//SceneMain//indoors-skyboxes//zeus_rt.tga");
		meshList[GEO_RIGHT]->position.Set(-scale_translate_skybox / 2, 0, 0);

		meshList[GEO_BACK] = MeshBuilder::GenerateQuad("back", (1.f, 1.f, 1.f), 1.f);
		meshList[GEO_BACK]->textureID = LoadTGA("Image//SceneMain//indoors-skyboxes//zeus_bk.tga");
		meshList[GEO_BACK]->position.Set(0, 0, scale_translate_skybox / 2);

		meshList[GEO_FRONT] = MeshBuilder::GenerateQuad("front", (1.f, 1.f, 1.f), 1.f);
		meshList[GEO_FRONT]->textureID = LoadTGA("Image//SceneMain//indoors-skyboxes//zeus_ft.tga");
		meshList[GEO_FRONT]->position.Set(0, 0, -scale_translate_skybox / 2);

		meshList[GEO_GROUND] = MeshBuilder::GenerateQuad("front", (1.f, 1.f, 1.f), 1.f);
		meshList[GEO_GROUND]->textureID = LoadTGA("Image//SceneMain//indoors-skyboxes//Carpet.tga");

		meshList[GEO_PLAYERSPHERE] = MeshBuilder::GenerateSphere("playersphere", Color(1, 1, 1), 50, 50, 1.0f);

		meshList[GEO_DESK] = MeshBuilder::GenerateOBJMTL("desk",
			"OBJ//stall.obj", "OBJ//stall.mtl");
		meshList[GEO_DESK]->position.Set(-50, 1, 0);
		

		meshList[GEO_COMPUTER_CASE] = MeshBuilder::GenerateOBJMTL("computer_case",
			"OBJ//SceneMain//com_case.obj", "OBJ//SceneMain//com_case.mtl");
		meshList[GEO_COMPUTER_CASE]->textureID = LoadTGA("OBJ//SceneMain//com_case.tga");

		meshList[GEO_MONITOR] = MeshBuilder::GenerateOBJMTL("monitor",
			"OBJ//SceneMain//monitor.obj", "OBJ//SceneMain//monitor.mtl");
		meshList[GEO_MONITOR]->textureID = LoadTGA("OBJ//SceneMain//monitor.tga");


		meshList[GEO_HOUSE] = MeshBuilder::GenerateOBJMTL("house",
			"OBJ//SceneMain//house.obj", "OBJ//SceneMain//house.mtl");
		meshList[GEO_HOUSE]->textureID = LoadTGA("OBJ//SceneMain//Stone_tiled_homemade_simple.tga");

		

		

		meshList[GEO_POLICEHELMET] = MeshBuilder::GenerateOBJMTL("police helmet", "OBJ//blue_helmet.obj", "OBJ//blue_helmet.mtl");
		// Police model
		meshList[GEO_HEAD] = MeshBuilder::GenerateSphere("STAR", Color(1, 1, 1), 30, 30, 2);
		meshList[GEO_HEAD]->material.kAmbient.Set(0.3f, 0.3f, 0.3f);
		meshList[GEO_HEAD]->material.kDiffuse.Set(0.2f, 0.2f, 0.2f);
		meshList[GEO_HEAD]->material.kSpecular.Set(0.3f, 0.3f, 0.3f);
		meshList[GEO_HEAD]->material.kShininess = 1.f;


		meshList[GEO_LIMBS] = MeshBuilder::GenerateCylinder("STAR", Color(0, 0, 1), 30, 10, 5);
		meshList[GEO_LIMBS]->material.kAmbient.Set(0.3f, 0.3f, 0.3f);
		meshList[GEO_LIMBS]->material.kDiffuse.Set(0.2f, 0.2f, 0.2f);
		meshList[GEO_LIMBS]->material.kSpecular.Set(0.3f, 0.3f, 0.3f);
		meshList[GEO_LIMBS]->material.kShininess = 1.f;

		meshList[GEO_PALM] = MeshBuilder::GenerateSphere("STAR", Color(1, 1, 1), 30, 10, 0);
		meshList[GEO_PALM]->material.kAmbient.Set(0.3f, 0.3f, 0.3f);
		meshList[GEO_PALM]->material.kDiffuse.Set(0.2f, 0.2f, 0.2f);
		meshList[GEO_PALM]->material.kSpecular.Set(0.3f, 0.3f, 0.3f);
		meshList[GEO_PALM]->material.kShininess = 1.f;


		meshList[GEO_FEET] = MeshBuilder::GenerateCube("STAR", Color(0.545098039215686, 0.270588235294117, 0.074509803921568), 1);
		meshList[GEO_FEET]->material.kAmbient.Set(0.3f, 0.3f, 0.3f);
		meshList[GEO_FEET]->material.kDiffuse.Set(0.2f, 0.2f, 0.2f);
		meshList[GEO_FEET]->material.kSpecular.Set(0.3f, 0.3f, 0.3f);
		meshList[GEO_FEET]->material.kShininess = 1.f;

		meshList[GEO_EYES] = MeshBuilder::GenerateTorus("STAR", Color(0, 0, 0), 20, 20, 3, 2);
		meshList[GEO_EYES]->material.kAmbient.Set(0.3f, 0.3f, 0.3f);
		meshList[GEO_EYES]->material.kDiffuse.Set(0.2f, 0.2f, 0.2f);
		meshList[GEO_EYES]->material.kSpecular.Set(0.3f, 0.3f, 0.3f);
		meshList[GEO_EYES]->material.kShininess = 1.f;

		meshList[GEO_MOUTH] = MeshBuilder::GenerateHalfTorus("STAR", Color(0, 0, 0), 20, 20, 3, 2);
		meshList[GEO_MOUTH]->material.kAmbient.Set(0.3f, 0.3f, 0.3f);
		meshList[GEO_MOUTH]->material.kDiffuse.Set(0.2f, 0.2f, 0.2f);
		meshList[GEO_MOUTH]->material.kSpecular.Set(0.3f, 0.3f, 0.3f);
		meshList[GEO_MOUTH]->material.kShininess = 1.f;

	}

	meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16,
		16);
	meshList[GEO_TEXT]->textureID = LoadTGA("Image//MS_gothic.tga");

	Mesh::SetMaterialLoc(m_parameters[U_MATERIAL_AMBIENT],
		m_parameters[U_MATERIAL_DIFFUSE],
		m_parameters[U_MATERIAL_SPECULAR],
		m_parameters[U_MATERIAL_SHININESS]);
}

void Game_over::Update(double dt)
{

	
	
		
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

			if (Application::IsKeyPressed('A'))
				translate_x -= (float)(50 * dt);
			//MOVE LIGHT TO POSITIVE SIDE OF Z AXIS
			if (Application::IsKeyPressed('S'))
				translate_z += (float)(50 * dt);
			//MOVE LIGHT TO NEGATIVE SIDE OF x AXIS
			if (Application::IsKeyPressed('D'))
				translate_x += (float)(50 * dt);
			//MOVE LIGHT TO POSITIVE SIDE OF X AXIS
			if (Application::IsKeyPressed('W'))
				translate_z -= (float)(50 * dt);

			if (translate_x >= 50)
			{
				translate_x = 50;
			}
			else if (translate_x <= -50)
			{
				translate_x = -50;
			}
			else if (translate_z <= -50)
			{
				translate_z = -50;
			}
			else if (translate_z >= 50)
			{
				translate_z = 50;
			}
		}
	}
	

	
void Game_over::RenderPolice()
	{
		//HEAD
		modelStack.PushMatrix();
		modelStack.Translate(30, 25, 0);
		modelStack.Scale(0.2, 0.1, 0.2);
		modelStack.Rotate(-90, 0, 1, 0);

		// Helmet
		modelStack.PushMatrix();
		{
			modelStack.Translate(0, 5, 0);
			modelStack.Scale(2, 2, 2);
			modelStack.Rotate(-90, 1, 0, 0);
			RenderMesh(meshList[GEO_POLICEHELMET], true);
		}
		modelStack.PopMatrix();

		//torso
		{
			modelStack.PushMatrix();
			modelStack.Translate(0, -40, 0);
			modelStack.Scale(1, 5, 1);
			modelStack.Rotate(0, 0, 0, 1);
			RenderMesh(meshList[GEO_LIMBS], true);
			modelStack.PopMatrix();
		}

		//LEFT ARM
		{
			modelStack.PushMatrix();
			modelStack.Translate(15, -20, 15);
			modelStack.Rotate(0, 0, 0, 1);


			//left for arm
			modelStack.PushMatrix();
			modelStack.Translate(5, 0, 25);
			modelStack.Rotate(90, 90, 0, 1);
			modelStack.Scale(1, 3, 1);
			RenderMesh(meshList[GEO_LIMBS], true);
			modelStack.PopMatrix();

			//left back arm
			modelStack.PushMatrix();
			modelStack.Rotate(90, -1.5, 0, 1);
			modelStack.Scale(1, 2, 1);
			RenderMesh(meshList[GEO_LIMBS], true);
			modelStack.PopMatrix();


			RenderMesh(meshList[GEO_PALM], true);
			modelStack.PopMatrix();
		}

		//RIGHT ARM
		{
			modelStack.PushMatrix();
			modelStack.Translate(-15, -20, 15);

			//right for arm
			modelStack.PushMatrix();
			modelStack.Translate(-5, 0, 25);
			modelStack.Rotate(90, 90, 0, 1);
			modelStack.Scale(1, 3, 1);
			RenderMesh(meshList[GEO_LIMBS], true);
			modelStack.PopMatrix();

			//right back arm
			modelStack.PushMatrix();
			modelStack.Rotate(90, 1.5, 0, 1);
			modelStack.Scale(1, 2, 1);
			RenderMesh(meshList[GEO_LIMBS], true);
			modelStack.PopMatrix();


			RenderMesh(meshList[GEO_PALM], true);
			modelStack.PopMatrix();
		}


		//ENTIRE RIGHT LEG
		{


			//right thigh joint
			modelStack.PushMatrix();
			modelStack.Translate(-4, -60, 0);
			modelStack.Rotate(0, 0, 0, 1);


			//right thigh
			modelStack.PushMatrix();
			modelStack.Translate(0, -15, 0);
			modelStack.Scale(1, 2, 1);
			RenderMesh(meshList[GEO_LIMBS], true);
			modelStack.PopMatrix();


			//right lower leg joint
			modelStack.PushMatrix();
			modelStack.Translate(0, -5, 0);
			modelStack.Rotate(30, 30, 0, 1);

			//right lower leg
			modelStack.PushMatrix();
			modelStack.Translate(0, -30, 10);
			modelStack.Scale(1, 3, 1);
			RenderMesh(meshList[GEO_LIMBS], true);
			modelStack.PopMatrix();

			//right feet
			modelStack.PushMatrix();
			modelStack.Translate(0, -45, 15);
			modelStack.Scale(10, 5, 20);
			RenderMesh(meshList[GEO_FEET], true);
			modelStack.PopMatrix();


			RenderMesh(meshList[GEO_PALM], true);
			modelStack.PopMatrix();




			RenderMesh(meshList[GEO_PALM], true);
			modelStack.PopMatrix();
		}


		//ENTIRE LEFT LEG
		{
			//left thigh joint
			modelStack.PushMatrix();
			modelStack.Translate(4, -60, 0);
			modelStack.Rotate(0, 0, 0, 1);

			//left thigh
			modelStack.PushMatrix();
			modelStack.Translate(0, -15, 0);
			modelStack.Scale(1, 2, 1);
			RenderMesh(meshList[GEO_LIMBS], true);
			modelStack.PopMatrix();


			//left lower leg joint
			modelStack.PushMatrix();
			modelStack.Translate(0, -5, 0);
			modelStack.Rotate(30, 30, 0, 1);

			//left lower leg
			modelStack.PushMatrix();
			modelStack.Translate(0, -30, 10);
			modelStack.Scale(1, 3, 1);
			//modelStack.Rotate(0, 30, 0, 1);
			RenderMesh(meshList[GEO_LIMBS], true);
			modelStack.PopMatrix();

			//left feet
			modelStack.PushMatrix();
			modelStack.Translate(0, -45, 15);
			modelStack.Scale(10, 5, 20);
			RenderMesh(meshList[GEO_FEET], true);
			modelStack.PopMatrix();


			RenderMesh(meshList[GEO_PALM], true);
			modelStack.PopMatrix();




			RenderMesh(meshList[GEO_PALM], true);
			modelStack.PopMatrix();
		}



		modelStack.Rotate(-30 + 3, 0, 30 * 3, 1);
		//modelStack.Rotate( 3, 3, 0, 1);

		//ENTIRE EYES
		{
			modelStack.PushMatrix();
			modelStack.Translate(-5, 5, 15);
			modelStack.Scale(2, 2, 2);
			modelStack.Rotate(90, 0, 0, 1);
			RenderMesh(meshList[GEO_EYES], true);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(5, 5, 15);
			modelStack.Scale(2, 2, 2);
			modelStack.Rotate(90, 0, 0, 1);
			RenderMesh(meshList[GEO_EYES], true);
			modelStack.PopMatrix();
		}


		//ENTIRE MOUTH
		{
			modelStack.PushMatrix();
			modelStack.Translate(0, -10, 13);
			modelStack.Rotate(-90, -180, 360, 1);
			modelStack.Scale(2, 2, 2);
			RenderMesh(meshList[GEO_MOUTH], true);
			modelStack.PopMatrix();
		}

		modelStack.Scale(10, 10, 10);
		RenderMesh(meshList[GEO_HEAD], true);
		modelStack.PopMatrix();
	}
	


void Game_over::RenderMesh(Mesh* mesh, bool enableLight)
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

void Game_over::RenderSkybox()
{
	modelStack.PushMatrix();
	modelStack.Translate(meshList[GEO_TOP]->position.x, meshList[GEO_TOP]->position.y,
		meshList[GEO_TOP]->position.z);
	modelStack.Rotate(90, 1, 0, 0);
	modelStack.Rotate(-90, 0, 0, 1);
	modelStack.Scale(scale_translate_skybox, scale_translate_skybox,
		0);
	RenderMesh(meshList[GEO_TOP], false);
	modelStack.PopMatrix();


	modelStack.PushMatrix();
	modelStack.Translate(meshList[GEO_BOTTOM]->position.x, meshList[GEO_BOTTOM]->position.y,
		meshList[GEO_BOTTOM]->position.z);
	modelStack.Rotate(-90, 1, 0, 0);
	modelStack.Rotate(90, 0, 0, 1);
	modelStack.Scale(scale_translate_skybox, scale_translate_skybox,
		0);
	RenderMesh(meshList[GEO_BOTTOM], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(meshList[GEO_LEFT]->position.x, meshList[GEO_LEFT]->position.y,
		meshList[GEO_LEFT]->position.z);
	modelStack.Rotate(-90, 0, 1, 0);
	modelStack.Scale(scale_translate_skybox, scale_translate_skybox,
		0);
	RenderMesh(meshList[GEO_LEFT], false);
	modelStack.PopMatrix();




	modelStack.PushMatrix();
	modelStack.Translate(meshList[GEO_RIGHT]->position.x, meshList[GEO_RIGHT]->position.y,
		meshList[GEO_RIGHT]->position.z);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Scale(scale_translate_skybox, scale_translate_skybox,
		0);
	RenderMesh(meshList[GEO_RIGHT], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(meshList[GEO_FRONT]->position.x, meshList[GEO_FRONT]->position.y,
		meshList[GEO_FRONT]->position.z);
	//modelStack.Rotate(180, 0, 1, 0);
	modelStack.Scale(scale_translate_skybox, scale_translate_skybox,
		0);
	RenderMesh(meshList[GEO_FRONT], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(meshList[GEO_BACK]->position.x, meshList[GEO_BACK]->position.y,
		meshList[GEO_BACK]->position.z);
	modelStack.Rotate(180, 0, 1, 0);
	modelStack.Scale(scale_translate_skybox, scale_translate_skybox,
		0);
	RenderMesh(meshList[GEO_BACK], false);
	modelStack.PopMatrix();


}

void Game_over::RenderText(Mesh* mesh, std::string text, Color color)
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

void Game_over::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y)
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

void Game_over::RenderMeshOnScreen(Mesh* mesh, int x, int y, float
	sizex, float sizey)
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
	//modelStack.Translate(40, 30, 0);
	modelStack.Translate(x, y, 0);
	//modelStack.Scale(10, 10, 0);
	modelStack.Scale(sizex, sizey, 0);

	RenderMesh(mesh, false); //UI should not have light
	projectionStack.PopMatrix();
	viewStack.PopMatrix();
	modelStack.PopMatrix();
	glEnable(GL_DEPTH_TEST);
}

void Game_over::RenderPhysicalWorld()
{
	//Set view matrix using camera settings
	viewStack.LoadIdentity();
	viewStack.LookAt(
		camera.position.x, camera.position.y, camera.position.z,
		camera.target.x, camera.target.y, camera.target.z,
		camera.up.x, camera.up.y, camera.up.z
	);
	modelStack.LoadIdentity();

	//RENDER LIGHTING
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

	RenderMesh(meshList[GEO_AXES], false);

	//RENDER THE CAMERA AND PHYSICAL OBJECTS
	{
		//MAKE THE SKYBOX TRANSLATE CORRECTLY WITH CAMERA
		modelStack.PushMatrix();
		modelStack.Translate(camera.position.x, 0, camera.position.z);

		RenderSkybox();

		RenderMesh(meshList[GEO_PLAYERSPHERE], true);
		modelStack.PopMatrix();

		//RENDER THE LIGHTBALL
		modelStack.PushMatrix();
		modelStack.Translate(light[0].position.x - 50, light[0].position.y + 40,
			light[0].position.z);
		modelStack.Scale(5, 5, 5);
		RenderMesh(meshList[GEO_LIGHTBALL], false);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(0, 1, 0);
		modelStack.Rotate(-90, 1, 0, 0);
		modelStack.Scale(scale_ground, scale_ground, 0);
		RenderMesh(meshList[GEO_GROUND], false);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		//modelStack.Translate(0, -5, 0);
		modelStack.Rotate(90, 0, 1, 0);
		modelStack.Scale(6, 6, 6);
		RenderMesh(meshList[GEO_HOUSE], true);
		modelStack.PopMatrix();

		//std::cout << meshList[GEO_HOUSE]->position.z;

		modelStack.PushMatrix();
		modelStack.Translate(meshList[GEO_DESK]->position.x, meshList[GEO_DESK]->position.y, meshList[GEO_DESK]->position.z);
		modelStack.Scale(45, 45, 45);
		RenderMesh(meshList[GEO_DESK], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(-50, 25, 5);
		modelStack.Rotate(90, 0, 1, 0);
		modelStack.Scale(0.3f, 0.3f, 0.3f);
		RenderMesh(meshList[GEO_COMPUTER_CASE], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(-50, 18, -5);
		modelStack.Rotate(90, 0, 1, 0);
		modelStack.Rotate(-90, 1, 0, 0);
		modelStack.Scale(0.3, 0.3, 0.3);
		RenderMesh(meshList[GEO_MONITOR], true);
		modelStack.PopMatrix();
	}

	RenderPolice();
}

void Game_over::RenderUI()
{
	RenderTextOnScreen(meshList[GEO_TEXT], "You're still arrested", Color(1, 0, 0), 3, 15, 30);
	RenderTextOnScreen(meshList[GEO_TEXT], "CRIME DOESN'T PAY", Color(1, 0, 0), 3, 15, 27);
}

void Game_over::Render()
{
	// Render VBO here
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	RenderPhysicalWorld();
	RenderUI();
}

void Game_over::Exit()
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

Game_over::~Game_over()
{
}