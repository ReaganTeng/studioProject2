#include "SceneUI.h"
#include "GL\glew.h"

#include "shader.hpp"
#include "Mtx44.h"
#include "Utility.h"
#include "Application.h"
#include "MeshBuilder.h"
#include "MatrixStack.h"
#include "LoadTGA.h"

#include <sstream>

SceneUI::SceneUI()
{
}

SceneUI::~SceneUI()
{
}

void SceneUI::Init()
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

		camera.Init(Vector3(4, 3, 3), Vector3(0, 0, 0), Vector3(0, 1, 0));

		light[0].type = Light::LIGHT_POINT;
		light[0].position.Set(0, 20, 0);
		light[0].color.Set(1, 1, 1);
		light[0].power = 2;
		light[0].kC = 1.f;
		light[0].kL = 0.01f;
		light[0].kQ = 0.001f;
		light[0].cosCutoff = cos(Math::DegreeToRadian(45));
		light[0].cosInner = cos(Math::DegreeToRadian(30));
		light[0].exponent = 3.f;
		light[0].spotDirection.Set(0.f, 1.f, 0.f);

		//set background color
		glClearColor(0.0f, 0.0f, 1.0f, 0.0f);

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



//LOAD THE MOPDELS
	{

	}

	Mesh::SetMaterialLoc(m_parameters[U_MATERIAL_AMBIENT],
		m_parameters[U_MATERIAL_DIFFUSE],
		m_parameters[U_MATERIAL_SPECULAR],
		m_parameters[U_MATERIAL_SHININESS]);
}



void  SceneUI::Update(double dt)
{
	fps = 1.f / dt;




	static bool bLButtonState = false;
	if (!bLButtonState && Application::IsMousePressed(0))
	{
		bLButtonState = true;
		std::cout << "LBUTTON DOWN" << std::endl;


		//Converting Viewport space to UI space
		double x, y;
		Application::GetCursorPos(&x, &y);
		unsigned w = Application::GetWindowWidth();
		unsigned h = Application::GetWindowHeight();
		float posX = x / w; //convert (0,800) to (0,80)
		float posY = 60 - (y / h); //convert (600,0) to (0,60)
		std::cout << "posX:" << posX << " , posY:" << posY <<
			std::endl;
		if (posX > 0.44 && posX < 0.56 && posY >
			59.42 && posY < 59.59)
		{
			std::cout << "Hit!" << std::endl;
			//trigger user action or function
		}
		else
		{
			std::cout << "Miss!" << std::endl;

		}
	}
	else if (bLButtonState && !Application::IsMousePressed(0))
	{
		bLButtonState = false;
		std::cout << "LBUTTON UP" << std::endl;

		//Converting Viewport space to UI space
		double x, y;
		Application::GetCursorPos(&x, &y);
		unsigned w = Application::GetWindowWidth();
		unsigned h = Application::GetWindowHeight();
		float posX = x / w; //convert (0,800) to (0,80)
		float posY = 60 - (y / h); //convert (600,0) to (0,60)
		std::cout << "posX:" << posX << " , posY:" << posY <<
			std::endl;
		if (posX > 0.44 && posX < 0.56 && posY >
			59.42 && posY < 59.59)
		{
			std::cout << "Hit!" << std::endl;
			//trigger user action or function
		}
		else
		{
			std::cout << "Miss!" << std::endl;

		}
	}
	static bool bRButtonState = false;
	if (!bRButtonState && Application::IsMousePressed(1))
	{
		bRButtonState = true;
		std::cout << "RBUTTON DOWN" << std::endl;

		//Converting Viewport space to UI space
		double x, y;
		Application::GetCursorPos(&x, &y);
		unsigned w = Application::GetWindowWidth();
		unsigned h = Application::GetWindowHeight();
		float posX = x / w; //convert (0,800) to (0,80)
		float posY = 60 - (y / h); //convert (600,0) to (0,60)
		std::cout << "posX:" << posX << " , posY:" << posY <<
			std::endl;
		if (posX > 0.44 && posX < 0.56 && posY >
			59.42 && posY < 59.59)
		{
			std::cout << "Hit!" << std::endl;
			//trigger user action or function
		}
		else
		{
			std::cout << "Miss!" << std::endl;

		}
	}
	else if (bRButtonState && !Application::IsMousePressed(1))
	{
		bRButtonState = false;
		std::cout << "RBUTTON UP" << std::endl;

		//Converting Viewport space to UI space
		double x, y;
		Application::GetCursorPos(&x, &y);
		unsigned w = Application::GetWindowWidth();
		unsigned h = Application::GetWindowHeight();
		float posX = x / w; //convert (0,800) to (0,80)
		float posY = 60 - (y / h); //convert (600,0) to (0,60)
		std::cout << "posX:" << posX << " , posY:" << posY <<
			std::endl;
		if (posX > 0.44 && posX < 0.56 && posY >
			59.42 && posY < 59.59)
		{
			std::cout << "Hit!" << std::endl;
			//trigger user action or function
		}
		else
		{
			std::cout << "Miss!" << std::endl;

		}
	}


	

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

		camera.Update(dt);
	}
}


void  SceneUI::RenderMesh(Mesh* mesh, bool enableLight)
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

void SceneUI::RenderSkybox()
{
	
}

void SceneUI::RenderText(Mesh* mesh, std::string text, Color color)
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

void SceneUI::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y)
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

void SceneUI::RenderMeshOnScreen(Mesh* mesh, int x, int y, int
	sizex, int sizey)
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
	modelStack.Translate(40, 30, 0);
	modelStack.Scale(10, 10, 0);

	RenderMesh(mesh, false); //UI should not have light
	projectionStack.PopMatrix();
	viewStack.PopMatrix();
	modelStack.PopMatrix();
	glEnable(GL_DEPTH_TEST);
}



void  SceneUI ::Render()
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



	

	
}

void  SceneUI::Exit()
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