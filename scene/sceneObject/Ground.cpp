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
	

	continuousFloorModel_ = std::make_unique<Mesh>();
	continuousFloorModel_ = Mesh::LoadFormOBJ("ground", false);

	groundObj1_ = std::make_unique<Object3d>();
	groundObj1_.get()->Initialize(true);
	groundObj1_.get()->SetModel(fieldM.get());
	groundObj1_->worldTransform.translation_ = { 0,0,0 };
	groundObj1_->worldTransform.rotation_ = { 0,0,0 };
	groundObj1_->worldTransform.scale_ = { 1,1,1 };
	groundObj1_->Update();

	continuousFloor_ = std::make_unique<Object3d>();
	continuousFloor_.get()->Initialize(true);
	continuousFloor_.get()->SetModel(continuousFloorModel_.get());
	continuousFloor_->worldTransform.translation_ = { 0,groundAdjustPosY_,0 };
	continuousFloor_->worldTransform.rotation_ = { 0,0,0 };
	continuousFloor_->worldTransform.scale_ = { groundAdjustScale_,1,groundAdjustScale_ };
	continuousFloor_->Update();



	skydomeModel_ = std::make_unique<Mesh>();
	skydomeModel_ = Mesh::LoadFormOBJ("backBuild", false);

	skydomeObj_ = std::make_unique<Object3d>();
	skydomeObj_.get()->Initialize(false);
	skydomeObj_.get()->SetModel(skydomeModel_.get());

	skydomeObj_->worldTransform.translation_ = { 0,backGroundAdjustPosY_,0 };
	skydomeObj_->worldTransform.scale_ = { groundAdjustScale_,groundAdjustScale_,groundAdjustScale_ };



	skydomeObj_->Update();


}
void Field::Update()
{
	

	groundObj1_->Update();

	continuousFloor_->Update();

	skydomeObj_->Update();
}

void Field::Draw(DirectXCommon* dxcomon_)
{
	
	groundObj1_->Draw(dxcomon_->GetCommandList());

	continuousFloor_->Draw(dxcomon_->GetCommandList());



	skydomeObj_->Draw(dxcomon_->GetCommandList());

}


