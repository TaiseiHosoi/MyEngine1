/**
 * @file SplineCurve.h
 * @brief スプライン曲線クラス
 */
#pragma once
#include <iostream>
#include <vector>
#include <cmath>
#include"Vector3.h"

class SplineCurve {
public:
    //コンストラクタ
    SplineCurve(const std::vector<Vector3>& controlPoints);

    //現在の位置取得
    Vector3 getCurrentPosition() const;

    //更新
    void update(float deltaTime);

    //現在の進行度（0.0から1.0の範囲）
    float getCurrentProgress() const;

    //現在通過しているコントロールポイントのインデックス取得
    int getCurrentPointIndex() const;

private:
    std::vector<Vector3> controlPoints; //等間隔に配置されたコントロールポイントの配列
    int numControlPoints; //コントロールポイントの数
    float currentProgress; //現在の進行度
    int currentPointIndex; //現在通過しているコントロールポイントのインデックス
};

