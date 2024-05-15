#include "scenebasic_uniform.h"

#include <cstdio>
#include <cstdlib>

#include <string>
using std::string;

#include <sstream>
#include <iostream>
using std::cerr;
using std::endl;

#include "helper/glutils.h"
#include <glm/gtc/matrix_transform.hpp>
#include "helper/texture.h"

using glm::vec3;
using glm::mat4;
using glm::vec4;
using glm::mat3;

SceneBasic_Uniform::SceneBasic_Uniform() : mesh(ObjMesh::load("media/comp3015.obj", true)), mesh1(ObjMesh::load("media/comp3015 room.obj", true)), tPrev(0.0f), lightPos(5.0f, 5.0f, 5.0f, 1.0f) {

}

void SceneBasic_Uniform::initScene()
{
	compile();

	glClearColor(0.1f, 0.1f, 0.2f, 1.0f);

	glEnable(GL_DEPTH_TEST);

	view = glm::lookAt(vec3(14.0f, 1.0f, 1.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));

	projection = glm::perspective(glm::radians(50.0f), (float)width / height, 0.5f, 100.f);



	lightAngle = 80.0f;
	lightRotationSpeed = 1.5f;

	prog.setUniform("Light[0].L", glm::vec3(45.0f));
	prog.setUniform("Light[0].Position", view * lightPos);
	prog.setUniform("Light[1].L", glm::vec3(0.3f));
	prog.setUniform("Light[1].Position", glm::vec4(0, 0.15f, -1.0f, 0));
	prog.setUniform("Light[2].L", glm::vec3(45.0f));
	prog.setUniform("Light[2].Position", view * glm::vec4(-7, 3, 7, 1));
}

void SceneBasic_Uniform::compile()
{
	try {
		prog.compileShader("shader/basic_uniform.vert");
		prog.compileShader("shader/basic_uniform.frag");
		prog.link();
		prog.use();
	} catch (GLSLProgramException &e) {
		cerr << e.what() << endl;
		exit(EXIT_FAILURE);
	}
}

void SceneBasic_Uniform::update( float t )
{
	float deltaT = t - tPrev;
	if (tPrev == 0.0f)
	{
		deltaT = 0.0f;
	}

	tPrev = t;

	if (animating())
	{
		lightAngle = glm::mod(lightAngle + deltaT * lightRotationSpeed, glm::two_pi<float>());
		lightPos.x = glm::cos(lightAngle) * 7.0f;
		lightPos.y = 3.0f;
		lightPos.z = glm::sin(lightAngle) * 7.0f;
	}
}

void SceneBasic_Uniform::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	prog.setUniform("Light[0].Position", view * lightPos);
	
	drawScene();
}

void SceneBasic_Uniform::setMatrices()
{
	mat4 mv = view * model;

	prog.setUniform("ModelViewMatrix", mv);
	prog.setUniform("NormalMatrix", glm::mat3(mv));
	prog.setUniform("MVP", projection * mv);
}

void SceneBasic_Uniform::resize(int w, int h)
{
	glViewport(0, 0, w, h);

    width = w;
    height = h;

	projection = glm::perspective(glm::radians(60.0f), (float)w / h, 0.3f, 100.0f);
}

void SceneBasic_Uniform::drawScene()
{
	drawModel();
	drawRoom(0.3f, 1, glm::vec3(1, 0.71f, 0.29f));
}

void SceneBasic_Uniform::drawRoom(float rough, int metal, const glm::vec3& color)
{
	model = glm::mat4(1.0f);
	model = glm::rotate(model, glm::radians(265.0f), vec3(0.0f, 1.0f, 0.0f));

	prog.setUniform("Material.Rough", rough);
	prog.setUniform("Material.Metal", metal);
	prog.setUniform("Material.Color", color);
	setMatrices();
	mesh1->render();

}

void SceneBasic_Uniform::drawModel()
{
	model = glm::mat4(1.0f);
	model = glm::translate(model, vec3(0.0f, -2.0f, 0.0f));
	model = glm::rotate(model, glm::radians(270.0f), vec3(0.0f, 1.0f, 0.0f));

	GLuint texID = Texture::loadTexture("media/ImphenziaPalette02-Albedo.png");
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texID);

	setMatrices();
	mesh->render();
	glBindTexture(GL_TEXTURE_2D, 0);

}
