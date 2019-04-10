#pragma once



#define CameraMaxSpeed 400.0f						//ī�޶� �ִ� �ӵ�
#define CameraMinSpeed 100.0f						//ī�޶� �ּ� �ӵ�
#define CameraMaxDistance 250.0f					//ī�޶� �ִ� �Ÿ�
#define CameraMinDistance 30.0f						//ī�޶� �ּ� �Ÿ�

#define CameraZoomMax 2.5f							//ī�޶� �ִ� �� ��
#define CameraZoomMin 0.5f							//ī�޶� �ּ� �� ��

#define StartCamera_MoveDistance 30.f				//ī�޶� ���� ��ġ�κ��� �̵� �Ÿ� ��

#define Camera_DefaultShakePower 2.58f				//ī�޶� ���� �⺻ �Ŀ�
#define Camera_DefaultShakeTime 0.2f				//ī�޶� ���� �⺻ �ð�
#define Camera_DefaultChangeShakeWayDelayTime 0.043f//ī�޶� ���� �ٲ�� ������ �ð�


/**************************************************************************
## Camera �̱��� ##
***************************************************************************/
class Camera final : public Singletonbase<Camera> 
{
public:
	//ī�޶� ����
	enum CameraState
	{
		FreeCamera = 0,	//���� ī�޶�
		TargetCamera,	//Ÿ�� �߽� ī�޶�
		None,			//ī�޶� ����
		End				
	};
private:
	CameraState _cameraState;						//ī�޶� ����
	float zoomValue;								//�� ��
	Figure::FloatRect cameraRect;					//ī�޶� ��Ʈ
	Synthesize(Vector2, cameraPos, CameraPos)		//ī�޶� ������, ī�޶� ������ GET, SET �Լ� ����
	Synthesize(Vector2, mapSize, MapSize)			//�� ������, �� ������ GET, SET �Լ� ���� 
	Synthesize(float, cameraSpeed, CameraSpeed)		//ī�޶� ���ǵ�, ī�޶� ���ǵ� GET, SET �Լ� ����
	class GameObject* pTarget;						//������ Ÿ�� ���� Ŭ����
	Vector2 saveMousePt;							//���콺 ��ǥ ����� (���� ���� ī�޶�)  
	
	float shakePower;								//ī�޶� ���� �Ŀ�
	float shakeTime;								//���� �ð�
	float saveShakeTime;							//���� �ð� �����

	float startCameraFromMoveDistance;				//ī�޶� ���� ��ġ�κ��� �̵� �Ÿ�
	float currentChangeShakeWayDelayTime;			//���� ���� ���� �ٲ�� ������ �ð�
	float ChangeShakeWayDelayTime;					//���� ���� �ٲ�� ������ �ð�
	float shakeWay;									//���� ���� (�¿�, ���� ��)

	bool isShake;									//���� true/false

public:
	Camera();
	~Camera();

	void Update();
	void Render();

	//ī�޶� ���� �⺻ �ʱ�ȭ
	void InitCameraToGameOjbect(class GameObject* object, string keyName);

	//##### ī�޶� ��� ��ǥ ���� �Լ� ######
	Figure::FloatRect GetRelativeCamera(Figure::FloatRect rc);
	POINT GetRelativeCamera(POINT pt);
	Vector2 GetRelativeCamera(Vector2 vector2);

	//Vector2�� �޴� ���� ��ǥ
	Vector2 GetWorldVector2(Vector2 vector2);
	
	//���콺 ���� ��ǥ
	Vector2 GetWorldMouseVector2();
	POINT GetWorldMousePoint();

	//Vector2�� �޴� ���� �߽� ��ǥ
	Vector2 GetWorldViewCenter();
	
	//�� ��������
	float GetZoom()const { return zoomValue; }
	//�� ���� 
	void SetZoom(float zoom);

	//Ÿ�� ����
	void SetTarget(class GameObject* object);
	//Ÿ�� ����
	void ChangeTarget(class GameObject* object);

	//ī�޶� �̵� �Ÿ� ����
	void SetCameraMoveDistance(float distance = StartCamera_MoveDistance) { startCameraFromMoveDistance = distance; }
	
	//ī�޶� ����
	void Shake(float power = Camera_DefaultShakePower,
		float shakeTime = Camera_DefaultShakeTime,
		float shakeWayDelayTime = Camera_DefaultChangeShakeWayDelayTime);

	//ī�޶� ���� ����
	void SetCameraState(CameraState state) { this->_cameraState = state; }
	//���� ���� ī�޶� ����
	void SetFreeCamera();

	//ī�޶� ���콺 �� ���ν���
	void CameraMouseWheelProc(UINT message, WPARAM wParam, LPARAM lParam);

private:
	//ī�޶� ��Ʈ ������Ʈ
	void UpdateRenderRect();

	//���� ���� ī�޶� ������Ʈ
	void MoveMouseToFreeCamera();

	//Ÿ�� ���� ī�޶� ������Ʈ
	void UpdateTargetCamera();

	//ī�޶� ���� (ȭ�� ������ ������ ����)
	void ReviseCamera();

	//ī�޶� ���� ������Ʈ
	void ShakingUpdate();
};

#define _CameraManager Camera::Get()