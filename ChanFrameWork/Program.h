#pragma once
/****************************************************************************
## Program ##
*****************************************************************************/
class Program
{
private:
	class Image* image;
public:
	Program();
	~Program();

	void Init();
	void Release();
	void Update();
	void Render();
};

