/*!
 *@brief	ゲームカメラ
 */

#pragma once

#include "tkEngine/camera/tkSpringCamera.h"

class Player;
class Car;
/*!
 *@brief	ゲームカメラ。
 */
class GameCamera : public IGameObject{
public:
	GameCamera();
	~GameCamera();
	void Start() override ;
	void Update() override ;
	void Render( CRenderContext& renderContext ) override;
	void SetPlayer(Player* player)
	{
		this->player = player;
	}
	void SetCar(Car* car)
	{
		this->car = car;
	}
	const CCamera& GetCamera() const
	{
		return *springCamera.GetCamera();
	}
	void SetDampingRate(float rate)
	{
		springCamera.SetDampingRate(rate);
	}
	void SetViewAngle(float angle)
	{
		m_targetViewAngle = angle;
	}
private:
	//CCamera camera;
	CSpringCamera springCamera;
	Car* car;
	Player*	player;
	CVector3	toPosition;		//注視点から視点へのベクトル。
	float m_targetViewAngle;	//ビューアングル。
};


extern GameCamera*			g_camera;				//カメラ。