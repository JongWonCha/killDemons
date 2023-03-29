#include "stdafx.h"
#include "ScnMgr.h"
#include <math.h>

float g_Seq = 0;
float g_Time = 0.f;
extern int g_Shoot;
int Shoot_Dir = 3;
extern int g_Score;
extern short g_InGame;


ScnMgr::ScnMgr()
{


	m_Renderer = NULL;


	m_Renderer = new Renderer(1200, 800);
	m_Sound = new Sound();
	if (!m_Renderer->IsInitialized())
	{
		std::cout << "Renderer could not be initialized.. \n";
	}

	m_HeroTexture = m_Renderer->CreatePngTexture("./textures/cha_ani.png");
	m_TestTexture = m_Renderer->CreatePngTexture("./textures/texture.png");
	m_TexSeq = m_Renderer->CreatePngTexture("./textures/texture.png");
	m_SmokeTexture = m_Renderer->CreatePngTexture("./textures/smoke.png");
	m_EnemyTexture = m_Renderer->CreatePngTexture("./textures/enemy.png");
	m_NumberTexture = m_Renderer->CreatePngTexture("./textures/number.png");
	m_BGTexture = m_Renderer->CreatePngTexture("./textures/bg.png");
	m_CongTexture = m_Renderer->CreatePngTexture("./textures/cong_r.png");
	m_EnemyEndTexture = m_Renderer->CreatePngTexture("./textures/enemy_end.png");
	m_HeroEndTexture = m_Renderer->CreatePngTexture("./textures/hero_end.png");
	m_GameOverTexture = m_Renderer->CreatePngTexture("./textures/gameover.png");
	m_KillTexture = m_Renderer->CreatePngTexture("./textures/kill.png");
	m_RestartTexture = m_Renderer->CreatePngTexture("./textures/restart.png");

	m_Gun = m_Sound->CreateSound("./Dependencies/SoundSamples/Shot.mp3");
	m_BG = m_Sound->CreateSound("./Dependencies/SoundSamples/Devil N Bass.mp3");
	m_Explode = m_Sound->CreateSound("./Dependencies/SoundSamples/Explod.wav");
	m_hit =  m_Sound->CreateSound("./Dependencies/SoundSamples/hit.mp3");
	m_Sound->PlaySound(m_BG, true, 0.2f);
	for (int i = 0; i < MAX_OBJECTS; i++)
	{
		m_Objects[i] = NULL;
	}
	//Create Hero Object
	m_Objects[HERO_ID] = new Object();
	m_Objects[HERO_ID]->SetPos(0.f, 0.f, 0.f);
	m_Objects[HERO_ID]->SetSize(1.f, 1.f, 1.f);
	m_Objects[HERO_ID]->SetColor(1, 1, 1, 1);
	m_Objects[HERO_ID]->SetVel(0.f, 0.f, 0.f);
	m_Objects[HERO_ID]->SetAcc(0.f, 0.f, 0.f);
	m_Objects[HERO_ID]->SetMass(1.f);
	m_Objects[HERO_ID]->SetCoefFrict(0.7f);
	m_Objects[HERO_ID]->SetKind(KIND_HERO);
	m_Objects[HERO_ID]->SetHP(10000);
	m_Objects[HERO_ID]->SetState(STATE_GROUND);
	m_Objects[HERO_ID]->SetInvin(0.f);

	//AddObject(4.f, 3.f, 1.f, 1.f, 1.f, 1.f, 0.f, 0.f, 0.f, KIND_ENEMY, 2000, STATE_GROUND);
}

ScnMgr::~ScnMgr()
{
	if (m_Renderer)
	{
		delete m_Renderer;
		m_Renderer = NULL;
	}
	if (m_Objects[HERO_ID])
	{
		delete m_Objects[HERO_ID];
		m_Objects[HERO_ID] = NULL;
	}
}

