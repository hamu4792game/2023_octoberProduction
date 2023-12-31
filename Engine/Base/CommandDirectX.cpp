#include "CommandDirectX.h"
#include "Engine/Log.h"
#include <cassert>
#include "Engine/Input/KeyInput/KeyInput.h"
#include "Engine/Manager/AudioManager/AudioManager.h"
#include "math/Vector2.h"
#include "math/Vector3.h"
#include "math/Vector4.h"


#include "Engine/Base/ConstantBuffer.h"
#include "Engine/Base/GraphicsPipeline/GraphicsPipeline.h"
#include "Engine/Base/MultipathRendering/MultipathRendering.h"

//	imguiのinclude
#include "externals/imgui/imgui.h"
#include "externals/imgui/imgui_impl_dx12.h"
#include "externals/imgui/imgui_impl_win32.h"
#include "GraphicsPipeline/GraphicsPipeline.h"
//	関数の外部宣言
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

CommandDirectX* CommandDirectX::GetInstance()
{
	static CommandDirectX instance;
	return &instance;
}


void CommandDirectX::Initialize(WinApp* winApp, int32_t bufferWidth, int32_t bufferHeight)
{
	//	メンバ変数の確保
	winApp_ = winApp;
	bufferWidth_ = bufferWidth; bufferHeight_ = bufferHeight;


	CreateFactry();
	SelectAdapter();
	CreateDevice();
	CreateCommandQueue();
	CreateCommandList();
	CreateSwapChain();
	CreateRenderTargetView();
	CreateDepthStencilResource();
	CreateMultipathRendering();
	CreatePeraVertex();
	CreatePeraPipeline();
	CreateFence();

	//	Inputの初期化処理
	KeyInput::InputInitialize();

	//	Audioの初期化処理
	AudioManager::Initialize();

	//	マルチパスレンダリング用の定数バッファ変数の初期化
	MultipathRendering::GetInstance()->Initialize();
}

void CommandDirectX::PreDraw()
{
	//	ImGuiにframeの始まりを伝える
	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	//	ImGuiデモの表示
	//ImGui::ShowDemoWindow();

	//	Input初期の更新
	KeyInput::Update();

	//	ここから書き込むバックバッファのインデックスを取得
	//UINT backBufferIndex = swapChain->GetCurrentBackBufferIndex();

	/*
	//	TransitionBarrierの設定
	D3D12_RESOURCE_BARRIER barrier{};
	//	今回のバリアはTransition
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	//	Noneにしておく
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	//	バリアを張る対象のリソース。現在のバックバッファに対して行う
	barrier.Transition.pResource = swapChainResources[backBufferIndex].Get();
	//	遷移前（現在）のResourceState
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
	//	遷移後のResourceState
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
	//	TransitionBarrierを張る
	commandList->ResourceBarrier(1, &barrier);

	*/

	//	バリア
	CD3DX12_RESOURCE_BARRIER preBarrier =
		CD3DX12_RESOURCE_BARRIER::Transition(peraResource.Get(), D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_RENDER_TARGET);
	commandList->ResourceBarrier(1, &preBarrier);


	//	描画先のRTVとDSVを設定する
	// 1パス目
	auto rtvHeapPointer = peraRTVHeap->GetCPUDescriptorHandleForHeapStart();

	auto dsvHandle = dsvDescriptorHeap->GetCPUDescriptorHandleForHeapStart();

	commandList->OMSetRenderTargets(1, &rtvHeapPointer, false, &dsvHandle);
	//	画面クリア
	ClearRenderTarget(rtvHeapPointer);
	//	指定した深度で画面全体をクリアする
	commandList->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);


	//	ビューポート
	D3D12_VIEWPORT viewport{};
	//	クライアント領域のサイズと一緒にして画面全体に表示
	viewport.Width = static_cast<FLOAT>(bufferWidth_);
	viewport.Height = static_cast<FLOAT>(bufferHeight_);
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	//	Viewportを設定
	commandList->RSSetViewports(1, &viewport);

	//	シザー矩形
	D3D12_RECT scissorRect{};
	//	基本的にビューポートと同じ矩形が構成されるようにする
	scissorRect.left = 0;
	scissorRect.right = bufferWidth_;
	scissorRect.top = 0;
	scissorRect.bottom = bufferHeight_;
	//	Scirssorを設定
	commandList->RSSetScissorRects(1, &scissorRect);

}

