using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Projectile : MonoBehaviour
{
    public float speed = 100f; //탄속
    Rigidbody _rigidbody;

    private void Start()
    {
        _rigidbody = GetComponent<Rigidbody>();
    }

    private void Update()
    {
        _rigidbody.velocity = transform.forward * speed;
    }
    
    //부딫힘
    private void OnTriggerEnter(Collider collision)
    {
        if (collision.gameObject.tag == "Obstacle")
        {
            if (collision.GetComponent<Shatter>() != null)
            {
                collision.GetComponent<Shatter>().DestroyMesh();

                Debug.Log("Shatter");
            }
            Destroy(gameObject);
        }


    }

}
