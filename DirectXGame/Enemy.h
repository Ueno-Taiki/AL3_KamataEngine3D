#pragma once

#include <list>
#include "Model.h"
#include "Input.h"
#include "WorldTransform.h"
#include "EnemyBullet.h"

//自機クラスの前方宣言
class Player;

class Enemy {
public:
	/// </summary>
	/// 初期化
	/// </summary>
	void Initialize(Model* model, uint32_t textureHandle, const Vector3& position);

	/// </summary>
	/// デストラクタ
	/// </summary>
	~Enemy();

	/// </summary>
	/// 初期化
	/// </summary>
	void Update();

	/// </summary>
	/// 初期化
	/// </summary>
	void Draw(ViewProjection& viewProjection);

	//接近
	void Approach(const Vector3& vector);

	//弾発射
	void Fire();

	//接近フェーズ初期化
	void ApproachInitialize();

	//ワールド座標を取得
	Vector3 GetWorldPosition() const;

	//衝突を検出したら呼び出されるコールバック関数
	void OnCollision();

	//敵の当たり判定
	float GetRadius() const { return 1.5f; }

	//setterの利用
	void SetPlayer(Player* player) { player_ = player; }

	//弾リストを取得
	const std::list<EnemyBullet*>& GetBullets() const { return bullets_; }

	//デスフラグのgetter
	bool isDead() const { return isDead_; }

public:
	//発射間隔
	static const int kFireInterval = 60;

private:
	//ワールド変換データ
	WorldTransform worldTransform_;
	//モデル
	Model* model_ = nullptr;
	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	//自キャラ
	Player* player_ = nullptr;

	//敵の速度
	static inline const float kBulletSpeed = -0.05f;

	//弾
	std::list<EnemyBullet*> bullets_;

	//発射タイマー
	int32_t timer = 0;

	//デスフラグ
	bool isDead_ = false;
};
