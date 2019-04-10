#include "stdafx.h"
#include "Camera.h"
#include "GameObject.h"

/**************************************************************************
## Camera 생성자 ##
***************************************************************************/
Camera::Camera()//생성자 멤버 이니셜라이즈
	:mapSize((float)_WinSizeX, (float)_WinSizeY),pTarget(nullptr),zoomValue(1.0f),
	cameraSpeed(100.0f), _cameraState(CameraState::FreeCamera), shakeWay(1.0f), startCameraFromMoveDistance(StartCamera_MoveDistance)
{	
	//카메라 쉐이크 false 초기화
	isShake = false;
	//카메라 포지션 기본 값 초기화
	this->cameraPos = Vector2(0.f, 0.f);
	//카메라 렉트 초기화
	this->cameraRect = Figure::FloatRect(cameraPos.x, cameraPos.y, (float)_WinSizeX, (float)_WinSizeY);
	//마우스 포지션 저장용 기본 값 초기화
	this->saveMousePt = _Input->GetMousePosition();
	//카메라 흔드는 방향 바뀌는 딜레이 시간 기본 값 초기화
	this->ChangeShakeWayDelayTime = Camera_DefaultChangeShakeWayDelayTime;
}

/**************************************************************************
## Camera 소멸자 ##
***************************************************************************/
Camera::~Camera()
{
}

/**************************************************************************
## Camera Update ##
***************************************************************************/
void Camera::Update()
{
	//카메라 상태
	switch (_cameraState)
	{
	//카메라 상태 : 자유 카메라
	case Camera::FreeCamera:
		//자유 시점 카메라 상태에서 마우스 포지션으로 화면 이동하기
		this->MoveMouseToFreeCamera();
		break;
	//카메라 상태 : 타겟 카메라
	case Camera::None:
	case Camera::TargetCamera:
		this->UpdateTargetCamera();
		break;
	case Camera::End:
		break;
	default:
		break;
	}
}
/**************************************************************************
## Camera Render ##
***************************************************************************/
void Camera::Render()
{
}

/***************************************************************************
## GetRelativeCamera ##
카메라 상대좌표로 RECT / POINT / Vector2 보정
***************************************************************************/
Figure::FloatRect Camera::GetRelativeCamera(Figure::FloatRect rc)
{
	//카메라 이동 보정
	Figure::MoveFloatRect(&rc, -cameraPos.x, -cameraPos.y);
	//카메라 줌 보정
	rc.left = rc.left * zoomValue;
	rc.right = rc.right * zoomValue;
	rc.top = rc.top * zoomValue;
	rc.bottom = rc.bottom * zoomValue;
	return std::move(rc);
}
POINT Camera::GetRelativeCamera(POINT pt)
{
	//포지션으로 카메라 이동 보정
	pt.x -= cameraPos.x;
	pt.y -= cameraPos.y;
	//카메라 줌 보정
	pt.x = pt.x * zoomValue;
	pt.y = pt.y * zoomValue;

	return std::move(pt);
}
Vector2 Camera::GetRelativeCamera(Vector2 vector2)
{
	vector2.x -= cameraPos.x;
	vector2.y -= cameraPos.y;
	vector2 = vector2 * zoomValue;
	return std::move(vector2);
}

/***************************************************************************
## GetWorldVector2 ##
카메라 상대좌표를 역으로 더해서 월드 포지션을 반환
***************************************************************************/
Vector2 Camera::GetWorldVector2(Vector2 vector2)
{
	//곱한 줌을 나누고 뺀 포지션 x,y를 더하기
	return Vector2(vector2.x / zoomValue + cameraPos.x, vector2.y / zoomValue + cameraPos.y);
}
/***************************************************************************
## GetWorldViewCenter ##
카메라 상대좌표를 역으로 더하고 윈도우 크기의 중심을 더해 월드 포지션의 중앙 값을 반환
***************************************************************************/
Vector2 Camera::GetWorldViewCenter()
{
	return Vector2(cameraPos.x + (float)((_WinSizeX / 2) / zoomValue), cameraPos.y + (float)((_WinSizeY / 2) / zoomValue));
}




