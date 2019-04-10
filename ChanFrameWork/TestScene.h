#pragma once
#include "SceneBase.h"
class TestScene : public SceneBase
{
private:
	class Image* _map;
public:
	TestScene() {}
	~TestScene() {}

	void Init()override;
	void Release()override;
	void Update()override;
	void Render()override;

private:
	void SceneInit();
};

