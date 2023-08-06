#include "WorldTransform.h"

WorldTransform::WorldTransform(Vector3 scale, Vector3 rotation,Vector3 translation)
{
	this->scale_ = scale;
	this->rotation_ = rotation;
	this->translation_ = translation;
	
}

void WorldTransform::initialize() {

	scale_ = {1 , 1 , 1};
	rotation_ = {0 , 0 , 0};
	translation_ = {0 , 0 , 0};
	matWorld_.identity();

	UpdateMatWorld();

}

//ワールド変換行列の更新
void WorldTransform::UpdateMatWorld() {
    // ワールド変換行列を用意
    Matrix4 matScale, matRot, matTrans;

    // 各軸の回転をクオータニオンとして作成
    Quaternion qRotZ(sinf(rotation_.x / 2), cosf(rotation_.x / 2), 0, 0);
    Quaternion qRotY(cosf(-rotation_.y / 2), 0, sinf(-rotation_.y / 2), 0);
    Quaternion qRotX(cosf(rotation_.z / 2), 0, 0, sinf(rotation_.z / 2));

    // 各軸のクオータニオンを合成
    Quaternion qRot = qRot.Multiply(qRotZ, qRot.Multiply(qRotY, qRotX));

    // スケーリング行列を作成
    matScale = MathFunc::Scale(scale_);

    // 回転行列を作成
    //matRot = qRot.ToMatrix4(qRot);
    matRot = MathFunc::Rotation(rotation_, 6);

    // 平行移動行列を作成
    matTrans = MathFunc::Move(translation_);

    // ワールド行列に単位行列を代入
    matWorld_.identity();

    // 行列の計算
    matWorld_ *= matScale;
    matWorld_ *= matRot;
    matWorld_ *= matTrans;

    // もし親がある場合、親のワールド行列との計算を行う
    if (parent_) {
        matWorld_ *= parent_->matWorld_;
    }
}