/***************************************************************************
## GetWorldMosueVector2 ##
카메라 영역 안의 마우스 포지션 Vector2로 반환
***************************************************************************/
Vector2 Camera::GetWorldMouseVector2()
{
	Vector2 mouse = _Input->GetMousePosition();
	//곱한 줌을 나누고 뺀 포지션 x,y를 더하기
	return Vector2(mouse.x / zoomValue + cameraPos.x, mouse.y / zoomValue + cameraPos.y);
}
/***************************************************************************
## GetWorldMousePoint ##
카메라 영역 안의 마우스 포지션 POINT로 반환
***************************************************************************/
POINT Camera::GetWorldMousePoint()
{
	Vector2 mouse = _Input->GetMousePosition();
	mouse = Vector2(mouse.x / zoomValue + cameraPos.x, mouse.y / zoomValue + cameraPos.y);
	return POINT{ ((LONG)mouse.x, (LONG)mouse.y) };
}

/***************************************************************************
## SetZoom ##
줌 수치 인자로 받아 세팅
***************************************************************************/
void Camera::SetZoom(float zoom)
{
	this->zoomValue = Math::Clampf(zoom, CameraZoomMin, CameraZoomMax);
}

/***************************************************************************
## SetTarget ##
타겟 대상을 인자로 받아 세팅
***************************************************************************/
void Camera::SetTarget(GameObject* object)
{
	if (object) 
	{
		//대상을 인자로
		this->pTarget = object;
		//카메라 기본 상태는 없음
		this->_cameraState = CameraState::None;
		//카메라 포지션은 object의 포지션의 화면 중심
		this->cameraPos = object->GetPosition() - Vector2(_WinSizeX / 2, _WinSizeY / 2);
		//카메라 보정
		this->ReviseCamera();
	}
}
/***************************************************************************
## ChangeTarget ##
카메라 타겟 변경
***************************************************************************/
void Camera::ChangeTarget(GameObject* object)
{
	this->pTarget = object;
	//카메라 상태는 없음 상태
	this->_cameraState = CameraState::None;
}
/***************************************************************************
## SetFreeCamera ##
자유 시점 카메라 상태로 변경 세팅
***************************************************************************/
void Camera::SetFreeCamera()
{
	//카메라 상태를 자유 시점 카메라로 변경
	_cameraState = CameraState::FreeCamera;
}

/***************************************************************************
## Shake ##
카메라 흔들기
***************************************************************************/
void Camera::Shake(float power, float shakeTime, float shakeWayDelayTime)
{
	//만일 shake상태가 false라면
	if (isShake == false) 
	{
		//Shake상태를 true로
		this->isShake = true;
		//흔들기 파워
		this->shakePower = power;
		//흔드는 시간, 저장용 흔드는 시간
		this->shakeTime = saveShakeTime = shakeTime;
		//흔드는 방향 바뀌는 딜레이 시간 
		this->ChangeShakeWayDelayTime = 0.f;
		//현재 흔드는 방햡 바뀌는 딜레이 시간
		this->currentChangeShakeWayDelayTime = shakeWayDelayTime;
	}
}
/***************************************************************************
## ShakingUpdate ##
카메라 흔들기
***************************************************************************/
void Camera::ShakingUpdate()
{
	if (isShake == true)
	{
		shakeTime -= _TimeManager->DeltaTime();
		currentChangeShakeWayDelayTime += _TimeManager->DeltaTime();
		//만일 현재 카메라 방향 변경 시간이 기존의 카메라 변경 시간 값보다 크다면
		if (currentChangeShakeWayDelayTime >= ChangeShakeWayDelayTime)
		{
			//카메라 방향 설정
			shakeWay = -1.0f * shakeWay;
			//현재 카메라 방향 변경 시간이 기존의 카메라 방향 변경 시간 값보다 크다면
			//기존 카메라 방향 변경 시간으로 동일하게 설정
			while (currentChangeShakeWayDelayTime >= ChangeShakeWayDelayTime)
				currentChangeShakeWayDelayTime -= ChangeShakeWayDelayTime;
		}

		shakePower -= (shakeTime / saveShakeTime) * _TimeManager->DeltaTime();
		//총 파워는 카메라 흔드는 파워 * 카메라 흔드는 방향
		float power = shakePower * shakeWay;

		//카메라 좌 우 흔들기
		//상 하 인자는 0.f로 현재 막아둠
		cameraPos += Vector2(power, 0.f);

		this->cameraRect.Update(cameraPos, Vector2(_WinSizeX, _WinSizeY), Pivot::LeftTop);
		//만일 흔드는 시간이 0보다 작거나 같아지면
		if (shakeTime <= 0.f)
		{
			//isShake는 false
			isShake = false;
			//카메라 흔드는 딜레이 시간 초기화
			ChangeShakeWayDelayTime = Camera_DefaultChangeShakeWayDelayTime;
		}
		//카메라 보정
		this->ReviseCamera();
	}
}

