#include <Windows.h>
#include "Engine/Engine.h"
#include "Engine/TimeBaseLoopExecuter/TimeBaseLoopExecuter.h"
#include "externals/imgui/imgui.h"
#include "Engine/Input/KeyInput/KeyInput.h"
#include "math/Rand.h"
#include <chrono>
#include "FrameCount.h"
#include "Game/GameScene/GameScene.h"

float FrameCount::currentFrameTime = 0.0f;

int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR lpCmdLine, _In_ int nShowCmd) {
	static D3DResourceLeakChecker leak;
	int32_t windowWidth = 1280; int32_t windowHeight = 720;
	Engine::Initialize("バグリズム", windowWidth, windowHeight);
	//乱数追加
	SetRandom();

	GameScene::GetInstance()->Initialize();

	//	ウィンドウの×ボタンが押されるまでループ
	while (!WinApp::ProcessMessage()) {
		auto frameStartTime = std::chrono::high_resolution_clock::now();
		//	フレームの開始
		TimeBaseLoopExecuter fpsManager(60);
		
		Engine::BeginFrame();
#ifdef _DEBUG
		//	ImGui のフレームに一つ目の ImGui のウィンドウを描く
		ImGui::Begin("Control panel");
		ImGui::Text("Frame rate: %6.2f fps", ImGui::GetIO().Framerate);
		ImGui::End();
#endif FrameRate

		GameScene::GetInstance()->Update();

		GameScene::GetInstance()->Draw();

		//	フレームの終了
		Engine::EndFrame();
		fpsManager.TimeAdjustment();
		auto frameEndTime = std::chrono::high_resolution_clock::now();
		std::chrono::duration<float> tmpFrameTime = frameEndTime - frameStartTime;
		FrameCount::currentFrameTime = tmpFrameTime.count();
		if (KeyInput::PushKey(DIK_ESCAPE)) {
			break;
		}
	}

	GameScene::GetInstance()->Finalize();

	Engine::Finalize();

	return 0;
}