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

enum DEAD_ACT_NUM {
	none,
	crash,
	bomb,
	disappear
};

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
	void CreateBulHitParticle(Vector3 posArg);

	//ダメージ
	static void minusHp(int damage);

	//移動処理
	void Move();

	//射撃
	void BulletShot();

	//死亡時アクション
	void CrashAction();
	void BombAction();

public:	//アクセッサ

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

	//死亡時演出のフェーズゲッタ
	int GetIsDeadActNum();

	void SetIsDeadActNum(int arg);

	void PlayerPalamReset();


private:	
	


	//コライダー処理
	void PColliderUpdate();

	//ガード成功時
	bool GetGuardExcute() { return isGuard; }




public:
	//自機の向いている方向のゲッタ
	Vector3 GetNowFaceAngle();

	//レールカメラ情報セッタ
	void SetRailCameraInfo(RailCameraInfo* info);

	//ゲームオブジェクトの定数バッファゲッタ
	ID3D12Resource* GetConstBuff() { return gameObject_->GetConstBuff(); };

	//ゲームオーバー用
	void GoGameOver();

	//無敵
	void Invincible();

	//点滅処理
	void Blink(float elapsedTime, float blinkDuration, float blinkInterval, float& currentAlpha);



private:

	//入力
	Input* input_ = nullptr;

	//オブジェクト
	std::unique_ptr<FBXObject3d> gameObject_;
	std::unique_ptr<Object3d> hoverCarObject_;
	std::unique_ptr<Mesh> hoverCarModel_;

	int SPHERE_COLISSION_NUM;	//コライダー（スフィア）の数
	std::vector<Matrix4>* collisionBonesMat;	//当たり判定用のボーンのワールド行列
	std::vector<SphereCollider*> sphere;
	std::vector<Vector3> spherePos = {};

	PlayerReticle reticle_;

	//hpモデル
	std::unique_ptr<Object3d> hpObject_;
	std::unique_ptr<Mesh> hpModel_;
	std::unique_ptr<ParticleManager> hitParticle_;
	std::unique_ptr<ParticleManager> bombParticle_;
	//テスト用
	std::vector<std::unique_ptr<Object3d>> coliderPosTest_;

	std::unique_ptr <Audio> audio_;

	
#pragma region 移動処理で使う変数
	Vector3 velocity_;
	const float kMoveSpeed_ = 0.4f;//移動速度
	const float kTurnSpeed_ = MathFunc::Dig2Rad(10);//旋回速度
	bool isRun_ = false;
	Vector3 faceAngle_ = {};
	const float maxFaceAngle_ = 0.02f;
	const float faceMaxAngleY_ = 0.4f; //自機回転の最大
	const float faceMaxAngleX_ = 0.3f; //自機回転の最大
	
	float faceRotSpeedY_ = 0.05f;
	float faceRotSpeedX_ = 0.02f;
	float returnRotSpeed_ = 0.03f;

	Matrix4 pAngleMat = {};//自機の移動用Matrix
	Vector3 nowPos = {};
	float maxParallelMovement_ = 12.f;
#pragma endregion 移動処理で使う変数

#pragma region 射撃処理変数

	//ホーミング弾
	std::vector< std::unique_ptr<PlayerHomingBullet>> homingBullets_;
	std::unique_ptr<Mesh> bulletModel_;

	struct PRockTarget {	// ターゲット構造体
		WorldTransform* targetWtfPtr;
		bool isRockOn;
	};
	std::vector<PRockTarget> rockTargets_;

	//連射弾
	std::list< std::unique_ptr<PlayerRapidBullet>> rapidBullets_;
	const int shotDelay_ = 5;
	int nowShotDelayCount_ = 0;

	//ヒット時パーティクルのサイズ
	const int maxHitParticleLife_ = 10;
	const float startHitParticleSize_ = 0.7f;
	const float endHitParticleSize_ = 0.1f;
	

#pragma endregion 射撃処理変数


	//カメラの向き
	Vector3 cameraAngle_ = {0 , 0 , 0};
	RailCameraInfo* railCameraInfo_ = nullptr;
	//ブレーキアニメーション用フラグ
	bool isbrake = false;
	int brakeFlameCount = 0;
	//アニメーション何番目か
	int animCT = 0;
	int oldAnimCT = 0;
	//攻撃フラグ
	bool isAtk = false;



	//当たり判定外部参照用
	static bool isAtkCollide;
	static bool isGuardCollide;

	//防御時行動
	bool isGuard = false;	//ガードをするかしないか
	bool isGuardExcute = false;	//ガード成功時用

	//反撃フラグオン→反撃遷移モーション→反撃→反撃フラグオフ
	bool isCounter = false;	//反撃フラグ

	

	//ヒットポイント
	int hitDeley = 0;	//何フレーム連続で当たるか
	static int hp;
	const int maxHp_ = 100;
	bool isHitStop = false;
	//被弾関連
	bool isInvincible_ = false;
	bool isHitAction_ = false;
	int hitActFlameCount_ = 0;
	const int maxHitActFlameCount_ = 120;
	const float minHitBlinkingAlpha_ = 0.1f;
	const float maxHitBlinkingAlpha_ = 0.1f;

	//生死フラグ
	bool isDead_ = false;
	const int maxCrashActCount_ = 120;
	int deadActCount_ = 0;
	int oldDeadActNum_ = 0;
	int deadActNum_ = 0;
	Vector3 bombStartPos_ = {};
	Vector3 bombStartRot_ = {};
	const Vector3 bombPosVel_ = { 0.01f,0.6f,0.01f };	//吹っ飛び時のベロシティ
	const Vector3 bombRotateVel_ = { 0.1f,0.1f,0.2f };	//吹っ飛び時回転量

	 
	//アニメーション
	int oldPActNum_ = 0;	//アクション前フレーム保存変数
	int count = 0;

	//固定値
	const int delayCount_ = 4;
	const int damage_ = 2;
};