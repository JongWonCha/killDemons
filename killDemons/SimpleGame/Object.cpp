#include "stdafx.h"
#include "Object.h"
#include "global.h"


Object::Object()
{
}


Object::~Object()
{
}

void Object::GetPos(float *x, float *y, float *z)
{ 
	*x = m_PosX;
	*y = m_PosY;
	*z = m_PosZ;
}
void Object::SetPos(float x, float y, float z)
{
	this->m_PosX = x;
	this->m_PosY = y;
	this->m_PosZ = z;
}

void Object::GetXpos(float *x)
{
	*x = m_PosX;
}

void Object::SetXpos(float x)
{
	this->m_PosX = x;
}

void Object::GetYpos(float *y)
{
	*y = m_PosY;
}

void Object::SetYpos(float y)
{
	this->m_PosY = y;
}

void Object::GetZpos(float *z)
{
	*z = m_PosZ;
}

void Object::SetZpos(float z)
{
	this->m_PosZ = z;
}


void Object::GetSize(float *x, float *y, float *z)
{
	*x = this->m_SizeX;
	*y = this->m_SizeY;
	*z = this->m_SizeZ;
}
void Object::SetSize(float x, float y, float z)
{
	this->m_SizeX = x;
	this->m_SizeY = y;
	this->m_SizeZ = z;
}
void Object::GetColor(float *r, float *g, float *b, float *a)
{
	*r = this->m_r;
	*g = this->m_g;
	*b = this->m_b;
	*a = this->m_a;
}
void Object::SetColor(float r, float g, float b, float a)
{
	this->m_r = r;
	this->m_g = g;
	this->m_b = b;
	this->m_a = a;
}

void Object::GetVel(float *x, float *y, float *z)
{
	*x = this->m_VelX;
	*y = this->m_VelY;
	*z = this->m_VelZ;
}
void Object::SetVel(float x, float y, float z)
{
	this->m_VelX = x;
	this->m_VelY = y;
	this->m_VelZ = z;
}

void Object::GetXvel(float *x)
{
	*x = this->m_VelX;
}
void Object::SetXvel(float x)
{
	this->m_VelX = x;
}

void Object::GetYvel(float *y)
{
	*y = this->m_VelY;
}
void Object::SetYvel(float y)
{
	this->m_VelY = y;
}

void Object::GetZvel(float *z)
{
	*z = this->m_VelX;
}
void Object::SetZvel(float z)
{
	this->m_VelZ = z;
}

void Object::GetAcc(float *x, float *y, float *z)
{
	*x = this->m_AccX;
	*y = this->m_AccY;
	*z = this->m_AccZ;
}
void Object::SetAcc(float x, float y, float z)
{
	this->m_AccX = x;
	this->m_AccY = y;
	this->m_AccZ = z;
}


void Object::GetMass(float *x)
{
	*x = this->m_Mass;
}
void Object::SetMass(float x)
{
	this->m_Mass = x;
}

void Object::GetCoefFrict(float *x)
{
	*x = this->m_CoefFrict;
}
void Object::SetCoefFrict(float x)
{
	this->m_CoefFrict = x;
}

void Object::SetKind(int kind)
{
	this->m_kind = kind;
}
void Object::GetKind(int *kind)
{
	*kind = this->m_kind;
}

void Object::SetHP(int hp)
{
	this->m_HP = hp;
}
void Object::GetHP(int *hp)
{
	*hp = this->m_HP;
}

void Object::SetState(int state)
{
	this->m_State = state;
}
void Object::GetState(int *state)
{
	*state = this->m_State;
}

void Object::GetInvin(float *invin)
{
	*invin = this->m_InvinTime;
}
void Object::SetInvin(float invin)
{
	this->m_InvinTime = invin;
}

void Object::CalInvin(float delta)
{
	this->m_InvinTime -= delta;
}


void Object::Update(float eTime)
{
	float magVel = sqrtf(m_VelX * m_VelX + m_VelY * m_VelY + m_VelZ * m_VelZ);//이동방향의 벡터를 구한 후 정규화 할 것이다.
	float velX = m_VelX / magVel;
	float velY = m_VelY / magVel;
	float VelZ = m_VelZ / magVel;
	float fricX = -velX;
	float fricY = -velY;//이동 단위 벡터의 역벡터를 구함.

	float friction = m_CoefFrict * m_Mass * GRAVITY;
	fricX *= friction;
	fricY *= friction;


	if (magVel < FLT_EPSILON)//멈춰있을때 예외 처리, float을 같기 연산을 하면 안된다.
	{
		m_VelX = 0.f;
		m_VelY = 0.f;
		m_VelZ = 0.f;
	}
	else
	{
		float accX = fricX / m_Mass;
		float accY = fricY / m_Mass;

		float afterVelX = m_VelX + accX * eTime;
		float afterVelY = m_VelY + accY * eTime;
		
		if (afterVelX * m_VelX < 0.f)
			m_VelX = 0.f;
		else
			m_VelX = afterVelX;
		if (afterVelY * m_VelY < 0.f)
			m_VelY = 0.f;
		else
			m_VelY = afterVelY;

		//gravity
		m_VelZ = m_VelZ - GRAVITY * eTime;
	}

	//cal Velocity
	m_VelX = m_VelX + m_AccX * eTime;
	m_VelY = m_VelY + m_AccY * eTime;
	m_VelZ = m_VelZ + m_AccZ * eTime;
	//cal Position
	m_PosX = m_PosX + m_VelX * eTime;
	m_PosY = m_PosY + m_VelY * eTime;
	m_PosZ = m_PosZ + m_VelZ * eTime;
	//std::cout << m_PosX << ' ' << m_PosY << std::endl;
	if (m_PosZ <= 0.f)
	{
		m_State = STATE_GROUND;
		m_VelZ = 0.f;
		m_PosZ = 0.f;
	}
	else
	{
		m_State = STATE_AIR;
	}
}




void Object::ApplyForce(float x, float y, float z, float eTime)
{
	//cal accelaration
	m_AccX = x / m_Mass;
	m_AccY = y / m_Mass;
	m_AccZ = z / m_Mass;
	
	m_VelX = m_VelX + m_AccX * eTime;
	m_VelY = m_VelY + m_AccY * eTime;
	m_VelZ = m_VelZ + m_AccZ * 0.1f;

	m_AccX = 0.f;
	m_AccY = 0.f;
	m_AccZ = 0.f;
}


