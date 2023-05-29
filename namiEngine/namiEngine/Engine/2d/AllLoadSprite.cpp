#include "AllLoadSprite.h"

void AllLoadSprite::AllLoadTexture()
{
	//タイトルシーン
	if (!Sprite::LoadTexture(1, L"Resources/base.png")) {
		assert(0);
		return;
	}
	if (!Sprite::LoadTexture(2, L"Resources/fadeSprite.png")) {
		assert(0);
		return;
	}
	if (!Sprite::LoadTexture(3, L"Resources/tutorial1.png")) {
		assert(0);
		return;
	}
	if (!Sprite::LoadTexture(4, L"Resources/tutorial2.png")) {
		assert(0);
		return;
	}
	if (!Sprite::LoadTexture(5, L"Resources/title.png")) {
		assert(0);
		return;
	}
	if (!Sprite::LoadTexture(6, L"Resources/title2.png")) {
		assert(0);
		return;
	}

	//ゲームシーン
	if (!Sprite::LoadTexture(7, L"Resources/0.png")) {
		assert(0);
		return;
	}
	if (!Sprite::LoadTexture(8, L"Resources/1.png")) {
		assert(0);
		return;
	}
	if (!Sprite::LoadTexture(9, L"Resources/2.png")) {
		assert(0);
		return;
	}
	if (!Sprite::LoadTexture(10, L"Resources/3.png")) {
		assert(0);
		return;
	}

	//ゲームクリアシーン
	if (!Sprite::LoadTexture(11, L"Resources/gameclear.png")) {
		assert(0);
		return;
	}
	if (!Sprite::LoadTexture(12, L"Resources/selectRetry.png")) {
		assert(0);
		return;
	}
	if (!Sprite::LoadTexture(13, L"Resources/noSelectRetry.png")) {
		assert(0);
		return;
	}
	if (!Sprite::LoadTexture(14, L"Resources/selectTitle.png")) {
		assert(0);
		return;
	}
	if (!Sprite::LoadTexture(15, L"Resources/noSelectTitle.png")) {
		assert(0);
		return;
	}

	//ゲームオーバーシーン
	if (!Sprite::LoadTexture(16, L"Resources/gameover.png")) {
		assert(0);
		return;
	}

}
