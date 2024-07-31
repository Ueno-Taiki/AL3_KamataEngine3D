#define NOMINMAX
#include "Player.h"
#include "Input.h"
#include "MathUtilityForText.h"
#include "MapChipField.h"
#include <cassert>
#include <numbers>
#include <algorithm>
#include <DebugText.h>

void Player::Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position) {

	// NULLポインタをチェック
	assert(model);

	// 引数として受け取ったデータをメンバ変数に記録する
	model_ = model;
	viewProjection_ = viewProjection;
	// ワールド変数の初期化
	worldTransfrom_.Initialize();
	worldTransfrom_.translation_ = position;

	//初期回転
	worldTransfrom_.rotation_.y = std::numbers::pi_v<float> / 2.0f;
}

void Player::Update() {
	//➀移動入力
	InputMove();
	
	//➁マップ衝突チェック
	//衝突情報を初期化
	CollisionMapInfo collisionMapInfo;
	//移動量に速度の値をコピー
	collisionMapInfo.move = velocity_;

	//マップ衝突チェック
	CheckMapCollision(collisionMapInfo);

	//➂判定結果を反映して移動させる
	CheckMapMove(collisionMapInfo);

	//➃天井に接触している場合の処理
	CheckMapCeiling(collisionMapInfo);

	//➄壁に接触している場合の処理
	CheckMapWall(collisionMapInfo);

	//➅接地状態の切り替え処理
	CheckMapLanding(collisionMapInfo);

	//➆旋回制御
	AnimateTurn();

	//➇行列計算
	worldTransfrom_.UpdateMatrix();
}

void Player::Draw() {
	//3Dモデル描画
	model_->Draw(worldTransfrom_, *viewProjection_);
}

//移動入力
void Player::InputMove() {
	if (onGround_) {
		// 左右移動
		if (Input::GetInstance()->PushKey(DIK_RIGHT) || Input::GetInstance()->PushKey(DIK_LEFT)) {
			// 左右加速
			Vector3 acceleration = {};
			if (Input::GetInstance()->PushKey(DIK_RIGHT)) {
				// 左移動中の右入力
				if (velocity_.x < 0.0f) {
					// 速度と逆方向に入力中は急ブレーキ
					velocity_.x *= (1.0f - KAttenuation);
				}
				acceleration.x += kAcceleration;
				if (lrDirection_ != LRDirection::kRight) {
					lrDirection_ = LRDirection::kRight;
					// 旋回開始時の角度を記録する
					turnFirstRotationY_ = worldTransfrom_.rotation_.y;
					// 旋回タイマーに時間を設定
					turnTimer_ = kTimeTurn;
				}
			} else if (Input::GetInstance()->PushKey(DIK_LEFT)) {
				// 右移動中の左入力
				if (velocity_.x > 0.0f) {
					// 速度と逆方向に入力中は急ブレーキ
					velocity_.x *= (1.0f - KAttenuation);
				}
				acceleration.x -= kAcceleration;
				if (lrDirection_ != LRDirection::kLeft) {
					lrDirection_ = LRDirection::kLeft;
					// 旋回開始時の角度を記録する
					turnFirstRotationY_ = worldTransfrom_.rotation_.y;
					// 旋回タイマーに時間を設定
					turnTimer_ = kTimeTurn;
				}
			}
			// 加速／減速
			velocity_.x += acceleration.x;

			// 最大速度制限
			velocity_.x = std::clamp(velocity_.x, -kLimitRunSpeed, kLimitRunSpeed);

		} else {
			// 非入力時は移動減衰をかける
			velocity_.x *= (1.0f - KAttenuation);
		}
		if (Input::GetInstance()->PushKey(DIK_UP)) {
			// ジャンプ初速
			velocity_ += Vector3(0, kJumpAcceleration, 0);
		}
		// 空中
	} else {
		// 落下速度
		velocity_ += Vector3(0, -kGravityAcceleration, 0);
		// 落下速度
		velocity_.y = std::max(velocity_.y, -kLimitRunSpeed);
	}
}

//マップ衝突判定
void Player::CheckMapCollision(CollisionMapInfo& info) {
	CheckMapCollisionUp(info);
	CheckMapCollisionDown(info);
	CheckMapCollisionRight(info);
	CheckMapCollisionLeft(info);
}

