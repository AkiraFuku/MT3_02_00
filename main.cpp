#include <Novice.h>
#include "DrawFunction.h"
#include "MassFunction.h"
#include <imgui.h>

const char kWindowTitle[] = "学籍番号";

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = {0};
	char preKeys[256] = {0};
	Vector3 rotate = { 0.0f, 0.0f, 0.0f };
	Vector3 traslate = { 0.0f, 0.0f, 0.0f };

	Vector3 cameraTranslate = { 0.0f, 1.9f, -6.49f };
	Vector3 cameraRotate = { 0.26f, 0.0f, 0.0f };
	Segment segment = { { -2.0f, -1.0f, 0.0f }, { 3.0f, 2.0f, 2.0f } };
	Vector3 point = { -1.5f, 0.6f, 0.6f };



	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		///
		/// ↓更新処理ここから
		///
		Vector3 project = Project(Subtract(point, segment.origin),segment.diff);
		Vector3 closestPoint = ClosestPoint(point, segment);
	


		ImGui::Begin("Transform");
		ImGui::SliderFloat3("rotate", &rotate.x, -3.14f, 3.14f);
		ImGui::SliderFloat3("traslate", &traslate.x, -10.0f, 10.0f);
		ImGui::SliderFloat3("cameraTranslate", &cameraTranslate.x, -10.0f, 10.0f);
		ImGui::InputFloat3("project", &project.x,"%.3f",ImGuiInputTextFlags_ReadOnly);
		//ImGui::SliderFloat3("cameraRotate", &cameraRotate.x, -3.14f, 3.14f);
		ImGui::End();
			

		
		Matrix4x4 worldMatrix = MakeAfineMatrix({ 1.0f, 1.0f, 1.0f }, rotate, traslate);
		//MatrixScreenPrintf(0, kRowHeight, worldMatrix, "worldMatrix");
		Matrix4x4 cameraMatrix = MakeAfineMatrix({ 1.0f, 1.0f, 1.0f },cameraRotate,cameraTranslate );
		//MatrixScreenPrintf(0, kRowHeight * 20, cameraMatrix, "cameraMatrix");
		Matrix4x4 viewMatrix = Inverse(cameraMatrix);
		//MatrixScreenPrintf(0, kRowHeight * 30, viewMatrix, "viewMatrix");
		Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(0.45f, static_cast<float>(kwindowWidth)/static_cast <float>(kwindowHight) , 0.1f, 100.0f);
		//MatrixScreenPrintf(kColumnWidth*10, kRowHeight  , projectionMatrix, "projectionMatrix");
		Matrix4x4 worldViewProjectionMatrix = Multiply( worldMatrix,Multiply(viewMatrix,projectionMatrix));
		//MatrixScreenPrintf(kColumnWidth*10, kRowHeight * 20, worldViewProjectionMatrix, "worldViewProjectionMatrix");
		Matrix4x4 viewportMatrix = MakeViewportMatrix(0.0f, 0.0f, static_cast<float>(kwindowWidth), static_cast<float>(kwindowHight), 0.0f, 1.0f);
		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///
		Sphere pointSphere = { point, 0.01f };
		Sphere closestPointSphere = { closestPoint, 0.01f };
		DrawSphere(pointSphere, worldViewProjectionMatrix, viewportMatrix, RED);
		DrawSphere(closestPointSphere, worldViewProjectionMatrix, viewportMatrix, BLACK);

		
		Vector3 screen[2]={};
		


			Vector3 nbcOrigin = Transform(segment.origin,worldViewProjectionMatrix );
			screen[0] = Transform(nbcOrigin, viewportMatrix);
		
			Vector3 nbcDiff = Transform(Add(segment.origin ,segment.diff),worldViewProjectionMatrix );
			screen[1] = Transform(nbcDiff, viewportMatrix);
			Novice::DrawLine(
				static_cast<int> (screen[0].x),
				static_cast<int> (screen[0].y),
				static_cast<int> (screen[1].x),
				static_cast<int> (screen[1].y),
				WHITE);

			DrawGrid(worldViewProjectionMatrix,viewportMatrix);
		


		///
		/// ↑描画処理ここまで
		///

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}
