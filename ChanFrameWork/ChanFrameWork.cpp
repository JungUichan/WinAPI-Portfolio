// ChanFrameWork.cpp : 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "ChanFrameWork.h"

//F1으로 디버그 상태 On/Off
bool _isDebug = false;	//isDebug false

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	//윈도우 정보 구조체 
	Window::Desc desc;
	desc.appName = L"Chan";	//윈도우 창 이름
	desc.instance = hInstance;			//윈도우 인스턴스
	desc.bFullScreen = false;			//전체화면 여부				//수직동기화 여부
	desc.handle = NULL;					//윈도우 핸들러 
	desc.width = 1280;				//윈도우 창 가로길이
	desc.height = 760;				//윈도우 창 세로길이				//툴작업 용으로 로드 여부 
	desc.command = nCmdShow;				//윈도우 커맨드 값

	Window::SetDesc(&desc);		//다 초기화 했으면 세팅해준다 
	srand((UINT)time(0));				//타임시드 초기화 

	Window* window = new Window();		//윈도우 클래스 생성 -> 생성자에서 윈도우 창 생성 및 레지스터 등록한다.
	WPARAM wParam = window->Run();		//이곳에서 게임의 메인 루프가 돈다.
	SafeDelete(window);					//여기까지 왔다는 것은 메인루프가 끝났다는 뜻이므로 윈도우 클래스 삭제 

	return wParam;						//여기까지 왔으면 윈API종료 
}