//マップ衝突判定上方向
void Player::CheckMapCollisionUp(CollisionMapInfo& info) {
	// 上昇あり
	if (info.move.y <= 0) {
		return;
	}

	// 移動語の4つの角の座標
	std::array<Vector3, kNumCorner> positionNew = {0};

	for (uint32_t i = 0; i < positionNew.size(); ++i) {
		positionNew[i] = CornerPosition(worldTransfrom_.translation_ + info.move, static_cast<Corner>(i));
	}

	MapChipType mapChipType = {};
	// 真上の当たり判定を行う
	bool hit = false;
	// 左上点の判定
	MapChipField::IndexSet indexSet = {};
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionNew[kLeftTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}
	// 右上点の判定
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionNew[kRightTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}

	// ブロックにヒット
	if (hit) {
		// めり込みを排除する方向に移動量を設定する
		indexSet = mapChipField_->GetMapChipIndexSetByPosition(worldTransfrom_.translation_ + Vector3(0, +kHeight / 2.0f, 0));
		// めり込み先ブロックの範囲矩形
		MapChipField::Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
		info.move.y = std::max(0.0f, rect.bottom - worldTransfrom_.translation_.y - (kHeight / 2.0f + kBlank));
		// 天井に当たったことを記録する
		info.ceiling = true;
	}
}

//マップ衝突判定下方向
void Player::CheckMapCollisionDown(CollisionMapInfo& info) {
	//下降あり
	if (info.move.y >= 0) {
		return;
	}

	//移動後の4つの角の座標の計算
	std::array<Vector3, kNumCorner> positionNew = {0};

	for (uint32_t i = 0; i < positionNew.size(); ++i) {
		positionNew[i] = CornerPosition(worldTransfrom_.translation_ + info.move, static_cast<Corner>(i));
	}

	MapChipType mapChipType;
	//真下の当たり判定を行う
	bool hit = false;
	//左下点の判定
	MapChipField::IndexSet indexSet = {};
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionNew[kLeftBottom]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}
	//右下点の判定
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionNew[kRightBottom]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}

	//ブロックにヒット
	if (hit) {
		//めり込みを排除する方向に移動量を設定する
		indexSet = mapChipField_->GetMapChipIndexSetByPosition(worldTransfrom_.translation_ + Vector3(0, -kHeight / 2.0f, 0));
		//めり込み先ブロックの範囲矩形
		MapChipField::Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
		info.move.y = std::min(0.0f, rect.top - worldTransfrom_.translation_.y - (kHeight / 2.0f + kBlank));
		//地面に当たったことを記録する
		info.landing = true;
	}
}

//マップ衝突判定右方向
void Player::CheckMapCollisionRight(CollisionMapInfo& info) {
	//右移動あり
	if (info.move.x <= 0) {
		return;
	}

	// 移動後の4つの角の座標の計算
	std::array<Vector3, kNumCorner> positionNew = {0};

	for (uint32_t i = 0; i < positionNew.size(); ++i) {
		positionNew[i] = CornerPosition(worldTransfrom_.translation_ + info.move, static_cast<Corner>(i));
	}

	MapChipType mapChipType;
	//右側の当たり判定を行う
	bool hit = false;
	// 右上点の判定
	MapChipField::IndexSet indexSet = {};
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionNew[kRightTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}
	// 右下点の判定
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionNew[kRightBottom]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}

	// ブロックにヒット
	if (hit) {
		// めり込みを排除する方向に移動量を設定する
		indexSet = mapChipField_->GetMapChipIndexSetByPosition(worldTransfrom_.translation_ + Vector3(+kWidth / 2.0f, 0, 0));
		// めり込み先ブロックの範囲矩形
		MapChipField::Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
		info.move.x = std::max(0.0f, rect.right - worldTransfrom_.translation_.x - (kWidth / 2.0f + kBlank));
		// 壁に当たったことを記録する
		info.hitWall = true;
	}
}

