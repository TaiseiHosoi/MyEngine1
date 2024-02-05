#include "GameObjManager.h"
#include"MathFunc.h"


GameObjManager::GameObjManager()
{
}

GameObjManager::~GameObjManager()
{
	
}

void GameObjManager::DeleteWalkingEnemy()
{
	// Listのremove
	walkingEnemies.remove_if([](std::unique_ptr<WalkingEnemy>& enemy) {
		return enemy->compultionTrue();
		});
}

void GameObjManager::StaticInit()
{

	//モデルインクルード
	// モデル読み込み
	modelMoai = Mesh::LoadFormOBJ("moai", true);
	modelCube = Mesh::LoadFormOBJ("cube", true);
	modelRoad1 = Mesh::LoadFormOBJ("alphaVerLoad", true);
	modelCam = Mesh::LoadFormOBJ("cube", true); 
	modelWalkRobo = Mesh::LoadFormOBJ("walkEnemy",true);
	modelFly = Mesh::LoadFormOBJ("enemy", true);
	modelBill1 = Mesh::LoadFormOBJ("bill1", true);
	modelTower1 = Mesh::LoadFormOBJ("tower1", true);
	enemyBulletModel_ = Mesh::LoadFormOBJ("bume2", true);
	ojamaFenceModel_ = Mesh::LoadFormOBJ("ojamaFence",true);
	icoBallModel_ = Mesh::LoadFormOBJ("IcoSphere", true);
	carryBallEnemyModel_ = Mesh::LoadFormOBJ("CarryArm", true);

	//モデルインサート
	models.insert(std::make_pair("moai", modelMoai.get()));
	models.insert(std::make_pair("Cube", modelCube.get()));
	models.insert(std::make_pair("road1", modelRoad1.get()));
	models.insert(std::make_pair("cam", modelCam.get()));
	models.insert(std::make_pair("bill1", modelBill1.get()));
	models.insert(std::make_pair("tower1", modelTower1.get()));
	
	levelData = JsonLoader::LoadFile("testScene");

	// レベルデータからオブジェクトを生成、配置
	for (auto& objectData : levelData->objects) {
		// ファイル名から登録済みモデルを検索
		Mesh* model = nullptr;
		decltype(models)::iterator it = models.find(objectData.fileName);
		if (it != models.end()) {
			model = it->second;
		}
		
		//// 建物を一時的に消去
		//if (objectData.fileName == "bill1" || objectData.fileName == "tower1") {
		//	continue;
		//}
		
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
			camObjs.push_back(newObject);
			continue;
		}
		else if (objectData.fileName == "moai") {
			moaiObjs.push_back(newObject);
			moaiObjs.back().SetScale({3,3,3});
			EnemyState newState;
			newState.hp_ = offsetEnemyHp_;
			newState.isDead_ = false;
			newState.isAtk_ = false;
			moaiState.push_back(newState);

			moaiSpCollider.push_back(new SphereCollider);
			int nowSphereNum = static_cast<int>(moaiSpCollider.size()-1);
			CollisionManager::GetInstance()->AddCollider(moaiSpCollider[nowSphereNum]);
			moaiSpCollider[nowSphereNum]->SetBasisPos(&moaiObjs[moaiObjs.size()-1].worldTransform.translation_);
			moaiSpCollider[nowSphereNum]->center = moaiObjs[moaiObjs.size() - 1].worldTransform.translation_;
			moaiSpCollider[nowSphereNum]->SetRadius(scale.x);
			moaiSpCollider[nowSphereNum]->SetAttribute(COLLISION_ATTR_ENEMIES);
			moaiSpCollider[nowSphereNum]->Update();
			continue;
		}
		else if (objectData.fileName == "ojamaFence") {


			continue;
		}
		
		// 配列に登録
		objects.push_back(newObject);
	}