bool ScnMgr::RRCollision(float minX, float minY, float maxX, float maxY,
float minX1, float minY1, float maxX1, float maxY1)
{
	if (maxX < minX1)
		return false;
	if (maxX1 < minX)
		return false;
	if (maxY < minY1)
		return false;
	if (maxY1 < minY)
		return false;
	return true;
}

bool ScnMgr::BBCollision(float minX, float minY, float minZ, float maxX, float maxY, float maxZ,
	float minX1, float minY1, float minZ1, float maxX1, float maxY1, float maxZ1)
{
	if (maxX < minX1)
		return false;
	if (maxX1 < minX)
		return false;
	if (maxY < minY1)
		return false;
	if (maxY1 < minY)
		return false;
	if (maxZ < minZ1)
		return false;
	if (maxZ1 < minZ)
		return false;
	return true;
}




void ScnMgr::RenderScene(float eTime)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.3f, 0.3f, 1.0f);
	float quake_x, quake_y;
	float invin = 0;
	float temp;
	int hp;

	m_Objects[HERO_ID]->GetInvin(&invin);

	if (invin > 0.f)
	{
		m_Objects[HERO_ID]->CalInvin(eTime * 10.f);
		m_Objects[HERO_ID]->GetInvin(&invin);
	}
	if (invin < 0.f)
	{
		m_Objects[HERO_ID]->SetInvin(0.f);
		m_Objects[HERO_ID]->GetInvin(&invin);
	}
	quake_x = rand() % 100 - 50;
	quake_y = rand() % 100 - 50;
	temp = sqrtf(quake_x * quake_x + quake_y * quake_y);
	quake_x /= temp;
	quake_y /= temp;
	quake_x *= invin * 5.f;
	quake_y *= invin * 5.f;

	//std::cout << g_InGame << std::endl;

	m_Renderer->DrawTextureRectDepth(quake_x, quake_y, 0, 1200.f, 800.f, 1, 1, 1, 1, m_BGTexture, 1.f);
	if (g_Score < 10)
	{
		m_Renderer->DrawTextureRectSeqXYHeight(quake_x, quake_y, 0, 50, 50, 1, 1, 1, 1, m_NumberTexture, g_Score, 0, 10, 3, 0);
		m_Renderer->DrawTextureRectDepth(quake_x, quake_y - 60, 0, 600.f, 300.f, 1, 1, 1, 1, m_KillTexture, 1.f);
	}
	else if (10 <= g_Score && g_Score < 60)
	{
		m_Renderer->DrawTextureRectSeqXYHeight(50.f + quake_x, quake_y, 0, 50, 50, 1, 1, 1, 1, m_NumberTexture, g_Score % 10, 0, 10, 3, 0);
		m_Renderer->DrawTextureRectSeqXYHeight(-50.f + quake_x, quake_y, 0, 50, 50, 1, 1, 1, 1, m_NumberTexture, g_Score / 10, 0, 10, 3, 0);
	}
	else if (g_Score >= 60)
		g_InGame = GAME_CLEAR;
	m_Objects[HERO_ID]->GetHP(&hp);
	if (hp <= 0)
		g_InGame = GAME_OVER;
	if (g_InGame != GAME_CLEAR)
	{
		for (int i = -1; i < 2; i++)
		{
			for (int j = -1; j < 2; j++)
			{
				if (i != 0 || j != 0)
					m_Renderer->DrawParticle(j * 5.5f * 100.f + quake_x, i * 3.5f * 100.f + quake_y, 0, 50.f, 1, 1, 1, 1, i * 0.1f, j * 0.1f, m_SmokeTexture, g_Time);
			}
		}
	}
	else if (g_InGame == GAME_CLEAR)
	{
		m_Renderer->DrawTextureRect(0, 100, 0, 500, 80, 1, 1, 1, 1, m_CongTexture);
	}
	if(g_InGame != GAME_START)
		m_Renderer->DrawTextureRectDepth(quake_x, quake_y - 90, 0, 600.f, 300.f, 1, 1, 1, 1, m_RestartTexture, 0.f);

	for (int i = 0; i < MAX_OBJECTS; i++)
	{
		if (m_Objects[i] != NULL)
		{
			float x, y, z, sizeX, sizeY, sizeZ;
			float r, g, b, a;
			int hp;
			int kind;
			float vel_x, vel_y;
			int Walk_Dir = 0;
			m_Objects[i]->GetPos(&x, &y, &z);
			m_Objects[i]->GetSize(&sizeX, &sizeY, &sizeZ);
			m_Objects[i]->GetColor(&r, &g, &b, &a);
			m_Objects[i]->GetHP(&hp);
			m_Objects[i]->GetKind(&kind);
			m_Objects[i]->GetXvel(&vel_x);
			m_Objects[i]->GetYvel(&vel_y);
			







			
			if (g_Shoot != SHOOT_NONE)
				Shoot_Dir = g_Shoot;

			//g_Seq = (int)g_Seq % 600;

			g_Seq += eTime * 1000.f;
			if (g_Seq > 1200)
				g_Seq = 0;

			if (abs(vel_x) >= abs(vel_y))
			{
				if (vel_x < 0)
					Walk_Dir = 1;
				else
					Walk_Dir = 2;
			}
			else
			{
				if (vel_y < 0)
					Walk_Dir = 0;
				else
					Walk_Dir = 3;
			}

			// Renderer Test
			//m_Renderer->DrawSolidRect(x * 2.f, y * 2.f, 0, sizeX,sizeY, 1, 0, 1, 1);//미터로 바꾸기 위해서 x,y 에 100 씩 곱한다.
			//m_Renderer->DrawTextureRect(x * 2.f, y * 2.f, 0, sizeX, sizeY, 1, 1, 1, 1, m_HeroTexture);
			

			if (kind == KIND_HERO)
			{
				if(g_InGame == GAME_START || g_InGame == GAME_CLEAR)
					m_Renderer->DrawTextureRectSeqXYHeight(x * 100.f + quake_x, y * 100.f + quake_y, 1.f, sizeX * 100.f, sizeY * 100.f, r, g, b, a, m_HeroTexture, (int)(g_Seq / 400), Shoot_Dir, 3, 4, z * 100.f);
				else if (g_InGame == GAME_OVER)
				{
					m_Renderer->DrawTextureRectSeqXYHeight(x * 100.f + quake_x, y * 100.f + quake_y, 1.f, sizeX * 100.f, sizeY * 100.f, r, g, b, a, m_HeroEndTexture, (int)(g_Seq / 400), 0, 3, 1, z * 100.f);
					m_Renderer->DrawTextureRectSeqXYHeight(0,100, 1.f, 600, 300, 1, 1, 1, 1, m_GameOverTexture, 1, 1, 1, 1, 0);
				}
			}
			else if (kind == KIND_ENEMY)
			{
				if(g_InGame == GAME_START || g_InGame == GAME_OVER)
					m_Renderer->DrawTextureRectSeqXYHeight(x * 100.f + quake_x, y * 100.f + quake_y, 1.f, sizeX * 100.f, sizeY * 100.f, r, 0, 0, a, m_EnemyTexture, (int)(g_Seq / 400), Walk_Dir, 3, 4, z * 100.f);
				else if(g_InGame == GAME_CLEAR)
					m_Renderer->DrawTextureRectSeqXYHeight(x * 100.f + quake_x, y * 100.f + quake_y, 1.f, sizeX * 100.f, sizeY * 100.f, r, 0, 0, a, m_EnemyEndTexture, (int)(g_Seq / 400), 0, 3, 1, z * 100.f);
			}
				
				//m_Renderer->DrawTextureRectHeight(x * 100.f, y * 100.f, z * 100.f, sizeX * 100.f, sizeY * 100.f, r, g, b, a, m_EnemyTexture, z * 100.f);
			else if(kind == KIND_BULLET)
				m_Renderer->DrawTextureRectHeight(x * 100.f + quake_x, y * 100.f + quake_y, z * 100.f, sizeX * 100.f, sizeY * 100.f, r, g, b, a, m_TestTexture, z * 100.f);

			if (kind != KIND_BULLET)
			{
				if(kind == KIND_HERO)
					m_Renderer->DrawSolidRectGauge(x * 100.f + quake_x, y * 100.f + 100.f + quake_y, z * 100.f, sizeX * 100.f, sizeY * 10.f, 1, 0, 0, 1, m_TestTexture, (float)hp / (float)10000);
				else
					m_Renderer->DrawSolidRectGauge(x * 100.f + quake_x, y * 100.f + 100.f + quake_y, z * 100.f, sizeX * 100.f, sizeY * 10.f, 1, 0, 0, 1, m_TestTexture, (float)hp / (float)6000);
			}
			//m_Renderer->DrawTextureRectSeqXY(x * 100.f, y * 100.f, 1.f, sizeX * 100.f, sizeY * 100.f, r, g, b, a, m_TexSeq, seqX, seqY, 8, 2);
		}
	}
	
	//m_Renderer->DrawParticleClimate(0, 0, 0, 5.f, 1, 1, 1, 1, 0.f, -1.0f, m_HeroTexture, 0.2f, g_Time);
	//m_Renderer->DrawParticle(0, 0, 0, 30.f, 1, 1, 1, 1, 0.1f, 0.1f, m_SmokeTexture, g_Time);
	

	g_Time += eTime;
}

