/*----------------------------------------------
Programmer: Alberto Bobadilla (labigm@gmail.com)
Date: 2016/05
----------------------------------------------*/
#ifndef __MyOctant_H_
#define __MyOctant_H_

#include "RE\ReEng.h"
#include "MyBOManager.h"


//System Class
class MyOctant
{
	vector3 m_v3Position;
	float m_fSize = 0.0f;
	MeshManagerSingleton* m_pMeshMngr = nullptr;
	MyBOManager* m_pBOMngr = nullptr;

public:
	/*
	Usage: Constructor
	Arguments: ---
	Output: class object
	*/
	MyOctant(void);

	/*
	Usage: Destructor
	Arguments: ---
	Output: ---
	*/
	~MyOctant(void);

	/*
	Usage: Will display the Octant
	Arguments: ---
	Output: ---
	*/
	void Display(void);

private:
	/*
	Usage: Deallocates member fields
	Arguments: ---
	Output: ---
	*/
	void Release(void);

	/*
	Usage: Allocates member fields
	Arguments: ---
	Output: ---
	*/
	void Init(void);
};


#endif //__MyOctant_H__