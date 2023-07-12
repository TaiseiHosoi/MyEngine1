#include "EnemyManager.h"
#include"MathFunc.h"


void EnemyManager::StaticInit()
{
	levelData = JsonLoader::LoadFile("testFile");

	// レベルデータからオブジェクトを生成、配置
	for (auto& objectData : levelData->objects) {
		// ファイル名から登録済みモデルを検索
		Mesh* model = nullptr;
		decltype(models)::iterator it = models.find(objectData.fileName);
		if (it != models.end()) {
			model = it->second.get();
		}

		// モデルを指定して3Dオブジェクトを生成
		std::unique_ptr<Object3d> newObject = Object3d::Create();
		newObject->SetModel(model);


		// 座標
		DirectX::XMFLOAT3 pos;
		DirectX::XMStoreFloat3(&pos, objectData.translation);
		newObject->SetPosition(Vector3(pos.x,pos.y,pos.z));

		// 回転角
		DirectX::XMFLOAT3 rot;
		DirectX::XMStoreFloat3(&rot, objectData.rotation);
		newObject->SetRotate(Vector3(rot.x,rot.y,rot.z));

		// 座標
		DirectX::XMFLOAT3 scale;
		DirectX::XMStoreFloat3(&scale, objectData.scaling);
		newObject->SetScale(Vector3(scale.x,scale.y,scale.z));

		// 配列に登録
		objects.push_back(newObject);
	}
}

void EnemyManager::AddEnemy(Enemy* enemy)
{
    enemies.push_back(enemy);
}

void EnemyManager::UpdateAllEnemies()
{
    for (Enemy* enemy : enemies) {
        enemy->Update();
    }
}

void EnemyManager::DrawAllEnemies(ID3D12GraphicsCommandList* cmdList)
{
    for (Enemy* enemy : enemies) {
        enemy->Draw(cmdList);
    }
}

void EnemyManager::DestroyAllEnemies()
{
    for (Enemy* enemy : enemies) {
        delete enemy;
    }
    enemies.clear();
}
