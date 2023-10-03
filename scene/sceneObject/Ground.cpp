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

	ImGui::Begin("field");
	ImGui::InputFloat3("Trans", &groundObj1_->worldTransform.translation_.x);
	ImGui::InputFloat3("Rot", &groundObj1_->worldTransform.rotation_.x);
	ImGui::InputFloat3("Scale", &groundObj1_->worldTransform.scale_.x);
	ImGui::End();

	skydomeObj_->Update();
	/*ImGui::Begin("gRotY");
	ImGui::InputFloat("rocalRoty", &groundObj1_->worldTransform.rotation_.y);
	ImGui::InputFloat("matRoty", &groundObj1_->worldTransform.matWorld_.m[0][2]);
	ImGui::End();*/
}

void Field::Draw(DirectXCommon* dxcomon_)
{
	//Object3d::PreDraw(dxcomon_->GetCommandList());

	groundObj1_->Draw(dxcomon_->GetCommandList());
	//groundObj2_->Draw(dxcomon_->GetCommandList());

	skydomeObj_->Draw(dxcomon_->GetCommandList());

	//Object3d::PostDraw();
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