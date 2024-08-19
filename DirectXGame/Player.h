#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "MathUtilityForText.h"

//左右
enum class LRDirection {
	kRight,
	kLeft,
};

class MapChipField;

class Enemy;

class Player {
public: // メンバ関数
	//マップとの当たり判定情報
	struct CollisionMapInfo {
		bool ceiling = false;
		bool landing = false;
		bool hitWall = false;
		Vector3 move = {0};
	};

	//角
	enum Corner {
		kRightBottom,  //右下
		kLeftBottom,  //左下
		kRightTop,  //右上
		kLeftTop,  //左上

		kNumCorner  //要素数
	};

		/// <summary>
		/// 初期化
		/// </summary>
		void Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position);

		/// <summary>
		/// 毎フレーム処理
		/// </summary>
		void Update();

		void InputMove();

		/// <summary>
		/// 描画
		/// </summary>
		void Draw();

		const WorldTransform& GetWorldTransform() const { return worldTransfrom_; }

		const Vector3& GetVelocity() const { return velocity_; }

		void SetMapChipField(MapChipField* mapChipField) { mapChipField_ = mapChipField; }

		//指定した角の座標計算
		Vector3 CornerPosition(const Vector3& center, Corner corner);

		//マップ衝突判定
		void CheckMapCollision(CollisionMapInfo& info);

		//マップ衝突判定上方向
	    void CheckMapCollisionUp(CollisionMapInfo& info);

		//マップ衝突判定下方向
	    void CheckMapCollisionDown(CollisionMapInfo& info);

		//マップ衝突判定右方向
	    void CheckMapCollisionRight(CollisionMapInfo& info);

		//マップ衝突判定左方向
	    void CheckMapCollisionLeft(CollisionMapInfo& info);

		//判定結果を反映して移動させる
		void CheckMapMove(const CollisionMapInfo& info);

		//天井に接触している場合の処理
	    void CheckMapCeiling(const CollisionMapInfo& info);

		//壁に接触している場合の処理
	    void CheckMapWall(const CollisionMapInfo& info);

		//接地状態の切り替え処理
	    void CheckMapLanding(const CollisionMapInfo& info);

		//旋回制御
	    void AnimateTurn();

		//ワールド座標を取得
	    Vector3 GetWorldPosition();

		//AABBを取得
	    AABB GetAABB();

		//衝突判定
	    void OnCollision(const Enemy* enemy);

private:
	// 慣性移動
	static inline const float kAcceleration = 0.01f;
	static inline const float KAttenuation = 0.05f;
	static inline const float kLimitRunSpeed = 0.2f;
	//旋回時間
	static inline const float kTimeTurn = 0.3f;
	//重力加速度
	static inline const float kGravityAcceleration = 0.05f;
	//最大落下速度
	static inline const float kLimitFallSpeed = 0.5f;
	//着地時の速度減速率
	static inline const float kAttenuationWall = 0.1f;
	//着地時の速度減衰
	static inline const float KAttenuationLanding = 0.1f;
	//ジャンプ初速
	static inline const float kJumpAcceleration = 0.5f;
	//落下用
	static inline const float kGroundSearchHeight = 0.06f;
	
	//キャラクターの当たり判定サイズ
	static inline const float kWidth = 0.8f;
	static inline const float kHeight = 0.8f;
	//ブロック
	static inline const float kBlank = 0.1f;

	// メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	//モデル
	Model* model_ = nullptr;
	ViewProjection* viewProjection_ = nullptr;
	//マップチップによるフィールド
	MapChipField* mapChipField_ = nullptr;
	//ワールド変換データ
	WorldTransform worldTransfrom_;
	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	//速度
	Vector3 velocity_ = {};

	//左右
	LRDirection lrDirection_ = LRDirection::kRight;

	//旋回開始時の角度
	float turnFirstRotationY_ = 0.0f;
	//旋回タイマー
	float turnTimer_ = 0.0f;

	//接地状態フラグ
	bool onGround_ = true;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
};