void ScnMgr::Update(float eTime)
{
	for (int i = 0; i < MAX_OBJECTS; i++)
	{
		if(m_Objects[i] != NULL)
			m_Objects[i]->Update(eTime);
	}
}

void ScnMgr::DoCollisionTest()
{
	for (int i = 0; i < MAX_OBJECTS; i++)
	{
		if (m_Objects[i] == NULL)
			continue;

		int collisionCount = 0;

		for (int j = i + 1;j < MAX_OBJECTS; j++)// 중복이 되지 않게 i + 1로 바꿈.
		{
			if (m_Objects[j] == NULL)
				continue;
			if (i == j)
				continue;

			float pX, pY, pZ;
			float sX, sY, sZ;
			float minX, minY, maxX, maxY, minZ, maxZ;
			m_Objects[i]->GetPos(&pX, &pY, &pZ);
			m_Objects[i]->GetSize(&sX, &sY, &sZ);
			minX = pX - sX / 2.f;
			maxX = pX + sX / 2.f;
			minY = pY - sY / 2.f;
			maxY = pY + sY / 2.f;
			minZ = pZ - sZ / 2.f;
			maxZ = pZ + sZ / 2.f;

			float pX1, pY1, pZ1;
			float sX1, sY1, sZ1;
			float minX1, minY1, maxX1, maxY1, minZ1, maxZ1;
			m_Objects[j]->GetPos(&pX1, &pY1, &pZ1);
			m_Objects[j]->GetSize(&sX1, &sY1, &sZ1);
			minX1 = pX1 - sX1 / 2.f;
			maxX1 = pX1 + sX1 / 2.f;
			minY1 = pY1 - sY1 / 2.f;
			maxY1 = pY1 + sY1 / 2.f;
			minZ1 = pZ1 - sZ1 / 2.f;
			maxZ1 = pZ1 + sZ1 / 2.f;

			if (BBCollision(minX, minY,minZ, maxX, maxY, maxZ,
				minX1, minY1, minZ1, maxX1, maxY1, maxZ1))
			{
				//std::cout << "Collision\n";
				collisionCount++;
				ProcessCollision(i, j);
			}
		}
		if (collisionCount > 0)
		{
			m_Objects[i]->SetColor(1, 0, 0, 1);
		}
		else
		{
			m_Objects[i]->SetColor(1, 1, 1, 1);
		}
	}
}

