#include "NamiGame.h"

//# Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	NamiGame game;
	game.Initialize();
	while (true)  // ゲームループ
	{
		game.Update();

		//終了処理が入ったら
		if (game.GetIsEnd()) {
			break;
		}

		game.Draw();
	}

	game.Finalize();

	return 0;
}