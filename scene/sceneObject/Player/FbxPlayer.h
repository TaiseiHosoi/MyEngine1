/**
 * @file FbxPlayer.h
 * @brief 自機の基本クラス
 */
#pragma once
#include "Object3d.h"
#include "FBXObject3d.h"
#include "FBXModel.h"
#include "input.h"
#include"SphereCollider.h"
#include"CollisionManager.h"
#include"ParticleManager.h"

#include "audio.h"

#include "PlayerReticle.h"
#include"PlayerHomingBullet.h"
#include"PlayerRapidBullet.h"



class FbxPlayer
{
public:
	//コンストラクタ
	FbxPlayer();

	//デストラクタ
	~FbxPlayer();

	//初期化
	void Initialize(FBXModel* model);

	//更新
	void Update();

	//描画
	void Draw(ID3D12GraphicsCommandList* cmdList);

	//パーティクル発生
	void CreateParticle();

	//ダメージ
	static void minusHp(int damage);

	//移動処理
	void Move();

	//射撃
	void BulletShot();

	// 自機Object3d変数のゲッタ
	FBXObject3d* GetObject3d();

	//デスフラグのSetter
	void SetIsDead(bool isDead) { isDead_ = isDead; }

	//デスフラグのGetter
	bool GetIsDead() { return isDead_; }

	//スタティック用の変数アクセッサ
	//攻撃に当たり判定が発生しているかのアクセッサ
	static bool GetIsAtkCollide();

	//攻撃に当たり判定が発生しているかのアクセッサ
	static void SetIsAtkCollide(bool isAtkCollide);

	//ガードの判定が出ているかのアクセッサ
	static bool GetIsGuardCollide();

	//ガードの判定が出ているかのアクセッサ
	static void SetIsGuardCollide(bool isGuardCollide);

	//hpアクセッサ
	static int GetHp();	

	//hpアクセッサ
	static void SetHp(int hp);	
	


private:	
	


	//コライダー処理
	void PColliderUpdate();

	//ガード成功時
	bool GetGuardExcute() { return isGuard_; }


public:
	//自機の向いている方向のゲッタ
	Vector3 GetNowFaceAngle();

	//レールカメラ情報セッタ
	void SetRailCameraInfo(RailCameraInfo* info);

	//ゲームオブジェクトの定数バッファゲッタ
	ID3D12Resource* GetConstBuff() { return gameObject_->GetConstBuff(); };

private:

	//入力
	Input* input_ = nullptr;

	//オブジェクト
	std::unique_ptr<FBXObject3d> gameObject_;
	std::unique_ptr<Object3d> hoverCarObject_;
	std::unique_ptr<Mesh> hoverCarModel_;

	int SPHERE_COLISSION_NUM;	//コライダー（スフィア）の数
	std::vector<Matrix4>* collisionBonesMat_;	//当たり判定用のボーンのワールド行列
	std::vector<SphereCollider*> sphere_;
	std::vector<Vector3> spherePos_ = {};

	PlayerReticle reticle_;

	//hpモデル
	std::unique_ptr<Object3d> hpObject_;
	std::unique_ptr<Mesh> hpModel_;
	std::unique_ptr<ParticleManager> particle_;
	//テスト用
	std::vector<std::unique_ptr<Object3d>> coliderPosTest_;

	std::unique_ptr <Audio> audio_;

	
#pragma region 移動処理で使う変数
	Vector3 velocity_;
	const float kMoveSpeed_ = 0.4f;//移動速度
	const float kTurnSpeed_ = MathFunc::Dig2Rad(10);//旋回速度
	bool isRun_ = false;
	Vector3 faceAngle_ = {};
	const float faceMaxAngle_ = 0.4f; //自機回転の最大
	float faceRotSpeed_ = 0.05f;
	float returnRotSpeed_ = 0.03f;
	Matrix4 pAngleMat_ = {};//自機の移動用Matrix
	Vector3 nowPos_ = {};
#pragma endregion 移動処理で使う変数

#pragma region 射撃処理変数

	//ホーミング弾
	std::vector< std::unique_ptr<PlayerHomingBullet>> homingBullets_;
	std::unique_ptr<Mesh> bulletModel_;

	struct PRockTarget {	// ターゲット構造体
		WorldTransform* targetWtfPtr_;
		bool isRockOn_;
	};
	std::vector<PRockTarget> rockTargets_;

	//連射弾
	std::list< std::unique_ptr<PlayerRapidBullet>> rapidBullets_;
	

#pragma endregion 射撃処理変数


	//カメラの向き
	Vector3 cameraAngle_ = {0 , 0 , 0};
	RailCameraInfo* railCameraInfo_ = nullptr;
	//ブレーキアニメーション用フラグ
	bool isBrake_ = false;
	int brakeFlameCount_ = 0;
	//アニメーション何番目か
	int animCT_ = 0;
	int oldAnimCT_ = 0;
	//攻撃フラグ
	bool isAtk_ = false;

	//当たり判定外部参照用
	static bool isAtkCollide_;
	static bool isGuardCollide_;

	//防御時行動
	bool isGuard_ = false;	//ガードをするかしないか
	bool isGuardExcute_ = false;	//ガード成功時用

	//反撃フラグオン→反撃遷移モーション→反撃→反撃フラグオフ
	bool isCounter_ = false;	//反撃フラグ

	

	//ヒットポイント
	int hitDeley_ = 0;	//何フレーム連続で当たるか
	static int hp_;
	bool isHitStop_ = false;
	bool isDead_ = false;
	//アニメーション
	int oldPActNum_ = 0;	//アクション前フレーム保存変数
	int count_ = 0;
};