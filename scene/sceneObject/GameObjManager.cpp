#include "GameObjManager.h"
#include"MathFunc.h"


GameObjManager::GameObjManager()
{
}

GameObjManager::~GameObjManager()
{
	
}

void GameObjManager::StaticInit()
{

	//モデルインクルード
	// モデル読み込み
	modelMoai_ = Mesh::LoadFormOBJ("moai", true);
	modelCube_ = Mesh::LoadFormOBJ("cube", true);
	modelRoad1_ = Mesh::LoadFormOBJ("alphaVerLoad", true);
	modelCam_ = Mesh::LoadFormOBJ("cube", true); 
	modelWalkRobo_ = Mesh::LoadFormOBJ("walkEnemy",true);
	modelBill1_ = Mesh::LoadFormOBJ("bill1", true);
	modelTower1_ = Mesh::LoadFormOBJ("tower1", true);
	//モデルインサート
	models_.insert(std::make_pair("moai", modelMoai_.get()));
	models_.insert(std::make_pair("Cube", modelCube_.get()));
	models_.insert(std::make_pair("road1", modelRoad1_.get()));
	models_.insert(std::make_pair("cam", modelCam_.get()));
	models_.insert(std::make_pair("bill1", modelBill1_.get()));
	models_.insert(std::make_pair("tower1", modelTower1_.get()));
	
	levelData = JsonLoader::LoadFile("testScene");

	// レベルデータからオブジェクトを生成、配置
	for (auto& objectData : levelData->objects_) {
		// ファイル名から登録済みモデルを検索
		Mesh* model = nullptr;
		decltype(models_)::iterator it = models_.find(objectData.fileName);
		if (it != models_.end()) {
			model = it->second;
		}
		
		// 建物を一時的に消去
		if (objectData.fileName == "bill1" || objectData.fileName == "tower1") {
			continue;
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

		if (objectData.fileName == "camera") {
			camObjs_.push_back(newObject);
			continue;
		}
		else if (objectData.fileName == "moai") {
			moaiObjs_.push_back(newObject);
			moaiObjs_.back().SetScale({3,3,3});
			EnemyState newState;
			newState.hp_ = 1;
			newState.isDead_ = false;
			newState.isAtk_ = false;
			moaiState_.push_back(newState);

			moaiSpCollider_.push_back(new SphereCollider);
			int nowSphereNum = static_cast<int>(moaiSpCollider_.size()-1);
			CollisionManager::GetInstance()->AddCollider(moaiSpCollider_[nowSphereNum]);
			moaiSpCollider_[nowSphereNum]->SetBasisPos(&moaiObjs_[moaiObjs_.size()-1].worldTransform.translation_);
			moaiSpCollider_[nowSphereNum]->center = moaiObjs_[moaiObjs_.size() - 1].worldTransform.translation_;
			moaiSpCollider_[nowSphereNum]->SetRadius(scale.x);
			moaiSpCollider_[nowSphereNum]->SetAttribute(COLLISION_ATTR_ENEMIES);
			moaiSpCollider_[nowSphereNum]->Update();
			continue;
		}
		// 配列に登録
		objects_.push_back(newObject);
	}

#pragma region ポップデータ読み込み
	ResetCommands("Resources/enemyPop2.csv", walkingEnemyPopCommands_);
	gameTime_ = 0;
	standTime_ = 0;
	isStand_ = false;
#pragma endregion ポップデータ読み込み


}

void GameObjManager::AddEnemy(int enemyNum, int popTime,Vector3 offsetPos)
{
	static_cast<void>(popTime);

	if (enemyNum == ENEMY_NUM::WALKING_ENEMY) {
		//オブジェクト処理
		std::unique_ptr<WalkingEnemy> newWalkingEnemy;
		newWalkingEnemy = std::make_unique<WalkingEnemy>();
		walkingEnemies.push_back(std::move(newWalkingEnemy));
		walkingEnemies.back()->Initialize(modelWalkRobo_.get());
		walkingEnemies.back()->SetOffsetVec3(offsetPos);
		walkingEnemies.back()->SetRailCameraInfo(railCameraInfo_);
		walkingEnemies.back()->SetPlayerWorldTransform(playerWorldTF_);

	}
}





void GameObjManager::UpdateAllObjects()
{
	// Listのremove
	walkingEnemies.remove_if([](std::unique_ptr<WalkingEnemy>& enemy) {
		return enemy->GetState()->isDead_;
		});

	UpdateWalkingEnemyPopCommands();

	for (int i = 0; i < objects_.size();i++) {
		objects_[i].Update();
	}
	for (int i = 0; i < camObjs_.size(); i++) {
		camObjs_[i].Update();

	}


	moaiDigRot_++;
	if (moaiDigRot_ >= 360) {
		moaiDigRot_ = 0;
	}
	for (int i = 0; i < moaiObjs_.size(); i++) {
		float moveSpeed = 0.1f;

		
		moaiObjs_[i].worldTransform.translation_.x += cosf(moaiDigRot_ * 3.14f/180.f) * moveSpeed;
		moaiObjs_[i].worldTransform.translation_.y += sinf(moaiDigRot_ * 3.14f / 180.f) * moveSpeed;

		moaiObjs_[i].Update();

		moaiSpCollider_[i]->SetBasisPos(&moaiObjs_[i].worldTransform.translation_);

		
		if (moaiSpCollider_[i]->GetIsHit() == true) {
			if (moaiSpCollider_[i]->GetCollisionInfo().collider_->GetAttribute() == COLLISION_ATTR_ALLIES) {
				moaiState_[i].hp_--;
			}
		}
		if (moaiState_[i].hp_ <= 0) {
			moaiState_[i].isDead_ = true;
			moaiSpCollider_[i]->RemoveAttribute(8);
			
		}

		if (moaiState_[i].isDead_ == true) {	//死去
			//moaiObjs.erase(std::remove_if(moaiObjs.begin(), moaiObjs.end(),
			//	[](const EnemyState& state) {
			//		return state.isDead_;
			//	}),
			//	moaiObjs.end());
			////moaiSpCollider.erase(std::cbegin(moaiSpCollider) + i);
			//moaiState.erase(std::cbegin(moaiState) + i);
		}
		moaiSpCollider_[i]->Update();
		
	}

	//歩兵更新
	for (const unique_ptr<WalkingEnemy>& enemy : walkingEnemies) {
		enemy->Update();
	}


	/*ImGui::Begin("objects");
	for (int i = 0; i < objects.size(); i++) {
		Vector3 monitT = { objects[i].GetMatWorld().m[3][0], objects[i].GetMatWorld().m[3][1],objects[i].GetMatWorld().m[3][2] };
		Vector3 monitR = { objects[i].worldTransform.rotation_.x, objects[i].worldTransform.rotation_.y,objects[i].worldTransform.rotation_.z };
		Vector3 monitS = { objects[i].worldTransform.scale_.x, objects[i].worldTransform.scale_.y,objects[i].worldTransform.scale_.z };
		ImGui::InputFloat3("Trans", &monitT.x);
		ImGui::InputFloat3("Rot",&monitR.x);
		ImGui::InputFloat3("Scale", &monitS.x);

	}
	ImGui::End();*/
	

}

void GameObjManager::DrawAllEnemies(ID3D12GraphicsCommandList* cmdList)
{

	for (const unique_ptr<WalkingEnemy>& enemy : walkingEnemies) {
		enemy->Draw(cmdList);
	}


	for (int i = 0; i < objects_.size(); i++) {
		objects_[i].Draw(cmdList);
	}
	
	for (int i = 0; i < moaiObjs_.size(); i++) {
		if (moaiState_[i].hp_ > 0) {
			moaiObjs_[i].Draw(cmdList);
		}
	}

	
}

void GameObjManager::DestroyAllEnemies()
{
    //for (Enemy* enemy : enemies) {
    //    delete enemy;
    //}
    //enemies.clear();
}

void GameObjManager::LoadData(const char* filename, std::stringstream& stream)
{
	//ファイルを開く
	std::ifstream file;
	file.open(filename);

	assert(file.is_open());

	//ファイルの内容を文字列ストリームにコピー
	stream << file.rdbuf();

	//ファイルを閉じる
	file.close();
}

void GameObjManager::ResetCommands(const char* filename, std::stringstream& stream)
{
	stream.str("");
	stream.clear(std::stringstream::goodbit);
	LoadData(filename, stream);

}

void GameObjManager::UpdateWalkingEnemyPopCommands()
{
	gameTime_++;	//毎f処理
	//待機処理
	if (isStand_) {
		standTime_--;
		if (standTime_ <= 0) {
			//待機完了
			isStand_ = false;
		}
		return;
	}
	// 1行分の文字列を入れる変数
	std::string line;

	//コマンド実行ループ
	while (getline(walkingEnemyPopCommands_, line)) {
		// 1行分の文字数をストリームに変換して解折しやすくなる
		std::istringstream line_stream(line);

		std::string word;
		//,区切りで行の先頭文字を取得
		getline(line_stream, word, ',');

		//"//"から始まる行はコメント
		if (word.find("//") == 0) {
			//コメント行を飛ばす
			continue;
		}
		// POPコマンド
		if (word.find("POP") == 0) {

			//レーン
			std::getline(line_stream, word, ',');
			int lane = static_cast<int>(std::atof(word.c_str()));

			// ID
			std::getline(line_stream, word, ',');
			//int ID = static_cast<int>(std::atof(word.c_str()));

			

			
			if (lane == 1) {
				//offset
				Vector3 offset = { -10,0,0 };
				AddEnemy(0,0,offset);
			}
			else if (lane == 2) {
				//offset
				Vector3 offset = { 0,0,0 };
				AddEnemy(0, 0, offset);
			}
			else if (lane == 3) {
				//offset
				Vector3 offset = { 10,0,0 };
				AddEnemy(0, 0, offset);
			}
			else {
				//offset
				Vector3 offset = { 0,0,0 };
				AddEnemy(0, 0, offset);
			}
		}
		// WAITコマンド
		else if (word.find("WAIT") == 0) {
			std::getline(line_stream, word, ',');

			//待ち時間
			int32_t waitTime = std::atoi(word.c_str());

			//待機開始
			isStand_ = true;
			standTime_ = waitTime;

			//抜ける
			break;
		}
	}
}
