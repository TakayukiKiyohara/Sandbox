/*!
 * @brief	�G�l�~�[�}�l�[�W���B
 */

#pragma once


/*!
 * @brief	�G����̃}�l�[�W���[�B
 *@details
 * Facade�p�^�[���B
 */
class EnemyManager : public IGameObject{
public:
	EnemyManager();
	~EnemyManager();
	void Start() override;
	void Update() override;
	void Render(CRenderContext& renderContext) override;
private:
	
};