#pragma region ポップデータ読み込み
	InitEnemyCommands();
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
		walkingEnemies.back()->Initialize(modelWalkRobo.get());
		walkingEnemies.back()->SetOffsetVec3(offsetPos);
		walkingEnemies.back()->SetRailCameraInfo(railCameraInfo_);
		walkingEnemies.back()->SetPlayerWorldTransform(playerWorldTF_);
		walkingEnemies.back()->SetBulletModel(enemyBulletModel_.get());

	}
	else if (enemyNum == ENEMY_NUM::FLOATING_ENEMY) {
		//オブジェクト処理
		std::unique_ptr<FloatingEnemy> newFloatingEnemy;
		newFloatingEnemy = std::make_unique<FloatingEnemy>();
		floatingEnemies.push_back(std::move(newFloatingEnemy));
		floatingEnemies.back()->Initialize(modelFly.get());
		floatingEnemies.back()->SetOffsetVec3(offsetPos);
		floatingEnemies.back()->SetRailCameraInfo(railCameraInfo_);
		floatingEnemies.back()->SetPlayerWorldTransform(playerWorldTF_);
		floatingEnemies.back()->SetBulletModel(enemyBulletModel_.get());

	}
	else if (enemyNum == ENEMY_NUM::CARRY_BALL_ENEMY) {
		//オブジェクト処理
		std::unique_ptr<CarryBallEnemy> newCarryBallEnemy;
		newCarryBallEnemy = std::make_unique<CarryBallEnemy>();
		carryBallEnemies.push_back(std::move(newCarryBallEnemy));
		carryBallEnemies.back()->Initialize(carryBallEnemyModel_.get());
		carryBallEnemies.back()->SetOffsetVec3(offsetPos);
		carryBallEnemies.back()->SetRailCameraInfo(railCameraInfo_);
		carryBallEnemies.back()->SetPlayerWorldTransform(playerWorldTF_);
		carryBallEnemies.back()->SetBulletModel(icoBallModel_.get());

	}
}





void GameObjManager::UpdateAllObjects()
{
	// Listのremove
	walkingEnemies.remove_if([](std::unique_ptr<WalkingEnemy>& enemy) {
		return enemy->GetState()->isDead_;
		});
	floatingEnemies.remove_if([](std::unique_ptr<FloatingEnemy>& enemy) {
		return enemy->GetState()->isDead_;
		});
	carryBallEnemies.remove_if([](std::unique_ptr<CarryBallEnemy>& enemy) {
		return enemy->GetState()->isDead_;
		});
	ojamaFences.remove_if([](std::unique_ptr<OjamaFence>& enemy) {
		return enemy->GetState()->isDead_;
		});

	//ポップコマンド
	if (isEnemyPops_ == true) {
		UpdateWalkingEnemyPopCommands();
		UpdateFloatingEnemyPopCommands();
		UpdateCarryBallEnemyPopCommands();
	}

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
		float moveSpeed = 0.1f;

		
		moaiObjs[i].worldTransform.translation_.x += cosf(moaiDigRot * 3.14f/180.f) * moveSpeed;
		moaiObjs[i].worldTransform.translation_.y += sinf(moaiDigRot * 3.14f / 180.f) * moveSpeed;

		moaiObjs[i].Update();

		moaiSpCollider[i]->SetBasisPos(&moaiObjs[i].worldTransform.translation_);

		
		if (moaiSpCollider[i]->GetIsHit() == true) {
			if (moaiSpCollider[i]->GetCollisionInfo().collider->GetAttribute() == COLLISION_ATTR_ALLIES) {
				moaiState[i].hp_--;
			}
		}
		if (moaiState[i].hp_ <= 0) {
			moaiState[i].isDead_ = true;
			moaiSpCollider[i]->RemoveAttribute(8);
			
		}

		if (moaiState[i].isDead_ == true) {	//死去
			//moaiObjs.erase(std::remove_if(moaiObjs.begin(), moaiObjs.end(),
			//	[](const EnemyState& state) {
			//		return state.isDead_;
			//	}),
			//	moaiObjs.end());
			////moaiSpCollider.erase(std::cbegin(moaiSpCollider) + i);
			//moaiState.erase(std::cbegin(moaiState) + i);
		}
		moaiSpCollider[i]->Update();
		
	}

	//歩兵更新
	for (const unique_ptr<WalkingEnemy>& enemy : walkingEnemies) {
		enemy->Update();
	}

	//浮遊敵更新
	for (const unique_ptr<FloatingEnemy>& enemy : floatingEnemies) {
		enemy->Update();
	}

	//抱玉敵更新
	for (const unique_ptr<CarryBallEnemy>& enemy : carryBallEnemies) {
		enemy->Update();
	}

	//おじゃまフェンス
	for (const unique_ptr<OjamaFence>& enemy : ojamaFences) {
		enemy->Update();
	}

	

}

void GameObjManager::DrawAllObjs(ID3D12GraphicsCommandList* cmdList)
{

	for (const unique_ptr<WalkingEnemy>& enemy : walkingEnemies) {
		enemy->Draw(cmdList);
	}

	for (const unique_ptr<FloatingEnemy>& enemy : floatingEnemies) {
		enemy->Draw(cmdList);
	}

	for (const unique_ptr<CarryBallEnemy>& enemy :carryBallEnemies) {
		enemy->Draw(cmdList);
	}

	for (const unique_ptr<OjamaFence>& enemy : ojamaFences) {
		enemy->Draw(cmdList);
	}



	for (int i = 0; i < objects.size(); i++) {
		objects[i].Draw(cmdList);
	}
	
	for (int i = 0; i < moaiObjs.size(); i++) {
		if (moaiState[i].hp_ > 0) {
			moaiObjs[i].Draw(cmdList);
		}
	}
	
}