/***************************************************************************
## UpdateRenderRect ##
카메라 렉트에 대한 변경이 있을 경우 업데이트에서 렉트를 한번 더 정의해준다. 
***************************************************************************/
void Camera::UpdateRenderRect()
{
	this->cameraRect = Figure::FloatRect(cameraPos.x, cameraPos.y, (float)_WinSizeX, (float)_WinSizeY);
}

/***************************************************************************
## UpdateTargetCamera ##
타겟 위치로 카메라를 서서히 움직이는 기능
***************************************************************************/
void Camera::UpdateTargetCamera()
{
	if (pTarget) 
	{
		Vector2 centerPos(cameraPos.x + CastingFloat(_WinSizeX) * 0.5f, cameraPos.y + CastingFloat(_WinSizeY) * 0.5f);
		Vector2 toTarget = pTarget->GetPosition() - centerPos;

		switch (_cameraState)
		{
			case Camera::None:
			{
				float distance = Vector2::Length(&toTarget);
				//만일 toTarget의 길이가 카메라 시작 지점으로부터 이동 거리보다 크다면
				if (Vector2::Length(&toTarget) > startCameraFromMoveDistance)
				{
					//카메라 상태를 타겟 카메라 상태로 변경
					this->_cameraState = CameraState::TargetCamera;
				}
			}
			break;
				
			case Camera::TargetCamera: 
			{
				float length = Vector2::Length(&toTarget);
				//거리가 가까워질 수록 속도를 감소하기 위해
				//카메라 속도는 (타겟 거리 / (최대 거리 - 최소 거리)) * (최대 속도 - 최소 속도)
				this->cameraSpeed = (length / (CameraMaxDistance - CameraMinDistance)) * (CameraMaxSpeed - CameraMinSpeed);
				//스피드 값 보정 (Clampf 최대 최소 값을 넘어가거나 낮아지는 걸 보정)
				this->cameraSpeed = Math::Clampf(cameraSpeed, CameraMinSpeed, CameraMaxSpeed);
				//Normalize로 대상의 방향을 구한 뒤 카메라 속도를 곱하고 나온 값을 델타 타임 화면에 한 번 그리는 시간을 적용해 대입한다.
				centerPos += Vector2::Normalize(&toTarget) * cameraSpeed * _TimeManager->DeltaTime();
				//중심 좌표를 기존 카메라 좌표인 0,0 left, top으로 보냄
				this->cameraPos = centerPos - Vector2(_WinSizeX / 2, _WinSizeY / 2);
				this->cameraRect = Figure::FloatRect(cameraPos, Vector2(_WinSizeX, _WinSizeY), Pivot::LeftTop);
			
				if (Vector2::Length(&(pTarget->GetPosition() - centerPos)) <= 50.f)
				{
					//카메라 상태를 기본 상태로 변경
					this->_cameraState = CameraState::None;
					//중심 좌표를 기존 카메라 좌표인 0,0 left, top으로 보냄
					this->cameraPos = centerPos - Vector2(_WinSizeX / 2, _WinSizeY / 2);
					//카메라 렉트 포지션이 바뀌었으므로 카메라 렉트 재정의
					this->cameraRect.Update(cameraPos, Vector2(_WinSizeX, _WinSizeY), Pivot::LeftTop);
				}
				//카메라 보정
				this->ReviseCamera();
				
			}
			break;
		}
		//
		this->ShakingUpdate();
	}

}