void ScnMgr::ProcessCollision(int i, int j)
{
	if (m_Objects[i] == NULL || m_Objects[j] == NULL)
		return;

	Object *ob1 = m_Objects[i];
	Object *ob2 = m_Objects[j];
	int kind1, kind2;
	float bul_x, bul_y, ene_x, ene_y;
	float temp;
	float x, y;
	ob1->GetKind(&kind1);
	ob2->GetKind(&kind2);
	if (kind1 == KIND_ENEMY && kind2 == KIND_BULLET)
	{
		int hp1, hp2;
		ob1->GetHP(&hp1);	// 빌딩
		ob2->GetHP(&hp2);	// 총알

		hp1 = hp1 - hp2;	
		hp2 = 0;

		ob1->SetHP(hp1);
		ob2->SetHP(hp2);
		ob1->GetXpos(&ene_x);
		ob1->GetYpos(&ene_y);
		ob2->GetXpos(&bul_x);
		ob2->GetYpos(&bul_y);
		x = ene_x - bul_x;
		y = ene_y - bul_y;
		temp = sqrtf(x * x + y * y);
		ob1->ApplyForce(x / temp * 200, y / temp * 200, 0.f, 1.f / 60.f);

		m_Sound->PlaySound(m_Explode, false, 0.5f);
	}
	else if (kind1 == KIND_BULLET && kind2 == KIND_ENEMY)
	{
		int hp1, hp2;
		float bul_x, bul_y, ene_x, ene_y;
		float temp;
		float x, y;
		ob1->GetHP(&hp1);	// 총알
		ob2->GetHP(&hp2);	// 빌딩

		hp2 = hp2 - hp1;
		hp1 = 0;

		ob1->SetHP(hp1);
		ob2->SetHP(hp2);
		ob1->GetXpos(&bul_x);
		ob1->GetYpos(&bul_y);
		ob2->GetXpos(&ene_x);
		ob2->GetYpos(&ene_y);
		x = ene_x - bul_x;
		y = ene_y - bul_y;
		temp = sqrtf(x * x + y * y);
		ob2->ApplyForce(x / temp * 200, y / temp * 200, 0.f, 1.f / 60.f);
		m_Sound->PlaySound(m_Explode, false, 0.5f);
	}
	if (kind1 == KIND_HERO && kind2 == KIND_ENEMY )
	{
		float invin;
		int hp1, hp2;
		ob1->GetInvin(&invin);
		if (invin <= 0.f)
		{
			ob1->GetHP(&hp1);
			ob2->GetHP(&hp2);

			ob1->SetHP(hp1 - 1000);
			ob2->SetHP(0);
			ob1->SetInvin(5.f);
			m_Sound->PlaySound(m_hit, false, 0.5);
		}
		

	}
	else if (kind1 == KIND_ENEMY && kind2 == KIND_HERO)
	{
		float invin;
		int hp1, hp2;
		ob2->GetInvin(&invin);
		if (invin <= 0.f)
		{
			ob1->GetHP(&hp1);
			ob2->GetHP(&hp2);

			ob2->SetHP(hp2 - 1000);
			ob1->SetHP(0);
			ob2->SetInvin(5.f);
			m_Sound->PlaySound(m_hit, false, 0.5);
		}
		

	}
}

