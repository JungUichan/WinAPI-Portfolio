#include "stdafx.h"
#include "Camera.h"
#include "GameObject.h"

/**************************************************************************
## Camera ������ ##
***************************************************************************/
Camera::Camera()//������ ��� �̴ϼȶ�����
	:mapSize((float)_WinSizeX, (float)_WinSizeY),pTarget(nullptr),zoomValue(1.0f),
	cameraSpeed(100.0f), _cameraState(CameraState::FreeCamera), shakeWay(1.0f), startCameraFromMoveDistance(StartCamera_MoveDistance)
{	
	//ī�޶� ����ũ false �ʱ�ȭ
	isShake = false;
	//ī�޶� ������ �⺻ �� �ʱ�ȭ
	this->cameraPos = Vector2(0.f, 0.f);
	//ī�޶� ��Ʈ �ʱ�ȭ
	this->cameraRect = Figure::FloatRect(cameraPos.x, cameraPos.y, (float)_WinSizeX, (float)_WinSizeY);
	//���콺 ������ ����� �⺻ �� �ʱ�ȭ
	this->saveMousePt = _Input->GetMousePosition();
	//ī�޶� ���� ���� �ٲ�� ������ �ð� �⺻ �� �ʱ�ȭ
	this->ChangeShakeWayDelayTime = Camera_DefaultChangeShakeWayDelayTime;
}

/**************************************************************************
## Camera �Ҹ��� ##
***************************************************************************/
Camera::~Camera()
{
}

