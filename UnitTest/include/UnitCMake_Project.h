#pragma once
#include "xunittestbase.h"

class UnitCMake_Project :
	public XTestObject
{
	DECLARE_UNIT_TEST();
public:
	UnitCMake_Project(void);
	~UnitCMake_Project(void);
	// �����ļ�ϵͳ
	void Test();
};