void ScnMgr::GarbageCollector()
{
	for (int i = 0; i < MAX_OBJECTS; i++)
	{
		if (m_Objects[i] != NULL && i != HERO_ID)
		{
			float x, y, z;
			int hp;
			int kind;
			float vel_x, vel_y, vel_z;
			float mag;
			m_Objects[i]->GetPos(&x, &y, &z);
			m_Objects[i]->GetKind(&kind);
			m_Objects[i]->GetHP(&hp);
			m_Objects[i]->GetVel(&vel_x, &vel_y, &vel_z);
			mag = sqrtf(vel_x * vel_x + vel_y * vel_y + vel_z * vel_z);
			if (x > 6.f || x < -6.f || y > 4.f || y < -4.f)
			{
				DeleteObject(i);
				continue;
			}
			if (kind == KIND_BULLET || kind == KIND_ENEMY)
			{
				if (hp <= 0)
				{
					if (kind == KIND_ENEMY)
						g_Score++;
					DeleteObject(i);
					//std::cout << g_Score << std::endl;
					continue;
					
				}
			}
			if(kind == KIND_BULLET)
			{
				if (mag < FLT_EPSILON)
				{
					DeleteObject(i);
					break;
				}
			}
		}
	}
}

void ScnMgr::ApplyForce(float x, float y, float z, float eTime)
{
	int state;
	float vel_x, vel_y;
	m_Objects[HERO_ID]->GetState(&state);
	if (state == STATE_AIR)
		z = 0.f;
	m_Objects[HERO_ID]->ApplyForce(x, y, z, eTime);
	m_Objects[HERO_ID]->GetXvel(&vel_x);
	m_Objects[HERO_ID]->GetYvel(&vel_y);	
	

	for (int i = 0; i < MAX_OBJECTS; i++)//적이 플레이어를 향해 움직임.
	{
		int kind;
		float bul_x, bul_y, hero_x, hero_y;
		float x, y;
		float temp;
		if (m_Objects[i] == NULL)
			continue;
		m_Objects[i]->GetKind(&kind);
		if (kind != KIND_ENEMY)
			continue;
		m_Objects[i]->GetXpos(&bul_x);
		m_Objects[i]->GetYpos(&bul_y);
		m_Objects[HERO_ID]->GetXpos(&hero_x);
		m_Objects[HERO_ID]->GetYpos(&hero_y);
		x = hero_x - bul_x;
		y = hero_y - bul_y;
		temp = sqrtf(x * x + y * y);
		m_Objects[i]->ApplyForce(x / temp * 3, y / temp * 3, 0, eTime);
	}
}