/**************************************************************************
## Camera Update ##
***************************************************************************/
void Camera::Update()
{
	//ī�޶� ����
	switch (_cameraState)
	{
	//ī�޶� ���� : ���� ī�޶�
	case Camera::FreeCamera:
		//���� ���� ī�޶� ���¿��� ���콺 ���������� ȭ�� �̵��ϱ�
		this->MoveMouseToFreeCamera();
		break;
	//ī�޶� ���� : Ÿ�� ī�޶�
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
ī�޶� �����ǥ�� RECT / POINT / Vector2 ����
***************************************************************************/
Figure::FloatRect Camera::GetRelativeCamera(Figure::FloatRect rc)
{
	//ī�޶� �̵� ����
	Figure::MoveFloatRect(&rc, -cameraPos.x, -cameraPos.y);
	//ī�޶� �� ����
	rc.left = rc.left * zoomValue;
	rc.right = rc.right * zoomValue;
	rc.top = rc.top * zoomValue;
	rc.bottom = rc.bottom * zoomValue;
	return std::move(rc);
}
POINT Camera::GetRelativeCamera(POINT pt)
{
	//���������� ī�޶� �̵� ����
	pt.x -= cameraPos.x;
	pt.y -= cameraPos.y;
	//ī�޶� �� ����
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
ī�޶� �����ǥ�� ������ ���ؼ� ���� �������� ��ȯ
***************************************************************************/
Vector2 Camera::GetWorldVector2(Vector2 vector2)
{
	//���� ���� ������ �� ������ x,y�� ���ϱ�
	return Vector2(vector2.x / zoomValue + cameraPos.x, vector2.y / zoomValue + cameraPos.y);
}
/***************************************************************************
## GetWorldViewCenter ##
ī�޶� �����ǥ�� ������ ���ϰ� ������ ũ���� �߽��� ���� ���� �������� �߾� ���� ��ȯ
***************************************************************************/
Vector2 Camera::GetWorldViewCenter()
{
	return Vector2(cameraPos.x + (float)((_WinSizeX / 2) / zoomValue), cameraPos.y + (float)((_WinSizeY / 2) / zoomValue));
}




/***************************************************************************
## GetWorldMosueVector2 ##
ī�޶� ���� ���� ���콺 ������ Vector2�� ��ȯ
***************************************************************************/
Vector2 Camera::GetWorldMouseVector2()
{
	Vector2 mouse = _Input->GetMousePosition();
	//���� ���� ������ �� ������ x,y�� ���ϱ�
	return Vector2(mouse.x / zoomValue + cameraPos.x, mouse.y / zoomValue + cameraPos.y);
}
/***************************************************************************
## GetWorldMousePoint ##
ī�޶� ���� ���� ���콺 ������ POINT�� ��ȯ
***************************************************************************/
POINT Camera::GetWorldMousePoint()
{
	Vector2 mouse = _Input->GetMousePosition();
	mouse = Vector2(mouse.x / zoomValue + cameraPos.x, mouse.y / zoomValue + cameraPos.y);
	return POINT{ ((LONG)mouse.x, (LONG)mouse.y) };
}

/***************************************************************************
## SetZoom ##
�� ��ġ ���ڷ� �޾� ����
***************************************************************************/
void Camera::SetZoom(float zoom)
{
	this->zoomValue = Math::Clampf(zoom, CameraZoomMin, CameraZoomMax);
}

/***************************************************************************
## SetTarget ##
Ÿ�� ����� ���ڷ� �޾� ����
***************************************************************************/
void Camera::SetTarget(GameObject* object)
{
	if (object) 
	{
		//����� ���ڷ�
		this->pTarget = object;
		//ī�޶� �⺻ ���´� ����
		this->_cameraState = CameraState::None;
		//ī�޶� �������� object�� �������� ȭ�� �߽�
		this->cameraPos = object->GetPosition() - Vector2(_WinSizeX / 2, _WinSizeY / 2);
		//ī�޶� ����
		this->ReviseCamera();
	}
}
/***************************************************************************
## ChangeTarget ##
ī�޶� Ÿ�� ����
***************************************************************************/
void Camera::ChangeTarget(GameObject* object)
{
	this->pTarget = object;
	//ī�޶� ���´� ���� ����
	this->_cameraState = CameraState::None;
}
/***************************************************************************
## SetFreeCamera ##
���� ���� ī�޶� ���·� ���� ����
***************************************************************************/
void Camera::SetFreeCamera()
{
	//ī�޶� ���¸� ���� ���� ī�޶�� ����
	_cameraState = CameraState::FreeCamera;
}

/***************************************************************************
## Shake ##
ī�޶� ����
***************************************************************************/
void Camera::Shake(float power, float shakeTime, float shakeWayDelayTime)
{
	//���� shake���°� false���
	if (isShake == false) 
	{
		//Shake���¸� true��
		this->isShake = true;
		//���� �Ŀ�
		this->shakePower = power;
		//���� �ð�, ����� ���� �ð�
		this->shakeTime = saveShakeTime = shakeTime;
		//���� ���� �ٲ�� ������ �ð� 
		this->ChangeShakeWayDelayTime = 0.f;
		//���� ���� ���u �ٲ�� ������ �ð�
		this->currentChangeShakeWayDelayTime = shakeWayDelayTime;
	}
}
/***************************************************************************
## ShakingUpdate ##
ī�޶� ����
***************************************************************************/
void Camera::ShakingUpdate()
{
	if (isShake == true)
	{
		shakeTime -= _TimeManager->DeltaTime();
		currentChangeShakeWayDelayTime += _TimeManager->DeltaTime();
		//���� ���� ī�޶� ���� ���� �ð��� ������ ī�޶� ���� �ð� ������ ũ�ٸ�
		if (currentChangeShakeWayDelayTime >= ChangeShakeWayDelayTime)
		{
			//ī�޶� ���� ����
			shakeWay = -1.0f * shakeWay;
			//���� ī�޶� ���� ���� �ð��� ������ ī�޶� ���� ���� �ð� ������ ũ�ٸ�
			//���� ī�޶� ���� ���� �ð����� �����ϰ� ����
			while (currentChangeShakeWayDelayTime >= ChangeShakeWayDelayTime)
				currentChangeShakeWayDelayTime -= ChangeShakeWayDelayTime;
		}

		shakePower -= (shakeTime / saveShakeTime) * _TimeManager->DeltaTime();
		//�� �Ŀ��� ī�޶� ���� �Ŀ� * ī�޶� ���� ����
		float power = shakePower * shakeWay;

		//ī�޶� �� �� ����
		//�� �� ���ڴ� 0.f�� ���� ���Ƶ�
		cameraPos += Vector2(power, 0.f);

		this->cameraRect.Update(cameraPos, Vector2(_WinSizeX, _WinSizeY), Pivot::LeftTop);
		//���� ���� �ð��� 0���� �۰ų� ��������
		if (shakeTime <= 0.f)
		{
			//isShake�� false
			isShake = false;
			//ī�޶� ���� ������ �ð� �ʱ�ȭ
			ChangeShakeWayDelayTime = Camera_DefaultChangeShakeWayDelayTime;
		}
		//ī�޶� ����
		this->ReviseCamera();
	}
}

/***************************************************************************
## UpdateRenderRect ##
ī�޶� ��Ʈ�� ���� ������ ���� ��� ������Ʈ���� ��Ʈ�� �ѹ� �� �������ش�. 
***************************************************************************/
void Camera::UpdateRenderRect()
{
	this->cameraRect = Figure::FloatRect(cameraPos.x, cameraPos.y, (float)_WinSizeX, (float)_WinSizeY);
}

/***************************************************************************
## UpdateTargetCamera ##
Ÿ�� ��ġ�� ī�޶� ������ �����̴� ���
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
				//���� toTarget�� ���̰� ī�޶� ���� �������κ��� �̵� �Ÿ����� ũ�ٸ�
				if (Vector2::Length(&toTarget) > startCameraFromMoveDistance)
				{
					//ī�޶� ���¸� Ÿ�� ī�޶� ���·� ����
					this->_cameraState = CameraState::TargetCamera;
				}
			}
			break;
				
			case Camera::TargetCamera: 
			{
				float length = Vector2::Length(&toTarget);
				//�Ÿ��� ������� ���� �ӵ��� �����ϱ� ����
				//ī�޶� �ӵ��� (Ÿ�� �Ÿ� / (�ִ� �Ÿ� - �ּ� �Ÿ�)) * (�ִ� �ӵ� - �ּ� �ӵ�)
				this->cameraSpeed = (length / (CameraMaxDistance - CameraMinDistance)) * (CameraMaxSpeed - CameraMinSpeed);
				//���ǵ� �� ���� (Clampf �ִ� �ּ� ���� �Ѿ�ų� �������� �� ����)
				this->cameraSpeed = Math::Clampf(cameraSpeed, CameraMinSpeed, CameraMaxSpeed);
				//Normalize�� ����� ������ ���� �� ī�޶� �ӵ��� ���ϰ� ���� ���� ��Ÿ Ÿ�� ȭ�鿡 �� �� �׸��� �ð��� ������ �����Ѵ�.
				centerPos += Vector2::Normalize(&toTarget) * cameraSpeed * _TimeManager->DeltaTime();
				//�߽� ��ǥ�� ���� ī�޶� ��ǥ�� 0,0 left, top���� ����
				this->cameraPos = centerPos - Vector2(_WinSizeX / 2, _WinSizeY / 2);
				this->cameraRect = Figure::FloatRect(cameraPos, Vector2(_WinSizeX, _WinSizeY), Pivot::LeftTop);
			
				if (Vector2::Length(&(pTarget->GetPosition() - centerPos)) <= 50.f)
				{
					//ī�޶� ���¸� �⺻ ���·� ����
					this->_cameraState = CameraState::None;
					//�߽� ��ǥ�� ���� ī�޶� ��ǥ�� 0,0 left, top���� ����
					this->cameraPos = centerPos - Vector2(_WinSizeX / 2, _WinSizeY / 2);
					//ī�޶� ��Ʈ �������� �ٲ�����Ƿ� ī�޶� ��Ʈ ������
					this->cameraRect.Update(cameraPos, Vector2(_WinSizeX, _WinSizeY), Pivot::LeftTop);
				}
				//ī�޶� ����
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
ī�޶� ���� : ī�޶� ȭ�� ������ ������ ī�޶� �̵����� �ٽ� ������ ����  
***************************************************************************/
void Camera::ReviseCamera()
{
	//���� ī�޶� x��ǥ�� 0���� ������ (=ȭ�� ������ ������)
	if (cameraPos.x < 0.f) 
	{	
		//ī�޶� x��ǥ��ŭ �ٽ� �� ����ŭ ����
		cameraPos.x -= cameraPos.x;
		//�������� �����ǹǷ� ��Ʈ ������
		this->cameraRect.Update(cameraPos, Vector2(_WinSizeX, _WinSizeY), Pivot::LeftTop);
	}
	//���� ī�޶� ��Ʈ ���̰� �� ���� ���̺��� ũ�ٸ� 
	else if (cameraRect.right > (LONG)mapSize.x) 
	{
		//ī�޶� x��ǥ�� ī�޶� ��Ʈ ���� ���� - �� ���� �����Ѹ�ŭ �� ����
		cameraPos.x -= (cameraRect.right) - mapSize.x;
		//�������� �����ǹǷ� ��Ʈ ������
		this->cameraRect.Update(cameraPos, Vector2(_WinSizeX, _WinSizeY), Pivot::LeftTop);
	}

	//���� ī�޶� ��Ʈ ���̰� 0���� ������ (=ȭ�� ������ ������)
	if (cameraRect.top < 0) 
	{
		//ī�޶� y��ǥ��ŭ �ٽ� �� ����ŭ ����
		cameraPos.y -= cameraPos.y;
		//�������� �����ǹǷ� ��Ʈ ������
		this->cameraRect.Update(cameraPos, Vector2(_WinSizeX, _WinSizeY), Pivot::LeftTop);
	}
	//���� ī�޶� ��Ʈ ���� ���̰� �� ���� ���̺��� ũ�ٸ� 
	else if (cameraRect.bottom > (LONG)mapSize.y) 
	{
		//ī�޶� y��ǥ�� ī�޶� ��Ʈ ���� ���� - �� ���� �����Ѹ�ŭ �� ����
		cameraPos.y -= (cameraRect.bottom) - mapSize.y;
		//�������� �����ǹǷ� ��Ʈ ������
		this->cameraRect.Update(cameraPos, Vector2(_WinSizeX, _WinSizeY), Pivot::LeftTop);
	}
}

/***************************************************************************
## MoveMouseToFreeCamera ##
���� ���� ī�޶󿡼� ���콺 ��Ŭ���� ������ ���콺�� �����̸�
ī�޶� ȭ�� �̵� ���
***************************************************************************/
void Camera::MoveMouseToFreeCamera()
{
	//���� ���콺 ������ �ޱ�
	Vector2 currentMousePt = _Input->GetMousePosition();
	//���� ���콺 ��Ŭ���� �ϰ� ������
	if (_Input->GetKey(VK_RBUTTON))
	{
		//���� ����� ���콺 �����ǰ� ���� ���콺 �������� �ٸ���
		if (this->saveMousePt != currentMousePt)
		{
			//ī�޶� �����ǿ� ���콺 ������ ���� �� ����
			this->cameraPos += (saveMousePt - currentMousePt) / zoomValue;
			//ī�޶� ��ġ�� �ٲ�����Ƿ� ī�޶� ��Ʈ �ѹ� �� ����
			this->UpdateRenderRect();
			//����� ���콺 �����ǿ� ���� ���콺 ������ ����
			//���� �����ؼ� ������ �Ǹ� ������ �����ϰ� ���缭 ī�޶� ������ ���� �׸��ϱ�
			saveMousePt = currentMousePt;
		}
	}
	//����� ���콺 �����ǿ� ���� ���콺 ������ ����
	//���� �����ؼ� ������ �Ǹ� ������ �����ϰ� ���缭 ī�޶� ������ ���� �׸��ϱ�
	saveMousePt = currentMousePt;
}


/***************************************************************************
## CameraMouseWheelProc ##
ī�޶� ���콺 �� ����ϱ� ���� ���ν���
***************************************************************************/
void Camera::CameraMouseWheelProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (_cameraState == CameraState::FreeCamera) 
	{
		//���콺 �� �ø���
		if ((SHORT)HIWORD(wParam) > 0) 
		{
			this->zoomValue += 0.1f;
			this->zoomValue = Math::Clampf(zoomValue, CameraZoomMin, CameraZoomMax);
		}
		//���콺 �� ������
		else 
		{
			this->zoomValue -= 0.1f;
			this->zoomValue = Math::Clampf(zoomValue, CameraZoomMin, CameraZoomMax);
		}
	}
}


/***************************************************************************
## InitCameraToGameOjbect ##
//ī�޶� Ÿ�� ���� �� ī�޶� �� �� �ʱ�ȭ
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