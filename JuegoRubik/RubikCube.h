#ifndef RUBIK_CUBE_H_
#define RUBIK_CUBE_H_
#include <glad/glad.h>
#include <glfw/glfw3.h>

#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>
#include <glm/glm/gtc/constants.hpp>
#include <glm/glm/gtc/quaternion.hpp>
#include <glm/glm/gtx/quaternion.hpp>

#include <vector>
#include <string>
#include <sstream>
#include <stack>
#include <queue>
#include <algorithm>

#include "shader.h"
#include "defineRubik.h"

// usa 26 shaders para cada cubo
static GLfloat cube_fill_vertices[] =
{
	// extra black color for drawing borders
	// UP face - white color
	-0.5f,  0.5f, -0.5f, /*1.0f, 1.0f, 1.0f,*/ 0.0f, 1.0f, // 0 - 0
	 0.5f,  0.5f, -0.5f, /*1.0f, 1.0f, 1.0f,*/ 1.0f, 1.0f,// 1 - 1
	-0.5f,  0.5f,  0.5f, /*1.0f, 1.0f, 1.0f,*/ 0.0f, 0.0f,// 2 - 2
	 0.5f,  0.5f,  0.5f, /*1.0f, 1.0f, 1.0f,*/ 1.0f, 0.0f,// 3 - 3

	// LEFT face - orange color
	-0.5f,  0.5f, -0.5f, /*0.972f, 0.470f, 0.227f,*/ 0.0f, 1.0f,// 4 - 0
	-0.5f,  0.5f,  0.5f, /*0.972f, 0.470f, 0.227f,*/ 1.0f, 1.0f,// 5 - 2
	-0.5f, -0.5f, -0.5f, /*0.972f, 0.470f, 0.227f,*/ 0.0f, 0.0f,// 6 - 4
	-0.5f, -0.5f,  0.5f, /*0.972f, 0.470f, 0.227f,*/ 1.0f, 0.0f,// 7 - 5

	// FRONT face - green color
	-0.5f,  0.5f,  0.5f, /*0.0f, 1.0f, 0.0f,*/ 0.0f, 1.0f,// 8 - 2
	 0.5f,  0.5f,  0.5f, /*0.0f, 1.0f, 0.0f,*/ 1.0f, 1.0f,// 9 - 3
	-0.5f, -0.5f,  0.5f, /*0.0f, 1.0f, 0.0f,*/ 0.0f, 0.0f,// 10 - 5
	 0.5f, -0.5f,  0.5f, /*0.0f, 1.0f, 0.0f,*/ 1.0f, 0.0f,// 11 - 6

	// RIGHT face - red color
	 0.5f,  0.5f,  0.5f, /*1.0f, 0.0f, 0.0f,*/ 0.0f, 1.0f,// 12 - 3
	 0.5f,  0.5f, -0.5f, /*1.0f, 0.0f, 0.0f,*/ 1.0f, 1.0f,// 13 - 1
	 0.5f, -0.5f,  0.5f, /*1.0f, 0.0f, 0.0f,*/ 0.0f, 0.0f,// 14 - 6
	 0.5f, -0.5f, -0.5f, /*1.0f, 0.0f, 0.0f,*/ 1.0f, 0.0f,// 15 - 7

	// BACK face - blue color
	-0.5f,  0.5f, -0.5f, /*0.0f, 0.0f, 1.0f,*/ 1.0f, 1.0f,// 16 - 0
	 0.5f,  0.5f, -0.5f, /*0.0f, 0.0f, 1.0f,*/ 0.0f, 1.0f,// 17 - 1
	-0.5f, -0.5f, -0.5f, /*0.0f, 0.0f, 1.0f,*/ 1.0f, 0.0f,// 18 - 4
	 0.5f, -0.5f, -0.5f, /*0.0f, 0.0f, 1.0f,*/ 0.0f, 0.0f,// 19 - 7

	// DOWN face - yellow color
	-0.5f, -0.5f, -0.5f, /*1.0f, 1.0f, 0.0f,*/ 0.0f, 1.0f,// 20 - 4
	 0.5f, -0.5f, -0.5f, /*1.0f, 1.0f, 0.0f,*/ 1.0f, 1.0f,// 21 - 7
	-0.5f, -0.5f,  0.5f, /*1.0f, 1.0f, 0.0f,*/ 0.0f, 0.0f,// 22 - 5
	 0.5f, -0.5f,  0.5f, /*1.0f, 1.0f, 0.0f,*/ 1.0f, 0.0f // 23 - 6
};



