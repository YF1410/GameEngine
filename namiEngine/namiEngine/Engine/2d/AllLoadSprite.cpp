#include "AllLoadSprite.h"

void AllLoadSprite::AllLoadTexture()
{
	//�^�C�g���V�[��
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

	//�Q�[���V�[��
	if (!Sprite::LoadTexture(20, L"Resources/0.png")) {
		assert(0);
		return;
	}
	if (!Sprite::LoadTexture(21, L"Resources/1.png")) {
		assert(0);
		return;
	}
	if (!Sprite::LoadTexture(22, L"Resources/2.png")) {
		assert(0);
		return;
	}
	if (!Sprite::LoadTexture(23, L"Resources/3.png")) {
		assert(0);
		return;
	}

	//�Q�[���N���A�V�[��
	if (!Sprite::LoadTexture(41, L"Resources/gameclear.png")) {
		assert(0);
		return;
	}
	if (!Sprite::LoadTexture(42, L"Resources/selectRetry.png")) {
		assert(0);
		return;
	}
	if (!Sprite::LoadTexture(43, L"Resources/noSelectRetry.png")) {
		assert(0);
		return;
	}
	if (!Sprite::LoadTexture(44, L"Resources/selectTitle.png")) {
		assert(0);
		return;
	}
	if (!Sprite::LoadTexture(45, L"Resources/noSelectTitle.png")) {
		assert(0);
		return;
	}

	//�Q�[���I�[�o�[�V�[��
	if (!Sprite::LoadTexture(46, L"Resources/gameover.png")) {
		assert(0);
		return;
	}

}