void CommandDirectX::PostDraw()
{
	//	描画用のDescriptorHeapの設定
	ID3D12DescriptorHeap* descriptorHeap[] = { srvDescriptorHeap };
	commandList->SetDescriptorHeaps(1, descriptorHeap);

	//	peraResourceを反転させる
	D3D12_RESOURCE_BARRIER peraBarrier{};
	//	今回のバリアはTransition
	peraBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	//	Noneにしておく
	peraBarrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	//	バリアを張る対象のリソース。現在のバックバッファに対して行う
	peraBarrier.Transition.pResource = peraResource.Get();
	//	今回はRenderTargetからPresentにする
	peraBarrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
	peraBarrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
	//	TransitionBarrierを張る
	commandList->ResourceBarrier(1, &peraBarrier);


	//	ここから書き込むバックバッファのインデックスを取得
	UINT backBufferIndex = swapChain->GetCurrentBackBufferIndex();

	D3D12_RESOURCE_BARRIER barrier = {};
	//	今回のバリアはTransition
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	//	Noneにしておく
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	//	バリアを張る対象のリソース。現在のバックバッファに対して行う
	barrier.Transition.pResource = swapChainResources[backBufferIndex].Get();
	//	今回はRenderTargetからPresentにする
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
	//	TransitionBarrierを張る
	commandList->ResourceBarrier(1, &barrier);

	auto rtvHeapPointer = rtvDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
	rtvHeapPointer.ptr += static_cast<unsigned long long>(backBufferIndex) * device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	commandList->OMSetRenderTargets(1, &rtvHeapPointer, false, nullptr);
	ClearRenderTarget(rtvHeapPointer);


	//	ペラポリゴンの描画
	commandList->SetGraphicsRootSignature(peraRootSignature.Get());
	commandList->SetPipelineState(peraPipeline.Get());
	commandList->SetDescriptorHeaps(1, peraSRVHeap.GetAddressOf());
	auto handle = peraSRVHeap->GetGPUDescriptorHandleForHeapStart();
	commandList->SetGraphicsRootDescriptorTable(0, handle);
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	commandList->IASetVertexBuffers(0, 1, &peraVBV);
	commandList->SetGraphicsRootConstantBufferView(1, MultipathRendering::GetInstance()->cEffectParameters.GetGPUVirtualAddress());

	commandList->DrawInstanced(4, 1, 0, 0);

	//	ImGuiの内部コマンドを生成
	commandList->SetDescriptorHeaps(1, &srvDescriptorHeap);
	ImGui::Render();
#ifdef _DEBUG
	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), commandList);
#endif // DEBUG


	//	取得
	/*backBufferIndex = 0;
	backBufferIndex = swapChain->GetCurrentBackBufferIndex();*/

	D3D12_RESOURCE_BARRIER postBarrier = {};
	//	今回のバリアはTransition
	postBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	//	Noneにしておく
	postBarrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	//	バリアを張る対象のリソース。現在のバックバッファに対して行う
	postBarrier.Transition.pResource = swapChainResources[backBufferIndex].Get();
	//	今回はRenderTargetからPresentにする
	postBarrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
	postBarrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
	//	TransitionBarrierを張る
	commandList->ResourceBarrier(1, &postBarrier);

	//	コマンドリストの内容を確定させる。すべてのコマンドを積んでからCloseする
	HRESULT hr = commandList->Close();
	assert(SUCCEEDED(hr));

	//	GPUにコマンドリストの実行を行わせる
	ID3D12CommandList* commandLists[] = { commandList };
	commandQueue->ExecuteCommandLists(1, commandLists);
	//	GPUとOSに画面の交換を行うよう通知する
	swapChain->Present(1, 0);

	//	Fenceの値を更新
	fenceValue++;
	//	GPUがここまでたどり着いた時に、Fenceの値を指定した値に代入するようにSignalを送る
	commandQueue->Signal(fence, fenceValue);
	//	Fenceの値が指定したSignal値にたどり着いているか確認する
	//	GetCompletedValueの初期値はFence作成時に渡した初期値
	if (fence->GetCompletedValue() < fenceValue)
	{

		//	指定したSignalにたどり着いていないので、たどり着くまで待つようにイベントを設定する
		fence->SetEventOnCompletion(fenceValue, fenceEvent);
		//	イベント待つ
		WaitForSingleObject(fenceEvent, INFINITE);
	}

	//	次のフレーム用のコマンドリストを用意
	hr = commandAllocator->Reset();
	assert(SUCCEEDED(hr));
	hr = commandList->Reset(commandAllocator, nullptr);
	assert(SUCCEEDED(hr));
}

