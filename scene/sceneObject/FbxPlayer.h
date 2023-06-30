#pragma once
#include "Object3d.h"
#include "FBXObject3d.h"
#include "FBXModel.h"
#include "input.h"
#include"SphereCollider.h"
#include"CollisionManager.h"
#include"CollisionAttribute.h"
#include"ParticleManager.h"
#include"PlayerActionManager.h"

#include "audio.h"



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

	//ダメージ
	static void minusHp(int damage);

	FBXObject3d* GetObject3d();

	//デスフラグのSetter,Getter
	void SetIsDead(bool isDead) { isDead_ = isDead; }
	bool GetIsDead() { return isDead_; }

	//スタティック用の変数アクセッサ
	//攻撃に当たり判定が発生しているかのアクセッサ
	static bool GetIsAtkCollide();
	static void SetIsAtkCollide(bool isAtkCollide);
	//ガードの判定が出ているかのアクセッサ
	static bool GetIsGuardCollide();
	static void SetIsGuardCollide(bool isGuardCollide);
	//hpアクセッサ
	static int GetHp();	
	static void SetHp(int hp);	
	


private:	
	
	std::unique_ptr <PlayerActionManager> pActManager_;

	//コライダー処理
	void PColliderUpdate();

	bool GetGuardExcute() { return isGuard; }


public:
	Vector3 GetNowFaceAngle();

	ID3D12Resource* GetConstBuff() { return gameObject_->GetConstBuff(); };

	PlayerActionManager* GetPlayerActionManager() { return pActManager_.get(); };

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

	//hpモデル
	std::unique_ptr<Object3d> hpObject_;
	std::unique_ptr<Mesh> hpModel_;
	std::unique_ptr<ParticleManager> particle_;
	//テスト用
	std::vector<std::unique_ptr<Object3d>> coliderPosTest_;

	std::unique_ptr <Audio> audio_;

	//現在の行動
	

	//移動速度
	const float kMoveSpeed_ = 0.25f;
	//旋回速度
	const float kTurnSpeed_ = MathFunc::Dig2Rad(10);
	//移動ベクトル
	Vector3 velocity_;
	//自機の向き
	Vector3 faceAngle_ = {0 , 0 , 0};
	//カメラの向き
	Vector3 cameraAngle_ = {0 , 0 , 0};
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

	bool isHitStop = false;

	bool isDead_ = false;
	int oldPActNum_ = 0;	//アクション前フレーム保存変数
	
	int count = 0;
};