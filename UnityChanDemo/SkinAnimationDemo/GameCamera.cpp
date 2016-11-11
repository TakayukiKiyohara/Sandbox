#include "stdafx.h"
#include "GameCamera.h"
#include "Player/Player.h"
#include "Car.h"

GameCamera			*g_camera;				//�J�����B

GameCamera::GameCamera() :
	player(nullptr),
	car(nullptr)
{
}
GameCamera::~GameCamera()
{
}
void GameCamera::Start()
{
	CVector3 cameraPos(0.0f, 0.8f, 3.0f);
	CVector3 cameraTarget(0.0f, 0.2f, 0.0f);
	toPosition.Subtract(cameraPos, cameraTarget);
	cameraTarget = player->GetPosition();
	cameraTarget.y += 1.0f;
	cameraPos.Add(cameraTarget, toPosition);
	springCamera.Init(cameraTarget, cameraPos, 100.0f);
	springCamera.SetFar(1000.0f);
	m_targetViewAngle = springCamera.GetViewAngle();
	springCamera.Update();
	//���t���N�V�����}�b�v�ɃJ������ݒ肷��B
	ReflectionMap().SetCamera(*springCamera.GetCamera());
}
void GameCamera::Update()
{
	float viewAngle = m_targetViewAngle * 0.1f + springCamera.GetViewAngle() * 0.9f;
	springCamera.SetViewAngle(viewAngle);
	//�J��������]������B
	float rStick_x = Pad(0).GetRStickXF();
	float rStick_y = Pad(0).GetRStickYF();
	
	if (fabsf(rStick_x) > 0.0f) {
		//Y������̉�]���v�Z�B
		//��
		CMatrix mRot;
		mRot.MakeRotationY(0.05f * rStick_x);
		mRot.Mul(toPosition);
	}
	if (fabsf(rStick_y) > 0.0f) {
		CVector3 rotAxis;
		rotAxis.Cross(CVector3::Up, toPosition);
		rotAxis.Normalize();
		CMatrix mRot;
		mRot.MakeRotationAxis(rotAxis, 0.05f * rStick_y);
		CVector3 toPositionOld = toPosition;
		mRot.Mul(toPosition);
		CVector3 toPosDir = toPosition;
		toPosDir.Normalize();
		if (toPosDir.y < -0.5f) {
			//�J����������������B
			toPosition = toPositionOld;
		}
		else if (toPosDir.y > 0.8f) {
			//�J�����������������B
			toPosition = toPositionOld;
		}
	}
	CVector3 v;
	if (car) {
		//�Ԃ̌��ɒǏ]�B
		v = car->GetPosition();
		v.y += 1.0f;
		springCamera.SetTarget(v);
		CVector3 toPosOnCar = car->GetMoveDirection();
		toPosOnCar.Scale(-3.0f);
		toPosOnCar.y += 1.0f;
		v.Add(toPosOnCar);
		springCamera.SetPosition(v);
	}
	else {
		v = player->GetPosition();
		v.y += 1.0f;
		springCamera.SetTarget(v);
		v.Add(toPosition);
		springCamera.SetPosition(v);
		
	}
	springCamera.Update();
	//��ʊE�[�x�̃p�����[�^���X�V
	Dof().SetFocalLength(26.0f);
	Dof().SetFParam(5.6f);
	Dof().SetPint(toPosition.Length() * 1000.0f);
	//3D�T�E���h�̃��X�i�[�̓J�����B
	SoundEngine().SetListenerPosition(g_player->GetPosition());
	const CMatrix& m = springCamera.GetCameraRotation();
	SoundEngine().SetListenerFront({ m.m[2][0], m.m[2][1], m.m[2][2] });
	SoundEngine().SetListenerUp({ m.m[1][0], m.m[1][1], m.m[1][2] });
}
void GameCamera::Render( CRenderContext& renderContext )
{
}