void CommandDirectX::Finalize()
{
	//	ImGuiの解放
	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	//	解放処理
	CloseHandle(fenceEvent);
	fence->Release();
	dsvDescriptorHeap->Release();
	srvDescriptorHeap->Release();
	rtvDescriptorHeap->Release();

	peraVB->Release();
	//peraResource->Release();
	peraRTVHeap->Release();
	peraSRVHeap->Release();
	peraRootSignature->Release();

	swapChain->Release();
	commandList->Release();
	commandAllocator->Release();
	commandQueue->Release();
	useAdapter->Release();
	dxgiFactory->Release();
	depthStencilResource->Release();

}

void CommandDirectX::CreateFactry()
{
	//	DXGIファクトリーの生成
	dxgiFactory = nullptr;
	//	HRESULTはWindows系のエラーコードであり、
	//	関数が成功したかどうかをSUCCEEDEDマクロで判定できる
	HRESULT hr = CreateDXGIFactory(IID_PPV_ARGS(&dxgiFactory));
	//	初期化の根本的な部分でエラーが出た場合はプログラムが間違っているか、
	//	どうにもできない場合が多いのでassertにしておく
	assert(SUCCEEDED(hr));

}

void CommandDirectX::SelectAdapter()
{
	//	使用するアダプタ用の変数。最初にnullptrを入れておく
	useAdapter = nullptr;
	//	良い順にアダプタを頼む
	for (UINT i = 0; dxgiFactory->EnumAdapterByGpuPreference(i, DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, IID_PPV_ARGS(&useAdapter))
		!= DXGI_ERROR_NOT_FOUND; ++i)
	{
		//	アダプタの情報を取得する
		DXGI_ADAPTER_DESC3 adapterDesc{};
		HRESULT hr = useAdapter->GetDesc3(&adapterDesc);
		assert(SUCCEEDED(hr));	//取得できないのは一大事
		//	ソフトウェアアダプタでなければ採用
		if (!(adapterDesc.Flags & DXGI_ADAPTER_FLAG3_SOFTWARE))
		{
			//	採用したアダプタの情報をログに出力。wstringの方なので注意
			Log(ConvertString(std::format(L"Use Adapter:{}\n", adapterDesc.Description)));
			break;
		}
		//	ソフトウェアアダプタの場合は見なかったことにする
		useAdapter = nullptr;
	}
	//	適切なアダプタが見つからなかったので起動できない
	assert(useAdapter != nullptr);
}

void CommandDirectX::CreateDevice()
{
	device = nullptr;
	//	機能レベルとログ出力用の文字列
	D3D_FEATURE_LEVEL featureLevels[] = {
		D3D_FEATURE_LEVEL_12_2,D3D_FEATURE_LEVEL_12_1,D3D_FEATURE_LEVEL_12_0
	};
	const char* featureLevelStrings[] = { "12.2","12.1","12.0" };
	//	高い順に生成できるか試していく
	for (size_t i = 0; i < _countof(featureLevels); ++i)
	{
		//	採用したアダプタでデバイスを生成
		HRESULT hr = D3D12CreateDevice(useAdapter, featureLevels[i], IID_PPV_ARGS(&device));
		//	指定した機能レベルでデバイスが生成できたかを確認
		if (SUCCEEDED(hr))
		{
			//	生成できたのでログ出力を行ってループを抜ける
			Log(std::format("FeatureLevel : {}\n", featureLevelStrings[i]));
			break;
		}
	}
	//	デバイスの生成がうまくいかなかったので機能できない
	assert(device != nullptr);
	Log("Complete create D3D12Device!!!\n");	//初期化完了のログを出す
}

