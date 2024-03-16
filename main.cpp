#include"SummarizeEngine/SummarizeEngine.h"
#include"Error.h"

template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

//Windowsアプリでのエントリーポイント（main関数）
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
//コンソールへの文字出力
{	
	

	std::unique_ptr <SummarizeEngine> engine_;
	engine_ = std::make_unique<SummarizeEngine>();

	engine_->Initialize();

	MSG msg = {};
	


	while (engine_->isRunningGame) {
		engine_->Update();
		
		//描画
		engine_->Draw();

	}

	engine_->Finalize();

	return 0;
}