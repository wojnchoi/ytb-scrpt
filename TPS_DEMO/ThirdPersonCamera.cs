using UnityEngine;

public class ThirdPersonCamera : MonoBehaviour
{
	public float sensitivity = 1; //감도

	public Transform target; //카메라가 따라갈 플레이어

	float xMove; //좌우
	float yMove; //상하
	public Vector2 yMinMax = new Vector2(-10, 30); //최소 최대 상하 움직임

	public float offsetZ = 2;
	public float offsetX = 2;
	public float offsetY = 2;

	public float smoothTime = .01f;
	Vector3 rotationVel;
	Vector3 curRot;

	void Start()
	{
		Cursor.lockState = CursorLockMode.Locked;
		Cursor.visible = false;
	}
    void LateUpdate()
	{

		xMove += Input.GetAxis("Mouse X") * sensitivity;
		yMove += -(Input.GetAxis("Mouse Y") * sensitivity);
		yMove = Mathf.Clamp(yMove, yMinMax.x, yMinMax.y);
		//마우스 입력값에 맞게 카메라 회전
		curRot = Vector3.SmoothDamp(curRot, new Vector3(yMove, xMove), ref rotationVel, smoothTime);
		transform.eulerAngles = curRot;
		//포지션 설정
		transform.position = target.position - transform.forward * offsetZ + transform.right * offsetX + transform.up * offsetY;

	}

}