void CommandDirectX::CreateCommandQueue()
{
	//	コマンドキューを生成する
	commandQueue = nullptr;
	D3D12_COMMAND_QUEUE_DESC commandQueueDesc{};
	HRESULT hr = device->CreateCommandQueue(&commandQueueDesc, IID_PPV_ARGS(&commandQueue));
	//	コマンドキューの生成がうまくいかないので起動できない
	assert(SUCCEEDED(hr));
}

void CommandDirectX::CreateCommandList()
{
	//	コマンドアロケーターを生成する
	commandAllocator = nullptr;
	HRESULT hr = device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&commandAllocator));
	//	コマンドアロケーターの生成がうまくいかないので起動できない
	assert(SUCCEEDED(hr));

	//	コマンドリストを生成する
	commandList = nullptr;
	hr = device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, commandAllocator, nullptr, IID_PPV_ARGS(&commandList));
	//	コマンドリストの生成がうまくいかないので起動できない
	assert(SUCCEEDED(hr));
}

void CommandDirectX::CreateSwapChain()
{
	//	スワップチェーンを生成する
	swapChain = nullptr;
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};
	swapChainDesc.Width = bufferWidth_;		//	画面の幅
	swapChainDesc.Height = bufferHeight_;	//	画面の高さ
	swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;	//	色の形式
	swapChainDesc.SampleDesc.Count = 1;		//	マルチサンプリングしない
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;	//	描画のターゲットとして利用する
	swapChainDesc.BufferCount = 2;			//	ダブルバッファ
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;	//	モニタに移したら、中身を破棄
	//	コマンドキュー、ウィンドウハンドル、設定を渡して生成する
	HRESULT hr = dxgiFactory->CreateSwapChainForHwnd(commandQueue, winApp_->GetHwnd(), &swapChainDesc, nullptr, nullptr, reinterpret_cast<IDXGISwapChain1**>(&swapChain));
	assert(SUCCEEDED(hr));
}

void CommandDirectX::CreateRenderTargetView()
{
	HRESULT hr = S_FALSE;
	//	ディスクリプタヒープの生成
	//	RTV用のヒープでディスクリプタの数は2。RTVはShader内で触るものではないので、ShaderVisibleはfalse
	rtvDescriptorHeap = CreateDescriptorHeap(device.Get(), D3D12_DESCRIPTOR_HEAP_TYPE_RTV, 2, false);
	//	SRV用のヒープでディスクリプタの数は128。SRVはShader内で触るものなので、ShaderVisibleはtrue
	srvDescriptorHeap = CreateDescriptorHeap(device.Get(), D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, 128, true);

	//	SwapChainからResourceを引っ張ってくる
	swapChainResources[0] = nullptr;
	swapChainResources[1] = nullptr;
	hr = swapChain->GetBuffer(0, IID_PPV_ARGS(&swapChainResources[0]));
	//	うまく取得できなければ起動できない
	assert(SUCCEEDED(hr));
	hr = swapChain->GetBuffer(1, IID_PPV_ARGS(&swapChainResources[1]));
	assert(SUCCEEDED(hr));

	//	RTVの設定
	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};
	rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;		//	出力結果をSRGBに変換して書き込む
	rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;	//	2dテクスチャとして書き込む
	//	ディスクリプタの先頭を取得する
	D3D12_CPU_DESCRIPTOR_HANDLE rtvStartHandle = rtvDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
	//	RTVを2つ作るのでディスクリプタを2つ用意
	//	まず1つ目を作る。一つ目は最初のところに作る。作る場所をこちらで指定してあげる必要がある
	rtvHandle[0] = rtvStartHandle;
	device->CreateRenderTargetView(swapChainResources[0].Get(), &rtvDesc, rtvHandle[0]);
	//	2つ目のディスクリプタハンドルを得る（自力で）
	rtvHandle[1].ptr = rtvHandle[0].ptr + device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	//	2つ目を作る
	device->CreateRenderTargetView(swapChainResources[1].Get(), &rtvDesc, rtvHandle[1]);

	//	swapChaineのbufferCountの取得
	DXGI_SWAP_CHAIN_DESC1 SCD;
	swapChain->GetDesc1(&SCD);
	//	ImGuiの初期化
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui_ImplWin32_Init(winApp_->GetHwnd());
	ImGui_ImplDX12_Init(device.Get(),
		SCD.BufferCount,
		rtvDesc.Format,
		srvDescriptorHeap,
		srvDescriptorHeap->GetCPUDescriptorHandleForHeapStart(),
		srvDescriptorHeap->GetGPUDescriptorHandleForHeapStart()
	);

}

