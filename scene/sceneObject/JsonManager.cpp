#include "JsonManager.h"
#include"MathFunc.h"


JsonManager::~JsonManager()
{
}

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
		else if (objectData.fileName == "moai") {
			moaiObjs.push_back(newObject);
			EnemyState newState;
			newState.hp_ = 1;
			newState.isAlive_ = true;
			newState.isAtk_ = false;
			moaiState.push_back(newState);

			moaiSpCollider.push_back(new SphereCollider);
			int nowSphereNum = static_cast<int>(moaiSpCollider.size()-1);
			CollisionManager::GetInstance()->AddCollider(moaiSpCollider[nowSphereNum]);
			moaiSpCollider[nowSphereNum]->SetBasisPos(&moaiObjs[moaiObjs.size()-1].worldTransform.translation_);
			moaiSpCollider[nowSphereNum]->SetRadius(2.0f);
			moaiSpCollider[nowSphereNum]->SetAttribute(COLLISION_ATTR_ENEMIES);
			moaiSpCollider[nowSphereNum]->Update();
			continue;
		}
		// 配列に登録
		objects.push_back(newObject);
	}
}

//void JsonManager::AddEnemy(Enemy* enemy)
//{
//    enemies.push_back(enemy);
//}

void JsonManager::UpdateAllObjects()
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

	moaiDigRot++;
	if (moaiDigRot >= 360) {
		moaiDigRot = 0;
	}
	for (int i = 0; i < moaiObjs.size(); i++) {
		float moveSpeed = 0.01f;

		
		moaiObjs[i].worldTransform.translation_.x += cosf(moaiDigRot * 3.14f/180.f);
		moaiObjs[i].worldTransform.translation_.y += sinf(moaiDigRot * 3.14f / 180.f);
		moaiObjs[i].Update();

		moaiSpCollider[i]->Update();
		if (moaiSpCollider[i]->GetIsHit() == true) {
			if (moaiSpCollider[i]->GetCollisionInfo().collider->GetAttribute() == COLLISION_ATTR_ALLIES) {
				moaiState[i].hp_--;
			}
		}
		if (moaiState[i].hp_ <= 0) {
			moaiState[i].isAlive_ = false;
		}

		if (moaiState[i].isAlive_ == false) {	//死去
			moaiObjs.erase(std::cbegin(moaiObjs) + i);
			moaiSpCollider.erase(std::cbegin(moaiSpCollider) + i);
			moaiState.erase(std::cbegin(moaiState) + i);
		}
		
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

	//for (int i = 0; i < camObjs.size(); i++) {
	//	camObjs[i].Draw(cmdList);
	//}
	
	for (int i = 0; i < moaiObjs.size(); i++) {
		moaiObjs[i].Draw(cmdList);
	}
}

void JsonManager::DestroyAllEnemies()
{
    /*for (Enemy* enemy : enemies) {
        delete enemy;
    }
    enemies.clear();*/
}
