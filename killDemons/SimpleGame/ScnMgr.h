#pragma once
#include "Object.h"
#include "Renderer.h"
#include "Sound.h"

class ScnMgr
{
public:
	ScnMgr();
	~ScnMgr();

	void RenderScene(float eTime);
	void Update(float eTime);
	void ApplyForce(float x, float y, float z, float eTime);
	void Shoot(int shootID);
	void AddObject(float x, float y, float z,
		float sx, float sy, float sz,
		float vx, float vy, float vz,
		int kind,
		int hp,
		int state);
	int FindEmptyObjectSlot();
	void DeleteObject(int id);
	void GarbageCollector();
	bool RRCollision(float minX, float minY, float maxX, float maxY,
		float minX1, float minY1, float maxX1, float maxY1);
	bool BBCollision(float minX, float minY, float minZ, float maxX, float maxY, float maxZ,
		float minX1, float minY1, float minZ1, float maxX1, float maxY1, float maxZ1);
	void DoCollisionTest();
	void ProcessCollision(int i, int j);

	void WallCollision();

	void Init();

private:
	Renderer * m_Renderer;
	Object *m_Objects[MAX_OBJECTS];
	Sound *m_Sound;

	GLuint m_HeroTexture = 0;
	GLuint m_TexSeq = 0;
	GLuint m_TestSound = 0;
	GLuint m_SmokeTexture = 0;
	GLuint m_TestTexture = 0;
	GLuint m_EnemyTexture = 0;
	GLuint m_NumberTexture = 0;
	GLuint m_BGTexture = 0;
	GLuint m_CongTexture = 0;
	GLuint m_EnemyEndTexture = 0;
	GLuint m_HeroEndTexture = 0;
	GLuint m_GameOverTexture = 0;
	GLuint m_KillTexture = 0;
	GLuint m_RestartTexture = 0;

	int m_Gun = 0;
	int m_BG = 0;
	int m_Explode = 0;
	int m_hit = 0;

};