void CommandDirectX::CreateMultipathRendering()
{
	// 書き込み先リソースの作成

	// 使っているバックバッファーの情報を利用する
	auto& backbuffer = swapChainResources[1];
	auto resDesc = backbuffer->GetDesc();
	D3D12_HEAP_PROPERTIES heapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
	//	レンダリング時のクリア値と同じ値
	float clearColor[] = { 0.1f,0.25f,0.5f,1.0f };	//	青っぽい色、RGBA
	D3D12_CLEAR_VALUE clearValue = CD3DX12_CLEAR_VALUE(DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, clearColor);

	HRESULT result = device->CreateCommittedResource(
		&heapProp,
		D3D12_HEAP_FLAG_NONE,
		&resDesc,
		// D3D12_RESOURCE_STATE_RENDER_TARGETではない
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
		&clearValue,
		IID_PPV_ARGS(peraResource.ReleaseAndGetAddressOf())
	);
	assert(SUCCEEDED(result));

	// ↓ビューを作る

	// 作成済みのヒープ情報を使ってもう一枚作る
	auto heapDesc = rtvDescriptorHeap->GetDesc();
	
	// RTV用ヒープを作る
	heapDesc.NumDescriptors = 1;
	result = device->CreateDescriptorHeap(
		&heapDesc,
		IID_PPV_ARGS(peraRTVHeap.GetAddressOf()));
	assert(SUCCEEDED(result));

	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc = {};
	rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
	rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;

	// レンダーターゲットビュー(RTV)を作る
	auto handle = peraRTVHeap->GetCPUDescriptorHandleForHeapStart();

	device->CreateRenderTargetView(
		peraResource.Get(),
		&rtvDesc,
		handle);

	// SRV用ヒープを作る
	heapDesc = {};
	heapDesc.NumDescriptors = 1;
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	heapDesc.NodeMask = 0;
	
	result = device->CreateDescriptorHeap(
		&heapDesc,
		IID_PPV_ARGS(peraSRVHeap.GetAddressOf()));
	assert(SUCCEEDED(result));

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Format = rtvDesc.Format;
	srvDesc.Texture2D.MipLevels = 1;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;

	auto peraHandle = peraSRVHeap->GetCPUDescriptorHandleForHeapStart();
	//peraHandle.ptr += device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	//	シェーダーリソースビューを作る
	device->CreateShaderResourceView(
		peraResource.Get(),
		&srvDesc,
		peraHandle);

}

void CommandDirectX::CreatePeraVertex()
{
	//	頂点データの作成
	struct PeraVertex {
		Vector4 pos;
		Vector2 uv;
	};

	PeraVertex pv[4] = {
		{{-1.0f,-1.0f,0.1f,1.0f},{0.0f,1.0f}}, // 左下
		{{-1.0f, 1.0f,0.1f,1.0f},{0.0f,0.0f}}, // 左上
		{{ 1.0f,-1.0f,0.1f,1.0f},{1.0f,1.0f}}, // 右下
		{{ 1.0f, 1.0f,0.1f,1.0f},{1.0f,0.0f}}, // 右上
	};

	// ペラポリゴンの頂点バッファの作成
	peraVB = Engine::CreateBufferResource(device.Get(), sizeof(pv));

	// バッファービューの設定
	peraVBV.BufferLocation = peraVB->GetGPUVirtualAddress();
	peraVBV.SizeInBytes = sizeof(pv);
	peraVBV.StrideInBytes = sizeof(PeraVertex);

	// マップでコピー
	PeraVertex* peramap = nullptr;
	peraVB->Map(0, nullptr, reinterpret_cast<void**>(&peramap));
	std::copy(std::begin(pv), std::end(pv), peramap);
	//for (auto i = 0; i < _countof(pv); i++) {
	//	peramap[i] = pv[i];
	//}
	peraVB->Unmap(0, nullptr);

}

