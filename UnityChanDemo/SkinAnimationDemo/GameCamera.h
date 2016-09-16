/*!
 *@brief	ゲームカメラ
 */

#pragma once

class UnityChan;
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
	void SetUnityChan(UnityChan* unityChan)
	{
		this->unityChan = unityChan;
	}
	void SetCar(Car* car)
	{
		this->car = car;
	}
	const CCamera& GetCamera() const
	{
		return camera;
	}
private:
	CCamera camera;
	Car* car;
	UnityChan*	unityChan;
	CVector3	toPosition;		//注視点から視点へのベクトル。
};


extern GameCamera*			g_camera;				//カメラ。