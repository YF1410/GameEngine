#pragma once
#include "WinApp.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Audio.h"
class Framework
{
public:
	//���s
	void Run();
	//������
	virtual void Initialize();
	//�I��
	virtual void Finalize();
	//���t���[���X�V
	virtual void Update();
	//�`��
	virtual void Draw() = 0;
protected:
	bool isEnd_ = false;
	std::unique_ptr<WinApp> win;
	std::unique_ptr<DirectXCommon> dxCommon;
	std::unique_ptr<Input> input;
	std::unique_ptr<Audio> audio;
};

