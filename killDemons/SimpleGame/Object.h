#pragma once
class Object
{
public:
	Object();
	~Object();
	void GetPos(float *x, float *y, float *z);
	void SetPos(float x, float y, float z);

	void GetXpos(float *x);
	void SetXpos(float x);

	void GetYpos(float *y);
	void SetYpos(float y);

	void GetZpos(float *z);
	void SetZpos(float z);

	void GetSize(float *x, float *y, float *z);
	void SetSize(float x, float y, float z);

	void GetColor(float *r, float *g, float *b, float *a);
	void SetColor(float r, float g, float b, float a);

	void GetVel(float *x, float *y, float *z);
	void SetVel(float x, float y, float z);

	void GetXvel(float *x);
	void SetXvel(float x);

	void GetYvel(float *y);
	void SetYvel(float y);

	void GetZvel(float *z);
	void SetZvel(float z);

	void GetAcc(float *x, float *y, float *z);
	void SetAcc(float x, float y, float z);

	void GetMass(float *x);
	void SetMass(float x);

	void GetCoefFrict(float *x);
	void SetCoefFrict(float x);

	void SetKind(int kind);
	void GetKind(int *kind);

	void SetHP(int hp);
	void GetHP(int *hp);

	void SetState(int state);
	void GetState(int *state);

	void GetInvin(float *invin);
	void SetInvin(float invin);

	void CalInvin(float delta);


	void ApplyForce(float x, float y, float z, float eTime);

	void Update(float eTime);
private:
	float m_PosX, m_PosY, m_PosZ;//position
	float m_SizeX, m_SizeY, m_SizeZ;//size
	float m_r, m_g, m_b, m_a;//color
	float m_VelX, m_VelY, m_VelZ;//velocity
	float m_AccX, m_AccY, m_AccZ;//accelaration
	float m_Mass;//mass
	float m_CoefFrict;//마찰계수
	int m_HP;//체력
	int m_State;//상태

	int m_kind;

	float m_InvinTime = 0; //무적시간, 0이 아니면 무적이다.

};

