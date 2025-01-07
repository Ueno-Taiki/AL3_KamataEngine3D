#include <cassert>
#include "Player.h"
#include "Input.h"
#include "MathUtilityForText.h"
#include "TextureManager.h"

void Player::Initialize(Model* model, uint32_t textureHandle) {
	//NULLポインタチャック
	assert(model);

	//引数として受け取ったデータをメンバ変数に記録
	model_ = model;
	textureHandle_ = textureHandle;

	//ファイルを指定してテクスチャを読み込む
	TextureHandle_[0] = TextureManager::Load("PlayerLife1.png");
	TextureHandle_[1] = TextureManager::Load("PlayerLife2.png");

	//スプライトの生成
	sprite_[0] = Sprite::Create(TextureHandle_[0], {20, 20});
	sprite_[1] = Sprite::Create(TextureHandle_[0], {120, 20});
	sprite_[2] = Sprite::Create(TextureHandle_[0], {220, 20});
	sprite_[3] = Sprite::Create(TextureHandle_[1], {20, 20});
	sprite_[4] = Sprite::Create(TextureHandle_[1], {120, 20});
	sprite_[5] = Sprite::Create(TextureHandle_[1], {220, 20});

	//シングルトンインスタンスを取得する
	input_ = Input::GetInstance();

	//ワールド変換の初期化
	worldTransform_.Initialize();
}

Player::~Player() { 
	//bullet_の解放
	for (PlayerBullet* bullet : bullets_) {
		delete bullet;
	}
	for (int i = 0; i < 6; i++) {
		delete sprite_[i];
	}
}

void Player::Update() {
	//デスフラグの立った弾を削除
	bullets_.remove_if([](PlayerBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
		});

	//キャラクターの移動ベクトル
	Vector3 move = {0, 0, 0};

	//キャラクターの移動速さ
	const float kCharacterSpeed = 0.2f;

	//押した方向で移動ベクトルを変更(左右)
	if (input_->PushKey(DIK_LEFT)) {
		move.x -= kCharacterSpeed;
	} else if (input_->PushKey(DIK_RIGHT)) {
		move.x += kCharacterSpeed;
	}

	//押した方向で移動ベクトルを変更(上下)
	if (input_->PushKey(DIK_UP)) {
		move.y += kCharacterSpeed;
	} else if (input_->PushKey(DIK_DOWN)) {
		move.y -= kCharacterSpeed;
	}

	//座標移動(ベクトルの加算)
	worldTransform_.translation_ += move;

	for (WorldTransform* worldTransformBlock : worldTransformBlocks_) {
		//アフィン変換作成
		Matrix4x4 matWorld = MakeAffineMatrix(worldTransformBlock->scale_, worldTransformBlock->rotation_, worldTransformBlock->translation_);
		worldTransformBlock->matWorld_ = matWorld;

		//定数バッファに転送する
		worldTransformBlock->TransferMatrix();
	}

	//旋回
	Rotate();

	//攻撃
	if (!isDead_) {
		Attack();
	}

	//弾更新
	for (PlayerBullet* bullet : bullets_) {
		bullet->Update();
	}

	//移動限界座標
	const float kMoveLimitX = 35.0f;
	const float kMoveKimitY = 19.0f;

	//範囲を超えない処理
	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveKimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveKimitY);

	//アフィン変換と転送
	worldTransform_.UpdateMatrix();
}

void Player::Draw(ViewProjection& viewProjection) { 
	if (!isDead_) {
		model_->Draw(worldTransform_, viewProjection, textureHandle_);
	}

	//弾描画
	for (PlayerBullet* bullet : bullets_) {
		bullet->Draw(viewProjection);
	}
}

//ライフ描画
void Player::LifeDraw() {
	if (playerLife == 3) {
		sprite_[0]->Draw();
		sprite_[1]->Draw();
		sprite_[2]->Draw();
	}
	if (playerLife == 2) {
		sprite_[3]->Draw();
		sprite_[1]->Draw();
		sprite_[2]->Draw();
	}
	if (playerLife == 1) {
		sprite_[3]->Draw();
		sprite_[4]->Draw();
		sprite_[2]->Draw();
	}
	if (playerLife == 0) {
		sprite_[3]->Draw();
		sprite_[4]->Draw();
		sprite_[5]->Draw();
	}
}

//旋回
void Player::Rotate() {
	//回転速さ[ラジアン/frame]
	const float kRotSpeed = 0.02f;

	//押した方向で移動ベクトルを変更
	if (input_->PushKey(DIK_A)) {
		worldTransform_.rotation_.y -= kRotSpeed;
	} else if (input_->PushKey(DIK_D)) {
		worldTransform_.rotation_.y += kRotSpeed;
	}
}

//攻撃
void Player::Attack() { 
	if (input_->PushKey(DIK_SPACE)) {
		//弾の速度
		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0, 0, kBulletSpeed);

		//速度ベクトルを自機の方向に合わせて回転させる
		velocity = TransformNormal(velocity, worldTransform_.matWorld_);

		//弾を生成し、初期化
		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Initialize(model_, worldTransform_.translation_, velocity);

		//弾を登録する
		bullets_.push_back(newBullet);
	}
}

// ワールド座標を取得
Vector3 Player::GetWorldPosition() const { 
	//ワールド座標を入れる変数
	Vector3 worldPos = {};
	//ワールド行列の平行移動成分を取得
	worldPos.x = worldTransform_.translation_.x;
	worldPos.y = worldTransform_.translation_.y;
	worldPos.z = worldTransform_.translation_.z;

	return worldPos; 
}

//衝突を検出したら呼び出されるコールバック関数
void Player::OnCollision() { 
	//ダメージを受ける
	playerLife -= damage;

	//ライフが0の時死ぬ
	if (playerLife == 0) {
		isDead_ = true;
	}
}