void ScnMgr::AddObject(float x, float y, float z,
	float sx, float sy, float sz,
	float vx, float vy, float vz,
	int kind,
	int hp,
	int state)
{
	int id = FindEmptyObjectSlot();
	if (id < 0)
	{
		return;
	}
	m_Objects[id] = new Object;
	m_Objects[id]->SetPos(x, y, z);
	m_Objects[id]->SetSize(sx, sy, sz);
	m_Objects[id]->SetColor(1, 1, 1, 1);
	m_Objects[id]->SetColor(1, 1, 1, 1);
	m_Objects[id]->SetVel(vx, vy, vz);
	m_Objects[id]->SetAcc(0.f, 0.f, 0.f);
	m_Objects[id]->SetMass(1.f);
	m_Objects[id]->SetCoefFrict(0.2f);
	m_Objects[id]->SetKind(kind);
	m_Objects[id]->SetHP(hp);
	if (kind == KIND_BULLET)
	{
		m_Objects[id]->SetCoefFrict(0.1f);
	}
}

void ScnMgr::WallCollision()
{
	float pos_x, pos_y, pos_z;
	float vel_x, vel_y, vel_z;
	float size_x, size_y, size_z;
	float temp;
	int kind;
	for (int i = 0; i < MAX_OBJECTS; i++)
	{
		if (m_Objects[i] == NULL )
			continue;
		m_Objects[i]->GetKind(&kind);
		if (kind == KIND_BULLET)
			continue;
		m_Objects[i]->GetPos(&pos_x, &pos_y, &pos_z);
		m_Objects[i]->GetVel(&vel_x, &vel_y, &vel_z);
		m_Objects[i]->GetSize(&size_x, &size_y, &size_z);
		if (pos_x > 6.f - size_x / 2.f)
		{
			m_Objects[i]->SetXpos(6.f - size_x / 2.f);
			m_Objects[i]->GetXvel(&temp);
			if (temp >= 0)
				m_Objects[i]->SetXvel(-temp);
		}
		else if (pos_x < -(6.f - size_x / 2.f))
		{
			m_Objects[i]->SetXpos(-(6.f - size_x / 2.f));
			m_Objects[i]->GetXvel(&temp);
			if (temp <= 0)
				m_Objects[i]->SetXvel(-temp);
		}
		if (pos_y > 4.f - size_y / 2.f)
		{
			m_Objects[i]->SetYpos(4.f - size_y / 2.f);
			m_Objects[i]->GetYvel(&temp);
			if (temp >= 0)
				m_Objects[i]->SetYvel(-temp);
		}
		else if (pos_y < -(4.f - size_y / 2.f))
		{
			m_Objects[i]->SetYpos(-(4.f - size_y / 2.f));
			m_Objects[i]->GetYvel(&temp);
			if (temp <= 0)
				m_Objects[i]->SetYvel(-temp);
		}
	}
	
}
void ScnMgr::DeleteObject(int id)
{
	if (m_Objects[id] != NULL)
	{
		delete m_Objects[id];
		m_Objects[id] = NULL;
	}
}

