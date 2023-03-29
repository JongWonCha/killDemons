//#pragma comment(lib,"winmm.lib")
#include "stdafx.h"
#include <iostream>
#include <Windows.h>
#include "Dependencies\glew.h"
#include "Dependencies\freeglut.h"
#include <math.h>
#include <chrono>
#include "ScnMgr.h"
#define FORCE 10.f
#define ZFORCE 100.0f

using namespace std;



ScnMgr *g_ScnMgr = NULL;
DWORD g_PrevTime = 0;
BOOL g_KeyW = FALSE;
BOOL g_KeyA = FALSE;
BOOL g_KeyD = FALSE;
BOOL g_KeyS = FALSE;
BOOL g_KeySP = FALSE;

extern int g_Shoot = SHOOT_NONE;
extern int g_Score = 0;

float g_AttackPerSec = 5.f;
DWORD g_AttackTime = 0;

float g_SpawnSec = 3.f;
DWORD g_SpawnTime = 0;

extern short g_InGame = GAME_START;


void RenderScene(void)
{
	if (g_PrevTime == 0)
	{
		g_PrevTime = GetTickCount();
		return;
	}
	DWORD CurrTime = GetTickCount();//밀리세컨드
	DWORD ElapsedTime = CurrTime - g_PrevTime;
	g_AttackTime += ElapsedTime;
	g_SpawnTime += ElapsedTime;
	g_PrevTime = CurrTime;
	float eTime = (float)ElapsedTime / 1000.0f;

	float xpos, ypos;



	float forceX = 0.f;
	float forceY = 0.f;
	float forceZ = 0.f;
	if (g_KeyW)
	{
		forceY += FORCE;
	}
	if (g_KeyS)
	{
		forceY -= FORCE;
	}
	if (g_KeyA)
	{
		forceX -= FORCE;
	}
	if (g_KeyD)
	{
		forceX += FORCE;
	}
	if (g_KeySP)
	{
		forceZ += ZFORCE;
	}
	if(g_InGame == GAME_START)
		g_ScnMgr->ApplyForce(forceX, forceY, forceZ, eTime);

	g_ScnMgr->Update(eTime);

	g_ScnMgr->RenderScene(eTime);
	
	
	if (g_Shoot != SHOOT_NONE)
	{
		if ((float)g_AttackTime / 1000.f >= 1.f / g_AttackPerSec)
		{
			g_AttackTime = 0;
			g_ScnMgr->Shoot(g_Shoot);
		}
	}
	do
	{
		xpos = (rand() % 3) - 1;
		ypos = (rand() % 3) - 1;
	} while (xpos == 0 && ypos == 0);

	if ((float)g_SpawnTime / 1000.f >= g_SpawnSec && g_InGame == GAME_START)
	{
		g_ScnMgr->AddObject(xpos * 6.f, ypos * 4.f, 0, 1, 1, 1, 0, 0, 0, KIND_ENEMY, (g_Score / 10 + 1) * 1000, STATE_GROUND);
		g_SpawnTime = 0;
		g_SpawnSec = (g_Score / 15 + 1) / 2.f;
	}
	g_ScnMgr->GarbageCollector();

	if(g_InGame == GAME_START)
		g_ScnMgr->DoCollisionTest();

	g_ScnMgr->WallCollision();

	glutSwapBuffers();
}

void Idle(void)
{
	RenderScene();
}

void MouseInput(int button, int state, int x, int y)
{
	//RenderScene();
}


void KeyDownInput(unsigned char key, int x, int y)
{
	if (key == 'w')
	{
		g_KeyW = TRUE;
	}
	else if (key == 's')
	{
		g_KeyS = TRUE;
	}
	else if (key == 'a')
	{
		g_KeyA = TRUE;
	}
	else if (key == 'd')
	{
		g_KeyD = TRUE;
	}
	else if (key == ' ')
	{
		g_KeySP = TRUE;
	}
}
void KeyUpInput(unsigned char key, int x, int y)
{
	if (key == 'w')
	{
		g_KeyW = FALSE;
	}
	else if (key == 's')
	{
		g_KeyS = FALSE;
	}
	else if (key == 'a')
	{
		g_KeyA = FALSE;
	}
	else if (key == 'd')
	{
		g_KeyD = FALSE;
	}
	else if (key == ' ')
	{
		g_KeySP = FALSE;
	}
	if (g_InGame == GAME_OVER || g_InGame == GAME_CLEAR)
	{
		if(key == 'r' || key == 'R')
			g_ScnMgr->Init();
	}
}
void SpecialKeyDownInput(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_UP:
		g_Shoot = SHOOT_UP;
		break;
	case GLUT_KEY_DOWN:
		g_Shoot = SHOOT_DOWN;
		break;
	case GLUT_KEY_LEFT:
		g_Shoot = SHOOT_LEFT;
		break;
	case GLUT_KEY_RIGHT:
		g_Shoot = SHOOT_RIGHT;
		break;
	}
}

void SpecialKeyUpInput(int key, int x, int y)
{
	g_Shoot = SHOOT_NONE;
}


int main(int argc, char **argv)
{
	// Initialize GL things
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(1200, 800);
	glutCreateWindow("Game Software Engineering KPU");

	glewInit();
	if (glewIsSupported("GL_VERSION_3_0"))
	{
		std::cout << " GLEW Version is 3.0\n ";
	}
	else
	{
		std::cout << "GLEW 3.0 not supported\n ";
	}

	glutDisplayFunc(RenderScene);
	glutIdleFunc(Idle);
	glutKeyboardFunc(KeyDownInput);
	glutKeyboardUpFunc(KeyUpInput);
	glutMouseFunc(MouseInput);

	glutSpecialFunc(SpecialKeyDownInput);
	glutSpecialUpFunc(SpecialKeyUpInput);

	glutSetKeyRepeat(GLUT_KEY_REPEAT_OFF);

	g_ScnMgr = new ScnMgr();

	glutMainLoop();//메인 루프 함수

	delete g_ScnMgr;

    return 0;
}