static GLuint cube_indices[] =
{
	// UP face
	// Triangles
	0, 1, 3,
	0, 2, 3,
	// offset: 6

	// LEFT face
	// Triangles
	4, 5, 6,
	5, 6, 7,
	// offset: 12

	// FRONT face
	// Triangles
	8, 9, 11,
	8, 10, 11,
	// offset: 18

	// RIGHT face
	// Triangles
	12, 13, 15,
	12, 14, 15,
	// offset: 24

	// BACK face
	// Triangles
	16, 17, 18,
	17, 19, 18,
	// offset: 30

	// DOWN face
	// Triangles
	20, 21, 22,
	21, 23, 22,
	// offset: 36

	// LineLoop
	// UP face
	0, 1, 3, 2,
	// offset: 40

	// LEFT face
	4, 6, 7, 5,
	// offset: 44

	// FRONT face
	8, 9, 11, 10,
	// offset: 48

	// RIGHT face
	12, 13, 15, 14,
	// offset: 52

	// BACK face
	16, 17, 19, 18,
	// offset: 56

	// DOWN face
	20, 21, 23, 22
	// offset: 60

};



class Cube
{
private:
	static const int NFACES = 6;
	void chooseColor(const Shader& program, GLint i);
public:
	char colors[NFACES];

	glm::vec3 pos;
	glm::mat4 model;
	Cube();
	Cube(const Cube& another_cube);
	Cube(glm::mat4 model_, glm::vec3 pos_);
	void draw(const Shader& program);
	~Cube();
};

class RubikCube3x3
{
public:
	enum class STATE_ANIMATION { NONE, F, f, U, u, L, l, R, r, B, b, D, d, SOLVE };
	enum class PAINT_MODE { BREATHE, STATIC };
	Shader program;
private:
	static const int NCUBES = 27;
	static const int MAX_N_TEXTURES = 2;
	// cada cubo de rubik tiene estas variables
	// imgPaths[0] = whiteFace.jpg, imgPaths[1] = orangeFace.jpg, imgPaths[2] = greenFace.jpg
	// imgPaths[3] = redFace.jpg, imgPaths[4] = blueFace.jpg, imgPaths[5] = yellowFace.jpg
	std::vector<GLuint> textureUnits;

	glm::mat4 global_model;
	glm::mat4 inverse_global_model;

	GLuint RVAO; // associated VAO
	// �ndice de inicio 0, omitimos el cubo con �ndice 13, ya que es el centro
	Cube cubes[NCUBES]; // 9(cuadrante) * 3(nivel) = 27 cubos en total
	STATE_ANIMATION state_animation = STATE_ANIMATION::NONE;

	std::string cubeString;
	solver::Rubik rubikSolver;
	std::vector<char> solution;
	std::queue<STATE_ANIMATION> solutionStates;

	float angle = 0.0f, step = 0.0f; // estas variables se usan al momento de realizar los movimientos del cubo


	void HandleRubikMoves(char movement);
	// orientation, h: horario, a: antihorario
	void CalculateRotation(float& parts, glm::vec3& axis, glm::mat4& rotationMatrix, char orientation = 'h');


	struct AnimationsHandler {
		glm::vec3 init_pos[NCUBES];
		float scalar, domain;
		AnimationsHandler() {
			domain = -glm::pi<float>();
			scalar = cosf(domain) * 0.25f + 1.25f;
			for (int i = 0; i < NCUBES; ++i) init_pos[i] = glm::vec3(0.0f);
		}
	} animator;
public:
	// Constructores y dem�s funciones para la configuraci�n
	RubikCube3x3(const Shader& program_);
	void AssociateVAO(GLuint VAO);

	// Funciones del cubo
	void ApplyTransformation(glm::mat4 glob_trans);
	void DrawCube(glm::mat4& view, glm::mat4& projection);
	void HandleDrawing(glm::mat4& view, glm::mat4& projection, STATE_ANIMATION& move_state, PAINT_MODE& animation);
	void Solve(STATE_ANIMATION& some_state);

	// MOVIMIENTOS DEL CUBO
	//// rota el FRONT del cubo en sentido horario
	bool F(float parts);
	//// rota el FRONT del cubo en sentido antihorario
	bool f(float parts);
	//// rota el RIGHT del cubo en sentido horario
	bool R(float parts);
	//// rota el RIGHT del cubo en sentido antihorario
	bool r(float parts);
	//// rota el UP del cubo en sentido horario
	bool U(float parts);
	//// rota el UP del cubo en sentido antihorario
	bool u(float parts);
	//// rota el BACK del cubo en sentido horario
	bool B(float parts);
	//// rota el BACK del cubo en sentido antihorario
	bool b(float parts);
	//// rota el LEFT del cubo en sentido horario
	bool L(float parts);
	//// rota el LEFT del cubo en sentido antihorario
	bool l(float parts);
	//// rota el DOWN del cubo en sentido horario
	bool RotateDownACW(float parts);
	//// rota el DOWN del cubo en sentido antihorario 
	bool d(float parts);
};


#endif // !RUBIK_CUBE_H