void GameObjManager::DestroyAllEnemies()
{
 
	// 敵オブジェをクリア
	walkingEnemies.clear();
	floatingEnemies.clear();
	carryBallEnemies.clear();
	ojamaFences.clear();

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

void GameObjManager::UpdatePopCommandsInfo()
{
}

void GameObjManager::UpdateWalkingEnemyPopCommands()
{
	gameTime_++;	//毎f処理
	//待機処理
	if (walkingEIsStand_) {
		walkingEstandTime_--;
		if (walkingEstandTime_ <= 0) {
			//待機完了
			walkingEIsStand_ = false;
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

			

			
			if (lane == SPOWN_OFFSET_POS::SP_LEFT) {
				//offset
				Vector3 offset = { -adjustWalkingESpownLen_,0,0 };
				AddEnemy(0,0,offset);
			}
			else if (lane == SPOWN_OFFSET_POS::SP_CENTER) {
				//offset
				Vector3 offset = { 0,0,0 };
				AddEnemy(0, 0, offset);
			}
			else if (lane == SPOWN_OFFSET_POS::SP_RIGHT) {
				//offset
				Vector3 offset = { adjustWalkingESpownLen_,0,0 };
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
			walkingEIsStand_ = true;
			walkingEstandTime_ = waitTime;

			//抜ける
			break;
		}
	}
}

void GameObjManager::UpdateFloatingEnemyPopCommands()
{
	//待機処理
	if (floatingEIsStand_) {
		floatingEstandTime_--;
		if (floatingEstandTime_ <= 0) {
			//待機完了
			floatingEIsStand_ = false;
		}
		return;
	}
	// 1行分の文字列を入れる変数
	std::string line;

	//コマンド実行ループ
	while (getline(floatingEnemyPopCommands_, line)) {
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


			if (lane == SPOWN_OFFSET_POS::SP_LEFT) {
				//offset
				Vector3 offset = { -adjustFloatingESpownLenLong_,0,0 };
				AddEnemy(ENEMY_NUM::FLOATING_ENEMY, 0, offset);
			}
			else if (lane == SPOWN_OFFSET_POS::SP_CENTER) {
				//offset
				Vector3 offset = { 0,0,0 };
				AddEnemy(ENEMY_NUM::FLOATING_ENEMY, 0, offset);
			}
			else if (lane == SPOWN_OFFSET_POS::SP_RIGHT) {
				//offset
				Vector3 offset = { adjustFloatingESpownLenLong_,0,0 };
				AddEnemy(ENEMY_NUM::FLOATING_ENEMY, 0, offset);
			}
			else if (lane == SPOWN_OFFSET_POS::SP_SHORT_RIGHT) {
				//offset
				Vector3 offset = { adjustFloatingESpownLenShort_,0,0 };
				AddEnemy(ENEMY_NUM::FLOATING_ENEMY, 0, offset);
			}
			else if (lane == SPOWN_OFFSET_POS::SP_SHORT_LEFT) {
				//offset
				Vector3 offset = { -adjustFloatingESpownLenShort_,0,0 };
				AddEnemy(ENEMY_NUM::FLOATING_ENEMY, 0, offset);
			}
			else {
				//offset
				Vector3 offset = { 0,0,0 };
				AddEnemy(ENEMY_NUM::FLOATING_ENEMY, 0, offset);
			}
		}
		// WAITコマンド
		else if (word.find("WAIT") == 0) {
			std::getline(line_stream, word, ',');

			//待ち時間
			int32_t waitTime = std::atoi(word.c_str());

			//待機開始
			floatingEIsStand_ = true;
			floatingEstandTime_ = waitTime;

			//抜ける
			break;
		}
	}
}

void GameObjManager::UpdateCarryBallEnemyPopCommands()
{
	//待機処理
	if (carryBallEIsStand_) {
		carryBallEstandTime_--;
		if (carryBallEstandTime_ <= 0) {
			//待機完了
			carryBallEIsStand_ = false;
		}
		return;
	}
	// 1行分の文字列を入れる変数
	std::string line;

	//コマンド実行ループ
	while (getline(carryBallEnemyPopCommands_, line)) {
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




			if (lane == SPOWN_OFFSET_POS::SP_LEFT) {
				//offset
				Vector3 offset = { -adjustCarryBallESpownLenLong_,0,0 };
				AddEnemy(ENEMY_NUM::CARRY_BALL_ENEMY, 0, offset);
			}
			else if (lane == SPOWN_OFFSET_POS::SP_CENTER) {
				//offset
				Vector3 offset = { 0,0,0 };
				AddEnemy(ENEMY_NUM::CARRY_BALL_ENEMY, 0, offset);
			}
			else if (lane == SPOWN_OFFSET_POS::SP_RIGHT) {
				//offset
				Vector3 offset = { adjustCarryBallESpownLenLong_,0,0 };
				AddEnemy(ENEMY_NUM::CARRY_BALL_ENEMY, 0, offset);
			}
			else if (lane == SPOWN_OFFSET_POS::SP_SHORT_RIGHT) {
				//offset
				Vector3 offset = { adjustCarryBallESpownLenShort_,0,0 };
				AddEnemy(ENEMY_NUM::CARRY_BALL_ENEMY, 0, offset);
			}
			else if (lane == SPOWN_OFFSET_POS::SP_SHORT_LEFT) {
				//offset
				Vector3 offset = { -adjustCarryBallESpownLenShort_,0,0 };
				AddEnemy(ENEMY_NUM::CARRY_BALL_ENEMY, 0, offset);
			}
			else {
				//offset
				Vector3 offset = { 0,0,0 };
				AddEnemy(ENEMY_NUM::CARRY_BALL_ENEMY, 0, offset);
			}
		}
		// WAITコマンド
		else if (word.find("WAIT") == 0) {
			std::getline(line_stream, word, ',');

			//待ち時間
			int32_t waitTime = std::atoi(word.c_str());

			//待機開始
			carryBallEIsStand_ = true;
			carryBallEstandTime_ = waitTime;

			//抜ける
			break;
		}
	}
}