int ScnMgr::FindEmptyObjectSlot()
{
	for (int i = 0; i < MAX_OBJECTS; i++)
	{
		if (m_Objects[i] == NULL)
			return i;
	}
	std::cout << "No more empty slot!\n";
	return -1;
}

void ScnMgr::Shoot(int shootID)
{
	if (shootID == SHOOT_NONE)
		return;
	float amount = 8.f;
	float bvX, bvY, bvZ;

	bvX = 0.f;
	bvY = 0.f;
	bvZ = 0.f;

	switch (shootID)
	{
	case SHOOT_UP:
		bvX = 0.f;
		bvY = amount;
		break;
	case SHOOT_DOWN:
		bvX = 0.f;
		bvY = -amount;
		break;
	case SHOOT_LEFT:
		bvX = -amount;
		bvY = 0.f;
		break;
	case SHOOT_RIGHT:
		bvX = amount;
		bvY = 0.f;
		break;
	}

	float pX, pY, pZ;
	m_Objects[HERO_ID]->GetPos(&pX, &pY, &pZ);
	float vX, vY, vZ;
	m_Objects[HERO_ID]->GetVel(&vX, &vY, &vZ);

	bvX = bvX + vX;
	bvY = bvY + vY;
	bvZ = bvZ + vZ;

	AddObject(pX, pY, pZ, 0.2f, 0.2f, 0.2f, bvX, bvY, bvZ, KIND_BULLET, 1000, STATE_AIR);
	m_Sound->PlaySound(m_Gun, false, 0.3f);
}

//배경음악, 총쏠때, 충돌할때 사운드 구현하기. bullet && building

void ScnMgr::Init()
{
	for (int i = 1; i < MAX_OBJECTS; i++)
	{
		DeleteObject(i);
	}
	m_Objects[HERO_ID]->SetPos(0.f, 0.f, 0.f);
	m_Objects[HERO_ID]->SetSize(1.f, 1.f, 1.f);
	m_Objects[HERO_ID]->SetColor(1, 1, 1, 1);
	m_Objects[HERO_ID]->SetVel(0.f, 0.f, 0.f);
	m_Objects[HERO_ID]->SetAcc(0.f, 0.f, 0.f);
	m_Objects[HERO_ID]->SetMass(1.f);
	m_Objects[HERO_ID]->SetCoefFrict(0.7f);
	m_Objects[HERO_ID]->SetKind(KIND_HERO);
	m_Objects[HERO_ID]->SetHP(10000);
	m_Objects[HERO_ID]->SetState(STATE_GROUND);
	m_Objects[HERO_ID]->SetInvin(0.f);
	g_Score = 0;

	g_InGame = GAME_START;

}