#include "Ground.h"
#include "Boss.h"

Field::Field()
{
}

Field::~Field()
{
	
}

void Field::Initialize()
{
	fieldM = std::make_unique<Mesh>();
	fieldM = Mesh::LoadFormOBJ("stage",false);

	gameObject_ = std::make_unique<Object3d>();
	gameObject_.get()->Initialize(true);
	gameObject_.get()->SetModel(fieldM.get());
	gameObject_->worldTransform.translation_ = { 0,0,0 };
	gameObject_->worldTransform.scale_ = { 45,90,45 };
	gameObject_->Update();

	skydomeModel_ = std::make_unique<Mesh>();
	skydomeModel_ = Mesh::LoadFormOBJ("skydome", false);

	skydomeObj_ = std::make_unique<Object3d>();
	skydomeObj_.get()->Initialize(false);
	skydomeObj_.get()->SetModel(skydomeModel_.get());
	skydomeObj_->worldTransform.scale_ = { 4000,2000,4000 };
	skydomeObj_->Update();


}

void Field::Update()
{
	gameObject_->Update();
	skydomeObj_->Update();
}

void Field::Draw(DirectXCommon* dxcomon_)
{
	//Object3d::PreDraw(dxcomon_->GetCommandList());
	
	gameObject_->Draw(dxcomon_->GetCommandList());
	skydomeObj_->Draw(dxcomon_->GetCommandList());
	
	//Object3d::PostDraw();
}

bool Field::PlayerOnGround(Vector3 wolPos,float Obj_R) {

	{
		Vector3 field = {0,0,0};
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


bool Field::EnemyOnGround(Vector3 wolPos,float Obj_R) {
	{
		Vector3 field = {0,0,0};
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