#include"SummarizeEngine/SummarizeEngine.h"
#include"Error.h"




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
	

	std::unique_ptr <SummarizeEngine> engine_;
	engine_ = std::make_unique<SummarizeEngine>();

	engine_->Initialize();

	MSG msg = {};
	


	while (engine_->isRunningGame_) {
		engine_->Update();
		
		//描画
		engine_->Draw();

	}

	engine_->Finalize();

	return 0;
}