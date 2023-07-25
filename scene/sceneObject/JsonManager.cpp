﻿#include "JsonManager.h"
#include"MathFunc.h"


void JsonManager::StaticInit()
{

	//モデルインクルード
	// モデル読み込み
	modelMoai = Mesh::LoadFormOBJ("moai", true);
	modelCube = Mesh::LoadFormOBJ("cube", true);
	modelRoad1 = Mesh::LoadFormOBJ("road1", true);
	modelCam = Mesh::LoadFormOBJ("cube", true);
	
	//モデルインサート
	models.insert(std::make_pair("moai", modelMoai.get()));
	models.insert(std::make_pair("Cube", modelCube.get()));
	models.insert(std::make_pair("road1", modelRoad1.get()));
	models.insert(std::make_pair("cam", modelCam.get()));
	

	levelData = JsonLoader::LoadFile("testScene");

	// レベルデータからオブジェクトを生成、配置
	for (auto& objectData : levelData->objects) {
		// ファイル名から登録済みモデルを検索
		Mesh* model = nullptr;
		decltype(models)::iterator it = models.find(objectData.fileName);
		if (it != models.end()) {
			model = it->second;
		}

		// モデルを指定して3Dオブジェクトを生成
		Object3d newObject;
		newObject.Initialize(true);
		newObject.SetModel(model);


		// 座標
		DirectX::XMFLOAT3 pos;
		DirectX::XMStoreFloat3(&pos, objectData.translation);
		newObject.SetPosition(Vector3(pos.x,pos.y,pos.z));

		// 回転角
		DirectX::XMFLOAT3 rot;
		DirectX::XMStoreFloat3(&rot, objectData.rotation);
		newObject.SetRotate(Vector3(rot.x,rot.y,rot.z));

		// 座標
		DirectX::XMFLOAT3 scale;
		DirectX::XMStoreFloat3(&scale, objectData.scaling);
		newObject.SetScale(Vector3(scale.x,scale.y,scale.z));

		if (objectData.fileName == "cam") {
			camObjs.push_back(newObject);
			continue;
		}
		// 配列に登録
		objects.push_back(newObject);
	}
}

void JsonManager::AddEnemy(Enemy* enemy)
{
    enemies.push_back(enemy);
}

void JsonManager::UpdateAllEnemies()
{
    /*for (Enemy* enemy : enemies) {
        enemy->Update();
    }*/

	for (int i = 0; i < objects.size();i++) {
		objects[i].Update();
	}
	for (int i = 0; i < camObjs.size(); i++) {
		camObjs[i].Update();
	}
}

void JsonManager::DrawAllEnemies(ID3D12GraphicsCommandList* cmdList)
{
    /*for (Enemy* enemy : enemies) {
        enemy->Draw(cmdList);
    }*/

	for (int i = 0; i < objects.size(); i++) {
		objects[i].Draw(cmdList);
	}

	for (int i = 0; i < camObjs.size(); i++) {
		camObjs[i].Draw(cmdList);
	}
}

void JsonManager::DestroyAllEnemies()
{
    /*for (Enemy* enemy : enemies) {
        delete enemy;
    }
    enemies.clear();*/
}