/***************************************************************************
## ReviseCamera ##
카메라 보정 : 카메라가 화면 밖으로 나가면 카메라 이동시켜 다시 안으로 보정  
***************************************************************************/
void Camera::ReviseCamera()
{
	//만일 카메라 x좌표가 0보다 낮으면 (=화면 밖으로 나가면)
	if (cameraPos.x < 0.f) 
	{	
		//카메라 x좌표만큼 다시 그 값만큼 보정
		cameraPos.x -= cameraPos.x;
		//포지션이 변동되므로 렉트 재정의
		this->cameraRect.Update(cameraPos, Vector2(_WinSizeX, _WinSizeY), Pivot::LeftTop);
	}
	//만일 카메라 렉트 길이가 맵 가로 길이보다 크다면 
	else if (cameraRect.right > (LONG)mapSize.x) 
	{
		//카메라 x좌표에 카메라 렉트 가로 길이 - 맵 가로 길이한만큼 값 보정
		cameraPos.x -= (cameraRect.right) - mapSize.x;
		//포지션이 변동되므로 렉트 재정의
		this->cameraRect.Update(cameraPos, Vector2(_WinSizeX, _WinSizeY), Pivot::LeftTop);
	}

	//만일 카메라 렉트 높이가 0보다 낮으면 (=화면 밖으로 나가면)
	if (cameraRect.top < 0) 
	{
		//카메라 y좌표만큼 다시 그 값만큼 보정
		cameraPos.y -= cameraPos.y;
		//포지션이 변동되므로 렉트 재정의
		this->cameraRect.Update(cameraPos, Vector2(_WinSizeX, _WinSizeY), Pivot::LeftTop);
	}
	//만일 카메라 렉트 세로 길이가 맵 세로 길이보다 크다면 
	else if (cameraRect.bottom > (LONG)mapSize.y) 
	{
		//카메라 y좌표에 카메라 렉트 세로 길이 - 맵 세로 길이한만큼 값 보정
		cameraPos.y -= (cameraRect.bottom) - mapSize.y;
		//포지션이 변동되므로 렉트 재정의
		this->cameraRect.Update(cameraPos, Vector2(_WinSizeX, _WinSizeY), Pivot::LeftTop);
	}
}

/***************************************************************************
## MoveMouseToFreeCamera ##
자유 시점 카메라에서 마우스 우클릭을 누르고 마우스를 움직이면
카메라 화면 이동 기능
***************************************************************************/
void Camera::MoveMouseToFreeCamera()
{
	//현재 마우스 포지션 받기
	Vector2 currentMousePt = _Input->GetMousePosition();
	//만일 마우스 우클릭을 하고 있으면
	if (_Input->GetKey(VK_RBUTTON))
	{
		//만일 등록한 마우스 포지션과 현재 마우스 포지션이 다르면
		if (this->saveMousePt != currentMousePt)
		{
			//카메라 포지션에 마우스 포지션 보정 값 대입
			this->cameraPos += (saveMousePt - currentMousePt) / zoomValue;
			//카메라 위치가 바뀌었으므로 카메라 렉트 한번 더 정의
			this->UpdateRenderRect();
			//저장용 마우스 포지션에 현재 마우스 포지션 대입
			//값을 보정해서 적용이 되면 포지션 동일하게 맞춰서 카메라 포지션 보정 그만하기
			saveMousePt = currentMousePt;
		}
	}
	//저장용 마우스 포지션에 현재 마우스 포지션 대입
	//값을 보정해서 적용이 되면 포지션 동일하게 맞춰서 카메라 포지션 보정 그만하기
	saveMousePt = currentMousePt;
}


/***************************************************************************
## CameraMouseWheelProc ##
카메라 마우스 휠 사용하기 위한 프로시져
***************************************************************************/
void Camera::CameraMouseWheelProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (_cameraState == CameraState::FreeCamera) 
	{
		//마우스 휠 올리기
		if ((SHORT)HIWORD(wParam) > 0) 
		{
			this->zoomValue += 0.1f;
			this->zoomValue = Math::Clampf(zoomValue, CameraZoomMin, CameraZoomMax);
		}
		//마우스 휠 내리기
		else 
		{
			this->zoomValue -= 0.1f;
			this->zoomValue = Math::Clampf(zoomValue, CameraZoomMin, CameraZoomMax);
		}
	}
}


/***************************************************************************
## InitCameraToGameOjbect ##
//카메라 타겟 지정 및 카메라 줌 값 초기화
***************************************************************************/
void Camera::InitCameraToGameOjbect(GameObject* object, string keyName)
{
	object = _ObjectManager->FindObject(ObjectType::Object, keyName);
	if (object)
	{
		this->SetTarget(object);
		zoomValue = 1.0f;
	}
}