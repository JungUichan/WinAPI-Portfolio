#pragma once
/******************************************************
## SceneManager ## �̱���
*******************************************************/
class SceneManager final : public Singletonbase<SceneManager>
{
public:
	string nextSceneName;	//���� �� �̸�
private:
	//�� ���� �̳�
	enum class SceneState 
	{
		None = 0, 
		FadeOut, //���̵� �ƿ�
		FadeIn,	//���̵� ��
		Load,
		End
	};



	//�� ����Ʈ
	//first�� Ű ��, second�� SceneBase������ �ּ�
	unordered_map<string, class SceneBase*> _sceneList;
	//���ͷ����� ��
	//first�� Ű ��, second�� SceneBase������ �ּ�
	typedef unordered_map<string, class SceneBase*>::iterator SceneIter;


	//�ε� �� ����Ʈ
	//first�� Ű ��, second�� SceneBase������ �ּ�
	unordered_map<string, class SceneBase*> _loadingSceneList;

	class SceneBase* _nowScene;	//���� ��
	
	function<void(string, bool)> _loadFunc;	//���ڿ� bool���� �Լ��� �޴� loadFunc����
	string _loadSceneName;	//�ε� �� �̸� Ű ��
	
	bool _isInit;
	
	SceneState _sceneState;	//�� ����
	float _fadeAlpha;		//����Ʈ �� �ƿ��� ���İ�

private:
	//�� �ٲٱ�
	void ChangeScene(string name, bool init);
public:
	SceneManager();
	~SceneManager();

	void Update();
	void Render();

	void SceneQueue();

	//�� �߰�
	void AddScene(string name, class SceneBase* scene);
	
	//�� ã�Ƽ� �������
	class SceneBase* FindScene(string name);
	
	//�� �ҷ�����
	void LoadScene(string name, bool init = true);

};

#define _SceneManager SceneManager::Get()