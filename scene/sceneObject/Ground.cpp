#include "Ground.h"


Field::Field()
{
}

Field::~Field()
{

}

void Field::Initialize()
{
	fieldM = std::make_unique<Mesh>();
	fieldM = Mesh::LoadFormOBJ("alphaVerLoad", false);

	groundObj1_ = std::make_unique<Object3d>();
	groundObj1_.get()->Initialize(true);
	groundObj1_.get()->SetModel(fieldM.get());
	groundObj1_->worldTransform.translation_ = { 0,0,0 };
	groundObj1_->worldTransform.rotation_ = { 0,0,0 };
	groundObj1_->worldTransform.scale_ = { 1,1,1 };
	groundObj1_->Update();



	skydomeModel_ = std::make_unique<Mesh>();
	skydomeModel_ = Mesh::LoadFormOBJ("skybox", false);

	skydomeObj_ = std::make_unique<Object3d>();
	skydomeObj_.get()->Initialize(false);
	skydomeObj_.get()->SetModel(skydomeModel_.get());
	skydomeObj_->worldTransform.translation_ = { 1, groundHeight_,1 };
	skydomeObj_->worldTransform.scale_ = { groundScale_,groundScale_,groundScale_ };
	skydomeObj_->Update();


}

void Field::Update()
{

	groundObj1_->Update();


	skydomeObj_->Update();
}

void Field::Draw(DirectXCommon* dxcomon_)
{
	
	groundObj1_->Draw(dxcomon_->GetCommandList());

	skydomeObj_->Draw(dxcomon_->GetCommandList());

}