void CommandDirectX::CreatePeraPipeline()
{
#pragma region ルートシグネチャの作成

	D3D12_DESCRIPTOR_RANGE range{};
	range.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV; // t
	range.BaseShaderRegister = 0; // 0
	range.NumDescriptors = 1;
	range.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	D3D12_ROOT_PARAMETER rp[2] = {};
	rp[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rp[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	rp[0].DescriptorTable.pDescriptorRanges = &range;
	rp[0].DescriptorTable.NumDescriptorRanges = 1;

	rp[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV; // b
	rp[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	rp[1].Descriptor.ShaderRegister = 0;

	peraRootSignature = GraphicsPipeline::GetInstance()->CreateRootSignature(rp, 2);

#pragma endregion

#pragma region インプットレイアウトの作成
	D3D12_INPUT_ELEMENT_DESC layout[2] = {
		{
			"POSITION",
			0,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
			0
		},
		{
			"TEXCOORD",
			0,
			DXGI_FORMAT_R32G32_FLOAT,
			0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
			0
		},
	};
#pragma endregion

#pragma region BlendState

	//	ブレンドモードの設定
	D3D12_BLEND_DESC blendDesc{};
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;

	blendDesc.RenderTarget[0].BlendEnable = false;

#pragma endregion

#pragma region シェーダーの生成

	IDxcBlob* vertexShader = GraphicsPipeline::GetInstance()->CreateVSShader("./Resources/Shader/PeraPoly.VS.hlsl");
	//IDxcBlob* vertexShader = ShaderManager::GetInstance()->CompileShader(ConvertString("./Resources/Shader/PeraPoly.VS.hlsl"), L"vs_6_0");
	IDxcBlob* pixelShader = GraphicsPipeline::GetInstance()->CreatePSShader("./Resources/Shader/PeraPoly.PS.hlsl");

#pragma endregion

	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpsDesc = {};
	gpsDesc.pRootSignature = peraRootSignature.Get();
	gpsDesc.InputLayout.NumElements = _countof(layout);
	gpsDesc.InputLayout.pInputElementDescs = layout;
	gpsDesc.BlendState = blendDesc;
	gpsDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	gpsDesc.NumRenderTargets = 1;
	gpsDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	gpsDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	gpsDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
	gpsDesc.SampleDesc.Count = 1;
	gpsDesc.SampleDesc.Quality = 0;
	gpsDesc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;
	gpsDesc.VS = {
		vertexShader->GetBufferPointer(),
		vertexShader->GetBufferSize()
	};
	gpsDesc.PS = {
		pixelShader->GetBufferPointer(),
		pixelShader->GetBufferSize()
	};

	//	実際に生成
	HRESULT result = device->CreateGraphicsPipelineState(&gpsDesc, IID_PPV_ARGS(peraPipeline.ReleaseAndGetAddressOf()));
	assert(SUCCEEDED(result));


}

void CommandDirectX::CreateFence()
{
	//	初期値0でFenceを作る
	fence = nullptr;
	fenceValue = 0;
	HRESULT hr = device->CreateFence(fenceValue, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));
	assert(SUCCEEDED(hr));

	//	FenceのSignalを持つためのイベントを作成する
	fenceEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	assert(fenceEvent != nullptr);
}

void CommandDirectX::ClearRenderTarget(D3D12_CPU_DESCRIPTOR_HANDLE rtvHeapPointer)
{
	/*UINT backBufferIndex = swapChain->GetCurrentBackBufferIndex();*/

	//	指定した色で画面全体をクリアする
	float clearColor[] = { 0.1f,0.25f,0.5f,1.0f };	//	青っぽい色、RGBA
	//float clearColor[] = { 0.0f,0.0f,0.0f,1.0f };	//	青っぽい色、RGBA
	commandList->ClearRenderTargetView(rtvHeapPointer, clearColor, 0, nullptr);
}

void CommandDirectX::CreateDepthStencilResource()
{
	//	depthStencilResourceの生成
	depthStencilResource = CreateDepthStencilTexture(device.Get(), bufferWidth_, bufferHeight_);
	//	2_5_p16
	//	DSV用のヒープでディスクリプタの数は1。DSVはShader内で触るものではないので、ShaderVisibleはfalse
	dsvDescriptorHeap = CreateDescriptorHeap(device.Get(), D3D12_DESCRIPTOR_HEAP_TYPE_DSV, 1, false);
	//	DSVの設定
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc{};
	dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;	//	Format。基本的にはResourceに合わせる
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;	//	2DTexture
	dsvDesc.Flags = D3D12_DSV_FLAG_NONE;

	//	DSVHeapの先頭にDSVを作る
	device->CreateDepthStencilView(depthStencilResource.Get(), &dsvDesc, dsvDescriptorHeap->GetCPUDescriptorHandleForHeapStart());
}

ID3D12Resource* CommandDirectX::CreateDepthStencilTexture(ID3D12Device* device, int32_t width, int32_t height)
{
	//	生成するResourceの設定
	D3D12_RESOURCE_DESC resourceDesc{};
	resourceDesc.Width = width;	//	textureの幅
	resourceDesc.Height = height;	//	textureの高さ
	resourceDesc.MipLevels = 1;	//	bitmapの数
	resourceDesc.DepthOrArraySize = 1;	//	奥行き or 配列Textureの配列数
	resourceDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;;	//	DepthStencilとして利用可能なフォーマット
	resourceDesc.SampleDesc.Count = 1;	//	サンプリングカウント。1固定
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;	//	二次元
	resourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;	//	DepthStencilとして使う通知

	//	利用するHeapの設定
	D3D12_HEAP_PROPERTIES heapProperties{};
	heapProperties.Type = D3D12_HEAP_TYPE_DEFAULT;	//	VRAM上に作る

	//	深度値のクリア設定
	D3D12_CLEAR_VALUE depthClearValue{};
	depthClearValue.DepthStencil.Depth = 1.0f;	//	1.0f(最大値)でクリア
	depthClearValue.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;;	//	フォーマット。Resourceと合わせる

	//	Resourceの生成
	ID3D12Resource* resource = nullptr;
	HRESULT hr = device->CreateCommittedResource(
		&heapProperties,	//	Heapの設定
		D3D12_HEAP_FLAG_NONE,	//	Heapの特殊な設定。特になし
		&resourceDesc,	//	Resourceの設定
		D3D12_RESOURCE_STATE_DEPTH_WRITE,	//	深度値を書き込む状態にしておく
		&depthClearValue,	//	Clear最適値
		IID_PPV_ARGS(&resource));	//	作成するResourceポインタへのポインタ
	assert(SUCCEEDED(hr));
	return resource;
}

//	DescriptorHeap作成の関数
ID3D12DescriptorHeap* CreateDescriptorHeap(ID3D12Device* device, D3D12_DESCRIPTOR_HEAP_TYPE heapType, UINT numDescriptors, bool shaderVisible) {
	ID3D12DescriptorHeap* descriptorHeap = nullptr;
	D3D12_DESCRIPTOR_HEAP_DESC descriptorHeapDesc{};
	descriptorHeapDesc.Type = heapType;
	descriptorHeapDesc.NumDescriptors = numDescriptors;
	descriptorHeapDesc.Flags = shaderVisible ? D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE : D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	HRESULT hr = device->CreateDescriptorHeap(&descriptorHeapDesc, IID_PPV_ARGS(&descriptorHeap));
	//	ディスクリプタヒープが作れなかったので起動できない
	assert(SUCCEEDED(hr));
	return descriptorHeap;
}

D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandle(ID3D12DescriptorHeap* descriptorHeap, uint32_t descriptorSize, uint32_t index) {
	D3D12_CPU_DESCRIPTOR_HANDLE handleCPU = descriptorHeap->GetCPUDescriptorHandleForHeapStart();
	handleCPU.ptr += (static_cast<unsigned long long>(descriptorSize) * index);
	return handleCPU;
}

D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescriptorHandle(ID3D12DescriptorHeap* descriptorHeap, uint32_t descriptorSize, uint32_t index) {
	D3D12_GPU_DESCRIPTOR_HANDLE handleGPU = descriptorHeap->GetGPUDescriptorHandleForHeapStart();
	handleGPU.ptr += (static_cast<unsigned long long>(descriptorSize) * index);
	return handleGPU;
}