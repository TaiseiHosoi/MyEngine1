#include "GameScene.h"


GameScene::GameScene()
{
}

GameScene::~GameScene()
{
}

void GameScene::Initialize(DirectXCommon* dxcomon)
{
	assert(dxcomon);
	dxCommon_ = dxcomon;

	//スプライト初期化
	spriteCommon_ = new SpriteCommon;
	spriteCommon_->Initialize(dxCommon_);
	spriteCommon_->LoadTexture(1, "mario.png");
	spriteCommon_->LoadTexture(2, "reimu.png");


	sprite1 = new Sprite();
	sprite1->Initialize(spriteCommon_, 1);
	sprite2 = new Sprite();
	sprite2->Initialize(spriteCommon_, 2);


	sprite1->SetSize({ 200,200 });
	sprite2->SetSize({ 200,200 });
	sprite1->SetPozition({ 100,100 });
	sprite2->SetPozition({ 100,200 });
	audio = new Audio();
	audio->Initialize();


	model = Mesh::LoadFromOBJ("cube");
	object3d = Object3d::Create();
	object3d->SetModel(model);

}

void GameScene::Update()
{
	object3d->Update();
}

void GameScene::Draw()
{
	spriteCommon_->SpritePreDraw();

	sprite1->Draw();
	sprite2->Draw();

	spriteCommon_->SpritePostDraw();

	Object3d::PreDraw(dxCommon_->GetCommandList());

	object3d->Draw();

	Object3d::PostDraw();

}
