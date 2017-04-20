#include <iostream>
#include <stdio.h>
#include "GL\glut.h"
#include "PierAstar.h"
#include "Color3.h"
#include "Vector2.h"

#define PI 3.14159265359
#define inc 0.003

enum PacManRot
{
	Up,
	Down,
	Left,
	Right
};

////////////
// Variables
////////////

// Window resolution
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 800

// Grid size
#define	 GRID_WIDTH 21
#define  GRID_HEIGHT 21

// Colors
Color3 pacManColor = Color3(1.0f, 1.0f, 0.0f);
Color3 henryColor = Color3(0.0f, 1.0f, 0.0f);
Color3 clydeColor = Color3(1.0f, 0.0f, 0.0f);
Color3 pinkyColor = Color3(1.0f, 0.2f, 0.5f);
Color3 wallColor = Color3(40.0f / 255.0f, 30.0f / 255.0f, 233.0f / 255.0f);

// Pacman Details
static float pacRadius = 0.5f;
static float mouthSize = 0.6;

////////////////////
// Runtime Variables
////////////////////

float translate_valuex = 1;
float translate_valuey = 1;
PacManRot pacManDirection = Right;
bool gameOver = false;

Point  * path;
astarSearch searchPathfinding;
float score = 0;

//////////////
// Ghost Class
//////////////

class Ghost 
{
public:
	Vector2 position;
	float r, g, b = 1;
	Point* myPath;
	Point* current = NULL;
	astarSearch mySearchPathfinding;

	float timer = 0;

	void SetColor3(Color3 newColor)
	{
		r = newColor.r;
		g = newColor.g;
		b = newColor.b;
	}

	void Render()
	{
		glPushMatrix();
		glTranslatef(0.5f, 0.5f, 0.0f);
		glTranslatef(position.x, position.y, 0.0f); //10s are temp vallues

		glColor3f(r, g, b);
		glutSolidSphere(0.2f, 10.0f, 6.0f);

		glPopMatrix();

		//cout << current->x << " " << current->y << endl;

		if (current != NULL) 
		{
			glPushMatrix();
			glTranslatef(current->x + 0.5f, current->y + 0.5f, -1.0f);
			glColor3f(1, 0, 0);
			//glutSolidCube(.3);
			glPopMatrix();
		}
	}

	void Update()
	{	
		timer += 0.1;

		if (timer > 10) 
		{
			Point start;
			Point Goal;
			if (current == NULL) 
			{
				start.x = position.x;
				start.y = position.y;
			}
			else 
			{
				start.x = current->x;
				start.y = current->y;
			}

			Goal.x = translate_valuex;
			Goal.y = translate_valuey;

			if (!(start == Goal))
			{
				myPath = mySearchPathfinding.astar(&start, &Goal);
				current = myPath;
				while (true)
				{
					if (!(*current->parent == start))
					{
						current = current->parent;
					}
					else 
					{
						break;
					}
				}
			}

			timer = 0;
		}

		if (current!= NULL)
		{
			if (position.x < current->x)
			{
				//cout << x << " " << current->x << endl;
				position.x += 0.01;
			}
			else 
			{
				position.x -= 0.01;
			}
			if (position.y < current->y)
			{
				position.y += 0.01;
			}
			else 
			{
				position.y -= 0.01;
			}
		}
	}
};

Ghost pinky;
Ghost clyde;
Ghost henry;

////////////////////
// Drawing Functions
////////////////////

// Text
void DrawText(int x, int y, char *st)
{
	int l, i;

	l = strlen(st);
	glRasterPos3i(x, y, -1);
	for (i = 0; i < l; i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, st[i]);

	}
}

// PacMan
void DrawPac() 
{
	//static int slices = 20;
	glPushMatrix();
	glTranslatef(.5, .5, 0);
	glTranslatef(translate_valuex, translate_valuey, 0);
	glColor3f(pacManColor.r, pacManColor.g, pacManColor.b);
	//glutSolidSphere(0.2, slices, 6);

	// Rotation
	float spin = 0;
	switch (pacManDirection)
	{
	case Up:
		spin = 270;
		break;

	case Down:
		spin = 90;
		break;

	case Left:
		spin = 180;
		break;

	case Right:
		spin = 0;
		break;
	}

	glRotatef(spin, 0, 0, 1.0);

	// Body
	float theta;
	glBegin(GL_POLYGON);
	for (theta = 0.0; theta <= 2 * PI; theta += inc)
	{
		glColor4f(1, 1, 0.3, 0);
		glVertex2f(pacRadius*cos(theta), pacRadius*sin(theta));
	}
	glEnd();

	// Mouth
	glBegin(GL_POLYGON);
	glColor4f(0, 0, 0, 0);
	glVertex2f(0, 0);
	for (theta = -mouthSize; theta <= mouthSize; theta += inc)
	{
		glColor4f(0, 0, 0, 0);
		glVertex2f((pacRadius)*cos(theta), (pacRadius)*sin(theta));
	}
	glEnd();

	glFlush();

	glPopMatrix();
}

