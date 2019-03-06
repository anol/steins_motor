#pragma once
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <NIDAQmx.h>
#include <windows.h>

class MyDAQmxController
{
public:
	MyDAQmxController();
	~MyDAQmxController();
	int Start();
	void printDebugInfo();
	TaskHandle	taskPulsTeller;
	TaskHandle	taskSpenningUt;
	LARGE_INTEGER counterFrequency;

};

