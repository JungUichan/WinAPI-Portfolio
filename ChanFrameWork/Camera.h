#pragma once



#define CameraMaxSpeed 400.0f						//카메라 최대 속도
#define CameraMinSpeed 100.0f						//카메라 최소 속도
#define CameraMaxDistance 250.0f					//카메라 최대 거리
#define CameraMinDistance 30.0f						//카메라 최소 거리

#define CameraZoomMax 2.5f							//카메라 최대 줌 값
#define CameraZoomMin 0.5f							//카메라 최소 줌 값

#define StartCamera_MoveDistance 30.f				//카메라 시작 위치로부터 이동 거리 값

#define Camera_DefaultShakePower 2.58f				//카메라 흔드는 기본 파워
#define Camera_DefaultShakeTime 0.2f				//카메라 흔드는 기본 시간
#define Camera_DefaultChangeShakeWayDelayTime 0.043f//카메라 방향 바뀌는 딜레이 시간


/**************************************************************************
## Camera 싱글톤 ##
***************************************************************************/
class Camera final : public Singletonbase<Camera> 
{
public:
	//카메라 상태
	enum CameraState
	{
		FreeCamera = 0,	//자유 카메라
		TargetCamera,	//타겟 중심 카메라
		None,			//카메라 없음
		End				
	};
private:
	CameraState _cameraState;						//카메라 상태
	float zoomValue;								//줌 값
	Figure::FloatRect cameraRect;					//카메라 렉트
	Synthesize(Vector2, cameraPos, CameraPos)		//카메라 포지션, 카메라 포지션 GET, SET 함수 생성
	Synthesize(Vector2, mapSize, MapSize)			//맵 사이즈, 맵 사이즈 GET, SET 함수 생성 
	Synthesize(float, cameraSpeed, CameraSpeed)		//카메라 스피드, 카메라 스피드 GET, SET 함수 생성
	class GameObject* pTarget;						//포인터 타겟 지정 클래스
	Vector2 saveMousePt;							//마우스 좌표 저장용 (자유 시점 카메라)  
	
	float shakePower;								//카메라 흔드는 파워
	float shakeTime;								//흔드는 시간
	float saveShakeTime;							//흔드는 시간 저장용

	float startCameraFromMoveDistance;				//카메라 시작 위치로부터 이동 거리
	float currentChangeShakeWayDelayTime;			//현재 흔드는 방향 바뀌는 딜레이 시간
	float ChangeShakeWayDelayTime;					//흔드는 방향 바뀌는 딜레이 시간
	float shakeWay;									//흔드는 방향 (좌우, 상하 등)

	bool isShake;									//흔들기 true/false

public:
	Camera();
	~Camera();

	void Update();
	void Render();

	//카메라 정보 기본 초기화
	void InitCameraToGameOjbect(class GameObject* object, string keyName);

	//##### 카메라 상대 좌표 보정 함수 ######
	Figure::FloatRect GetRelativeCamera(Figure::FloatRect rc);
	POINT GetRelativeCamera(POINT pt);
	Vector2 GetRelativeCamera(Vector2 vector2);

	//Vector2로 받는 월드 좌표
	Vector2 GetWorldVector2(Vector2 vector2);
	
	//마우스 월드 좌표
	Vector2 GetWorldMouseVector2();
	POINT GetWorldMousePoint();

	//Vector2로 받는 월드 중심 좌표
	Vector2 GetWorldViewCenter();
	
	//줌 가져오기
	float GetZoom()const { return zoomValue; }
	//줌 세팅 
	void SetZoom(float zoom);

	//타겟 세팅
	void SetTarget(class GameObject* object);
	//타겟 변경
	void ChangeTarget(class GameObject* object);

	//카메라 이동 거리 세팅
	void SetCameraMoveDistance(float distance = StartCamera_MoveDistance) { startCameraFromMoveDistance = distance; }
	
	//카메라 흔들기
	void Shake(float power = Camera_DefaultShakePower,
		float shakeTime = Camera_DefaultShakeTime,
		float shakeWayDelayTime = Camera_DefaultChangeShakeWayDelayTime);

	//카메라 상태 세팅
	void SetCameraState(CameraState state) { this->_cameraState = state; }
	//자유 시점 카메라 세팅
	void SetFreeCamera();

	//카메라 마우스 휠 프로시져
	void CameraMouseWheelProc(UINT message, WPARAM wParam, LPARAM lParam);

private:
	//카메라 렉트 업데이트
	void UpdateRenderRect();

	//자유 시점 카메라 업데이트
	void MoveMouseToFreeCamera();

	//타겟 시점 카메라 업데이트
	void UpdateTargetCamera();

	//카메라 보정 (화면 밖으로 나가면 보정)
	void ReviseCamera();

	//카메라 흔들기 업데이트
	void ShakingUpdate();
};

#define _CameraManager Camera::Get()