void GameObjManager::SetPlayerWorldTF(WorldTransform* worldTF)
{
	playerWorldTF_ = worldTF;
}

void GameObjManager::SetIsEnemyPops(bool isPop)
{
	isEnemyPops_ = isPop;
}

void GameObjManager::InitEnemyCommands()
{
	//歩兵敵
	ResetCommands("Resources/enemyPop2.csv", walkingEnemyPopCommands_);
	gameTime_ = 0;
	walkingEstandTime_ = 0;
	walkingEIsStand_ = false;

	//浮遊敵
	ResetCommands("Resources/floatingEnemyPop.csv", floatingEnemyPopCommands_);
	floatingEIsStand_ = 0;
	floatingEstandTime_ = false;

	ResetCommands("Resources/carryBallEnemyPop.csv", carryBallEnemyPopCommands_);
	carryBallEIsStand_ = 0;
	carryBallEstandTime_ = false;
}

void GameObjManager::InitOjamaFence()
{
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
		newObject.SetPosition(Vector3(pos.x, pos.y, pos.z));

		// 回転角
		DirectX::XMFLOAT3 rot;
		DirectX::XMStoreFloat3(&rot, objectData.rotation);
		newObject.SetRotate(Vector3(rot.x, rot.y, rot.z));

		// 座標
		DirectX::XMFLOAT3 scale;
		DirectX::XMStoreFloat3(&scale, objectData.scaling);
		newObject.SetScale(Vector3(scale.x, scale.y, scale.z));

		if (objectData.fileName == "ojamaFence") {

			std::unique_ptr<OjamaFence> newOjamaFence;
			newOjamaFence = std::make_unique<OjamaFence>();
			ojamaFences.push_back(std::move(newOjamaFence));
			ojamaFences.back()->Initialize(ojamaFenceModel_.get());
			ojamaFences.back()->SetWorldTransform(newObject.worldTransform);
			
		}
	}
}

void GameObjManager::GameObjInitialize()
{

	// fbx用変数
	{
		hitokunFbxM_.reset(FbxLoader::GetInstance()->LoadModelFromFile("Cube", true));
		//Player
		fbxPlayer_ = std::make_unique<FbxPlayer>();
		fbxPlayer_->SetRailTargetPos(camera_->GetRailTargetPosPtr());	//レティクル用
		fbxPlayer_->Initialize(hitokunFbxM_.get());

		fbxPlayer_->SetRailCameraInfo(railCameraInfo_);
		

		camera_->SetFollowerPos(fbxPlayer_.get()->GetObject3d()->GetWorldTransformPtr());
		camera_->SetPlayerParallelMoveVal_(fbxPlayer_->GetParallelMovePtr());
		camera_->SetTargetPosVelueToAdd(fbxPlayer_->GetTargetPosVelueToAddPtr());
	}

	//ゲームオブジェクトクラスに情報セット
	SetRailCamInfo(camera_->GetRailCameraInfo());
	SetPlayerWorldTF(fbxPlayer_->GetObject3d()->GetWorldTransformPtr());


}

void GameObjManager::RailCameraInit(RailCameraInfo* info)
{
	railCameraInfo_ = info;
}

