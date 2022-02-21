using System.Collections;
using System.Collections.Generic;
using UnityEngine;

[RequireComponent(typeof(Rigidbody))]
public class PlayerMovement : MonoBehaviour
{
	public float moveSpeed = 5; //캐릭터 속도

	public float turnSmoothTime = 0.2f;
	float turnSmoothVelocity;
	public float speedSmoothTime = 0.1f;
	float speedSmoothVelocity;
	public float currentSpeed;
	
	Rigidbody _rigidbody;
	Transform cam;

	public Transform muzzle;
	Vector3 MouseWorldPosition;
	public Transform bullet;

	public float MuzzelFps;
	void Start()
	{
		cam = Camera.main.transform;
		_rigidbody = GetComponent<Rigidbody>();
	}

	void Update()
	{
		//움직임
		Vector3 forward = transform.TransformDirection(Vector3.forward);
		Vector3 right = transform.TransformDirection(Vector3.right);
		float curSpeedX =  Input.GetAxis("Vertical") ;
		float curSpeedY = Input.GetAxis("Horizontal") ;
		Vector3 moveDirection = (forward * curSpeedX) + (right * curSpeedY);
		float targetSpeed = moveSpeed * moveDirection.magnitude;
		currentSpeed = Mathf.SmoothDamp(currentSpeed, targetSpeed, ref speedSmoothVelocity, speedSmoothTime);
		_rigidbody.MovePosition(_rigidbody.position + moveDirection.normalized * Time.fixedDeltaTime);


		//회전
		float targetRotation = cam.eulerAngles.y;
		transform.eulerAngles = Vector3.up * Mathf.SmoothDampAngle(transform.eulerAngles.y, targetRotation, ref turnSmoothVelocity, turnSmoothTime);
		
		//에임
		Vector2 screenCenterPoint = new Vector2(Screen.width / 2f, Screen.height / 2f);
		Ray ray = Camera.main.ScreenPointToRay(screenCenterPoint);
		if (Physics.Raycast(ray, out RaycastHit raycastHit))
		{
			Vector3 point = ray.GetPoint(777f);
			//Debug.DrawLine(ray.origin, point, Color.red);
			MouseWorldPosition = raycastHit.point;
			//슈팅
			if (Input.GetMouseButtonDown(0))
			{
				Vector3 aimDir = (MouseWorldPosition - muzzle.position).normalized;
				Instantiate(bullet, muzzle.position, Quaternion.LookRotation(aimDir, Vector3.up));
			}
		}

		
			
	}

}


