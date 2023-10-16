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
	skydomeObj_->worldTransform.translation_ = { 1,-5000,1 };
	skydomeObj_->worldTransform.scale_ = { 100,100,100 };
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

bool Field::PlayerOnGround(Vector3 wolPos, float Obj_R) {

	{
		Vector3 field = { 0,0,0 };
		Vector3 world = wolPos;
		float xz = std::pow(field.x - world.x, 2.0f) + std::pow(field.z - world.z, 2.0f);
		float lenR = std::pow(Obj_R + R, 2.0f);
		if (xz <= lenR) {
			return false;
		}
		else {
			return true;
		}
	}

}


bool Field::EnemyOnGround(Vector3 wolPos, float Obj_R) {
	{
		Vector3 field = { 0,0,0 };
		Vector3 world = wolPos;
		float xz = std::pow(field.x - world.x, 2.0f) + std::pow(field.z - world.z, 2.0f);
		float lenR = std::pow(Obj_R + BR, 2.0f);
		if (xz <= lenR) {
			return false;
		}
		else {
			return true;
		}
	}

}