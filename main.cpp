#include"WinApp.h"
#include"DirectXCommon.h"
#include"Error.h"
#include"Mesh.h"
#include"input.h"
#include"Object3d.h"
#include"Sprite.h"


const float PI = 3.14f;

//using namespace Microsoft::WRL;
template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

//@brief コンソール画面にフォーマット付き文字列の表示
//@param format フォーマット(%dとか%fとかの)
//@param 可変長引数
//@remarks この関数はデバック用です。デバッグ時にしか動作しません
//void DebugOutputFormatString(const char* format, ...) {
//#ifdef _DEBUG
//	va_list valist;
//	va_start(valist, format);
//	vprintf(format, valist);
//	va_end(valist);
//#endif
//}



//Windowsアプリでのエントリーポイント（main関数）
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
//コンソールへの文字出力
{


	//OutputDebugStringA("Hello,DrectX!!/n");

	//ポインタ宣言
	WinApp* winApp_ = nullptr;
	winApp_ = new WinApp;


	DirectXCommon* dxCommon_ = nullptr;
	dxCommon_ = new DirectXCommon;

	//winApp初期化
	winApp_->Initialize();

	MSG msg = {};
	//DirectX初期化処理　ここから
	dxCommon_->Initialize(winApp_);

	Input* input_ = nullptr;
	input_ = new Input;
	input_->Initialize(winApp_);

	// 3Dオブジェクト静的初期化
	Object3d::StaticInitialize(dxCommon_->GetDevice(), WinApp::window_width, WinApp::window_height);

	
	Mesh* model_ = Mesh::LoadFromOBJ("cube");
	
	Object3d* object3d = Object3d::Create();
	object3d->SetModel(model_);

	object3d->Update();
	
	
	//mesh_->Init();
	
	
	
	
	while (true) {
		if (winApp_->ProcessMessage()) {
			break;
		}

		//更新
		input_->Update();
		object3d->Update();
		
		
		//描画
		dxCommon_->PreDraw();

		Object3d::PreDraw(dxCommon_->GetCommandList());

		object3d->Draw();

		Object3d::PostDraw();

		dxCommon_->PostDraw();

	}

	winApp_->Finalize();

	//元データ解放
	//delete[] imageData;
	//3Dオブジェクトの解放
	delete object3d;
	//3Dモデル開放
	delete model_;
	//入力開放
	delete input_;

	return 0;
}