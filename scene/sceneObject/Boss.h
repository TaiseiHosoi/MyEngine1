#pragma once

#include "DirectXCommon.h"

#include "Vector3.h"
#include "input.h"
#include "Object3d.h"

#include "BossBullet.h"

#include "BossLaser.h"
#include "BossBurstBullet.h"


#include <list>
#include <memory>

#include"FBXObject3d.h"
#include"fbx/FBXLoader.h"
#include"FBXModel.h"


//前方宣言
class Player;

#include"SphereCollider.h"
#include"CollisionManager.h"
#include"CollisionAttribute.h"

#include "Audio.h"

//前方宣言
class FbxPlayer;


enum BattleMode
{
	Ground,
	Run,
	Flatbed,
};

enum AttackValue
{
	nasi,
	FIRE,
	TACKLE,
	LASER,
	CLOSELANGE,
	TRAMPLE,
};

class Boss
{
public:
	std::unique_ptr<Object3d> bossObj_;
#pragma region 攻撃関連

	//規定値に戻るための変数
	Vector3 returnPos;
	//規定値との距離
	Vector3 returnDis;

	float returnSpeed;

	Vector3 oldPos;


	Vector3 distance = { 0,0,0 };

	Vector3 normDis = { 0,0,0 };


	float attackTimer = 180.0f;

	float coolTimer = 90.0f;

	int attackValue = AttackValue::nasi;

	int oldValue = 0;

	bool isAttack = false;

	bool isGround = false;
#pragma endregion

#pragma region 

	//oldPos
	Vector3 backPos;
	//pos-oldposのベクトル
	Vector3 bossToBack;

	float backSpeed;

	int isTackle = false;

	int tackleCount = 0;

	bool isDown = false;
	int downtimer = 180.0f;


	Vector3 playerPos;

	Vector3 tackleDis = { 0,0,0 };

	Vector3 normTdis = { 0,0,0 };

	//タックル用の壁
	Vector3 wallPos;

	float tackleSpeed = 0.0f;

	Vector3 tackleVel;

	float moveTimer = 60.0f;

	float stopTimer = 60.0f;

#pragma endregion

	float bulletTimer = 60.0f;

	int bulletNum = 0;


	int isFire = false;

	int bulletCount = 0;


	Vector3 bulletDistance = { 0,0,0 };

#pragma endregion

#pragma region


	Vector3 lPlayerPos;
	Vector3 laserPos;
	Vector3 lDistance;
	float laserTimer = 180.0f;

	float lcoolTimer = 90.0f;

	int laserCount = 0;

	bool isLaser = false;

#pragma endregion

#pragma region 

	Vector3 tramPlayerPos;

	Vector3 tramDistance;

	Vector3 JumpPos;

	int trampleCount = 0;

	float trampleSpeed = 0.5f;

	float jumpTimer = 60.0f;

	float tramChageTimer = 120.0f;

	float trampleTimer = 120.0f;

	bool isJump = false;

	bool isTrample = false;

	bool isReturn = false;

	int hp_ = 0;


#pragma endregion
	
#pragma region 

	Vector3 clDistance = { 0,0,0 };

	Vector3 clRotate = { 0,0,0 };

	Vector3 clPlayerPos = { 0,0,0 };

	Vector3 crossLangePos = { 0,0,0 };

	int   burstCount = 0;

	float bcoolTimer = 3.0f;

	float burstTimer = 180.0f;

	float clTimer = 60.0f;

	float clChargeTimer = 80.0f;

	bool  isclCharge = false;

	bool  isCross = false;

	bool  isBurst = false;

	static int hp;

#pragma endregion

#pragma region 

	bool isFAttack = false;

#pragma endregion

public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(DirectXCommon* dxcomon);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 敵の攻撃関数
	/// </summary>
	void Attack();

	void BulletUpdate();

	void BulletDraw();

#pragma region 攻撃系の関数
	/// <summary>
	/// 射撃
	/// </summary>
	void Shot();

	/// <summary>
	/// 突進
	/// </summary>
	void Tackle();

	/// <summary>
	/// レーザー
	/// </summary>
	void Laser();

	/// <summary>
	/// 踏みつけ
	/// </summary>
	void Trample();

	/// <summary>
	/// 突進＆射撃
	/// </summary>
	void CrossLange();

#pragma endregion
	/// <summary>
	/// リセット
	/// </summary>
	void Reset();

	/// <summary>
	/// 座標の取得
	/// </summary>
	Vector3 GetPosition();

	/// <summary>
	/// インスタンスの取得
	/// </summary>
	static Boss* GetInstance();

	/// <summary>
	/// プレイヤーのセッター
	/// </summary>
	void SetPlayer(FbxPlayer* player) { player_ = player; }

	//hpのセッター
	void SetHp(int hp_) { hp = hp_; }
	//hpのゲッター
	int GetHp() { return hp; }

	void Damage();

	//当たり判定が出ている時かのゲッタ
	bool GetIsAtkCollide();

	FBXObject3d* GetObject3d();

	static void minusHp(int damage);

	Vector3 KnockBack(const Vector3&position,Vector3& distance,float);

	void SetSphere(std::vector<SphereCollider*> sphere_) { sphere = sphere_; }
	std::vector<SphereCollider*> GetSphere() { return sphere; }

private:

	Input* input_ = nullptr;

	std::unique_ptr<Mesh> bossModel_;

	FbxPlayer* player_ = nullptr;

	float angle = 0.0f;

	float radian = 0.0f;

	int mode = BattleMode::Ground;

	int movePhase = 0;

private:

	DirectXCommon* dxcomon_ = nullptr;

	std::unique_ptr<FBXModel> bossFbxM_;
	std::unique_ptr<FBXObject3d> bossFbxO_;

	std::list<std::unique_ptr<BossBullet>> bullets_;

	std::list<std::unique_ptr<BossLaser>> lasers_;
	std::list<std::unique_ptr<BossBurstBullet>> bursts_;
	
	//コライダー
	int SPHERE_COLISSION_NUM;	//コライダー（スフィア）の数
	std::vector<Matrix4>* collisionBonesMat;	//当たり判定用のボーンのワールド行列
	std::vector<SphereCollider*> sphere;
	std::vector<Vector3> spherePos;
	
	//testobj
	std::unique_ptr <Mesh> ico_;
	std::vector<std::unique_ptr<Object3d>> testObj_;

	//hp
	//hpモデル
	std::unique_ptr<Object3d> hpObject_;
	std::unique_ptr<Mesh> hpModel_;

	//近距離射撃用球
	std::unique_ptr<Object3d> bulletO_;
	std::unique_ptr<Mesh> bulletM_;

	std::vector<std::unique_ptr<Object3d>> coliderPosTest_;

	//アニメーション用
	int animNum;	//アニメーション番号
	int animFlameCount;	//アニメーションのフレームカウント

	//攻撃判定が出ているかどうか
	static bool isAtkCollide;

	std::unique_ptr<Audio> audio_;
};