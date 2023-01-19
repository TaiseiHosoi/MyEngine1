#pragma once
#include "DirectXCommon.h"
#include "SpriteCommon.h"
#include "Sprite.h"
#include "Object3d.h"
#include "Audio.h"
#include "Mesh.h"
#include "scene/Effect.h"
#include "scene/Goal.h"
#include "sstream"
#include "memory"
#include "list"
#include "Bullet.h"


class GameScene {
public:
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(DirectXCommon* dxcomon);

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();


	//弾リストを取得
	std::list<std::unique_ptr<Bullet>> bullets_;
	const std::list<std::unique_ptr<Bullet>>& GetBullets() { return bullets_; }

	//弾を足す
	void AddBullet(std::unique_ptr<Bullet>& Bullet);
	void GenerBullet(Vector3 BulletPos, int ID);

	/// <summary>
	/// 敵発生データ読み込み
	/// </summary>
	void LoadBulletPopData();
	void BulletReset();
	void UpdateBulletPopCommands();

	//当たり判定
	void CheckAllCollisions();


	int gameLevel_ = 0;
	int levelMax_ = 8;
	int scene = 0;

private:
	DirectXCommon* dxCommon_ = nullptr;
	

	Audio* audio = nullptr;
	SpriteCommon* spriteCommon_ = nullptr;
	Object3d* object3d = nullptr;
	Mesh* model = nullptr;
	

private://リソース
	Sprite* sprite1 = nullptr;
	Sprite* sprite2 = nullptr;
	Sprite* sprite3 = nullptr;

	Mesh* bulletModel_ = nullptr;
private:
	

	//3Dモデル
	Mesh* model_ = nullptr;
	Mesh* ico_ = nullptr;

	//レーン
	Field field_[3];

	//ゴール用
	Goal goal_;

	//csv
	std::stringstream bulletPopCommands_;

	//テクスチャ
	uint32_t testTexture_ = 0;

	//レーンデバッグ用のテクスチャ
	//uint32_t laneTexture_[3];

	

	

	//待機中フラグ
	bool isStand_ = false;
	//待機タイマー
	int standTime_ = 0;

	int gameTimer_ = 0;

};