//マップ衝突判定左方向
void Player::CheckMapCollisionLeft(CollisionMapInfo& info) {
	// 左移動あり
	if (info.move.x >= 0) {
		return;
	}

	// 移動後の4つの角の座標の計算
	std::array<Vector3, kNumCorner> positionNew = {0};

	for (uint32_t i = 0; i < positionNew.size(); ++i) {
		positionNew[i] = CornerPosition(worldTransfrom_.translation_ + info.move, static_cast<Corner>(i));
	}

	MapChipType mapChipType;
	// 左側の当たり判定を行う
	bool hit = false;
	// 左上点の判定
	MapChipField::IndexSet indexSet = {};
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionNew[kLeftTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}
	// 左下点の判定
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionNew[kLeftBottom]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}

	// ブロックにヒット
	if (hit) {
		// めり込みを排除する方向に移動量を設定する
		indexSet = mapChipField_->GetMapChipIndexSetByPosition(worldTransfrom_.translation_ + Vector3(-kWidth / 2.0f, 0, 0));
		// めり込み先ブロックの範囲矩形
		MapChipField::Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
		info.move.x = std::min(0.0f, rect.left - worldTransfrom_.translation_.x + (kWidth / 2.0f + kBlank));
		// 壁に当たったことを記録する
		info.hitWall = true;
	}
}

//指定した角の座標計算
Vector3 Player::CornerPosition(const Vector3& center, Corner corner) {
	Vector3 offsetTable[kNumCorner] = {
	    {+kWidth / 2.0,  -kHeight / 2.0f, 0},
        {-kWidth / 2.0f, -kHeight / 2.0f, 0},
        {+kWidth / 2.0f, +kHeight / 2.0f, 0},
        {-kWidth / 2.0f, +kHeight / 2.0f, 0}
    };

	return center + offsetTable[static_cast<uint32_t>(corner)];
}

//判定結果を反映して移動させる
void Player::CheckMapMove(const CollisionMapInfo& info) { 
	//移動
	worldTransfrom_.translation_ += info.move; 
}

//天井に接触している場合の処理
void Player::CheckMapCeiling(const CollisionMapInfo& info) {
	//天井に当たった
	if (info.ceiling) {
		DebugText::GetInstance()->ConsolePrintf("hit ceiling\n");
		velocity_.y = 0;
	}
}

//壁に接触している場合の処理
void Player::CheckMapWall(const CollisionMapInfo& info) {
	//壁接触による減速
	if (info.hitWall) {
		velocity_.x *= (1.0f - kAttenuationWall);
	}
}

//接地状態の切り替え処理
void Player::CheckMapLanding(const CollisionMapInfo& info) {
	// 接地判定
	if (onGround_) {
		// ジャンプ開始
		if (velocity_.y > 0.0f) {
			// 空中状態に移行
			onGround_ = false;
		} else {
			// 移動後の4つの角の座標の計算
			std::array<Vector3, kNumCorner> positionNew = {0};

			for (uint32_t i = 0; i < positionNew.size(); ++i) {
				positionNew[i] = CornerPosition(worldTransfrom_.translation_ + info.move, static_cast<Corner>(i));
			}

			MapChipType mapChipType;
			// 真下の当たり判定を行う
			bool hit = false;
			// 左下点の判定
			MapChipField::IndexSet indexSet = {};
			indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionNew[kLeftBottom] + Vector3(0, -kGroundSearchHeight, 0));
			mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
			if (mapChipType == MapChipType::kBlock) {
				hit = true;
			}
			// 右下点の判定
			indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionNew[kRightBottom] + Vector3(0, -kGroundSearchHeight, 0));
			mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
			if (mapChipType == MapChipType::kBlock) {
				hit = true;
			}

			// 落下開始
			if (!hit) {
				// 空中状態に切り替える
				onGround_ = false;
			}
		}
	} else {
		// 着地
		if (info.landing) {
			// 着地状態に切り替える
			onGround_ = true;
			// 着地時にX速度を減衰
			velocity_.x *= (1.0f - KAttenuationLanding);
			// Y速度をゼロにする
			velocity_.y = 0.0f;
		}
	}
}

//旋回制御
void Player::AnimateTurn() {
	// 旋回制御
	if (turnTimer_ > 0.0f) {
		// 旋回タイマーをカウントダウンする
		turnTimer_ -= 1.0f / 60.0f;

		// 左右の自キャラ角度デーブル
		float destinationRotationYTable[] = {std::numbers::pi_v<float> / 2.0f, std::numbers::pi_v<float> * 3.0f / 2.0f};
		// 状態に応じた角度を取得する
		float destinationRotationY = destinationRotationYTable[static_cast<uint32_t>(lrDirection_)];
		// 自キャラの角度を設定する
		worldTransfrom_.rotation_.y = EaseInOut(destinationRotationY, turnFirstRotationY_, turnTimer_ / kTimeTurn);
	}
}
