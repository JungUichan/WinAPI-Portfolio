#pragma once
/******************************************************
## SceneManager ## 싱글톤
*******************************************************/
class SceneManager final : public Singletonbase<SceneManager>
{
public:
	string nextSceneName;	//다음 씬 이름
private:
	//씬 상태 이넘
	enum class SceneState 
	{
		None = 0, 
		FadeOut, //페이드 아웃
		FadeIn,	//페이드 인
		Load,
		End
	};



	//씬 리스트
	//first는 키 값, second는 SceneBase포인터 주소
	unordered_map<string, class SceneBase*> _sceneList;
	//이터레이터 씬
	//first는 키 값, second는 SceneBase포인터 주소
	typedef unordered_map<string, class SceneBase*>::iterator SceneIter;


	//로딩 씬 리스트
	//first는 키 값, second는 SceneBase포인터 주소
	unordered_map<string, class SceneBase*> _loadingSceneList;

	class SceneBase* _nowScene;	//현재 씬
	
	function<void(string, bool)> _loadFunc;	//문자와 bool값을 함수로 받는 loadFunc변수
	string _loadSceneName;	//로딩 씬 이름 키 값
	
	bool _isInit;
	
	SceneState _sceneState;	//씬 상태
	float _fadeAlpha;		//페이트 인 아웃용 알파값

private:
	//씬 바꾸기
	void ChangeScene(string name, bool init);
public:
	SceneManager();
	~SceneManager();

	void Update();
	void Render();

	void SceneQueue();

	//씬 추가
	void AddScene(string name, class SceneBase* scene);
	
	//씬 찾아서 갖고오기
	class SceneBase* FindScene(string name);
	
	//씬 불러오기
	void LoadScene(string name, bool init = true);

};

#define _SceneManager SceneManager::Get()