// PacMan to pos (Debugging)
void DrawPath()
{
	Point start;
	start.x = 10;
	start.y = 10;
	Point Goal;
	Goal.x = translate_valuex;
	Goal.y = translate_valuey;
	path = searchPathfinding.astar(&start, &Goal);
	Point * current = path;
	int i = 0;
	while (true)
	{
		glPushMatrix();
		glTranslatef(current->x + .5, current->y + .5, 0);
		glColor3f(1, 1, 1);
		glutSolidCube(.1);

		glPopMatrix();
		//cout << "path " << i << " :" << current->x << "," << current->y << endl;
		if (current->parent != NULL)
		{
			current = current->parent;
		}
		else
		{
			break;
		}
		i++;
	}
}

// Board
void DrawBoard() 
{
	for (int i = 0; i < GRID_HEIGHT; i++)
	{
		for (int k = 0; k < GRID_WIDTH; k++)
		{
			if (board_array[i][k] == 'X')
			{
				glPushMatrix();
				glTranslatef(k + .5, i + .5, 0);
				glColor3f(wallColor.r, wallColor.g, wallColor.b);
				glutSolidCube(1);

				glPopMatrix();
			}

		}

	}
	DrawPac();
	//Render();
	//DrawPath();
}

// Display function
void Display(void)
{

	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();
	gluLookAt
	(
		0, 0, 5,
		0, 0, 0,
		0, 1, 0
	);

	if (gameOver == false)
	{
		DrawBoard();
	
		pinky.Render();
		clyde.Render();
		henry.Render();
	}
	else 
	{
		glColor3f(1.0f, 1.0f, 1.0f);
		glLoadIdentity();
		DrawText(8, 10, "GAME OVER");
	}

	glPushMatrix();
	glColor3f(0.0f, 0.0f, 0.0f);

	static char scoreText[16];
	sprintf_s(scoreText, "Score:%d", (int)score);
	glColor3f(1.0f, 1.0f, 1.0f);
	DrawText(1, 1, scoreText);
	glPopMatrix();
	glutSwapBuffers();
}

/////////////
// Collisions
/////////////

bool CheckCollision(Ghost _ghost)
{
	//std::cout << (int)_ghost.x <<"you died"<<translate_valuex << std::endl;
	if ((int)_ghost.position.x == translate_valuex && (int)_ghost.position.y == translate_valuey ) 
	{
		std::cout << "You died" << std::endl;
		return  true;
	}
	return false;
}

///////
// Idle
///////

void Idle()
{
	score += 0.01;

	if (gameOver == false)
	{
		pinky.Update();
		henry.Update();
		clyde.Update();
		if (CheckCollision(pinky) || CheckCollision(henry) || CheckCollision(clyde))
		{
			gameOver = true;
		}

	}	
	glutPostRedisplay();
}

/////////////////
// Initialization
/////////////////

void InitGhost()
{
	pinky.SetColor3(pinkyColor);
	pinky.position = Vector2(17, 18);

	henry.SetColor3(henryColor);
	henry.position = Vector2(1, 18);

	clyde.SetColor3(clydeColor);
	clyde.position = Vector2(18, 2);
}

void Init(void)
{
	InitGhost();

	// Select clearing (background) color
	glClearColor(0.0, 0.0, 0.0, 0.0);

	// Initialize viewing value
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, GRID_WIDTH, GRID_HEIGHT, 0, 0.1, 100.0);
}

////////////////
// Input Manager
////////////////

void KeyboardHandler(unsigned char key, int x, int y)
{
	if (gameOver == false)
	{
		if (key == 'a')
		{
			if (translate_valuex == 0)
			{
				translate_valuex = 20;
			}
			else if (board_array[(int)translate_valuey][(int)translate_valuex - 1] == ' ')
			{
				translate_valuex -= 1;
				pacManDirection = Left;
			}
			
		}
		else if (key == 'd')
		{
			if (translate_valuex == 20)
			{
				translate_valuex = 0;
			}
			else if (board_array[(int)translate_valuey][(int)translate_valuex + 1] == ' ')
			{
				translate_valuex += 1;
				pacManDirection = Right;
			}
		
		}
		else if (key == 's')
		{
			if (board_array[(int)translate_valuey + 1][(int)translate_valuex] == ' ')
			{
				translate_valuey += 1;
				pacManDirection = Down;
			}
			
		}


		else if (key == 'w')
		{
			if (board_array[(int)translate_valuey - 1][(int)translate_valuex] == ' ')
			{
				translate_valuey -= 1;
				pacManDirection = Up;
			}
			
		}
	}

	cout << "X: " << translate_valuex << " Y: " << translate_valuey << endl;

	glutPostRedisplay();
}

///////
// Main
///////

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Pacman by Edisson Flores");
	Init();
	glutDisplayFunc(Display);
	glutIdleFunc(Idle);
	glutKeyboardUpFunc(KeyboardHandler);

	glutMainLoop();
	return 0;
}
