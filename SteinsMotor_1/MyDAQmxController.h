#pragma once
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <NIDAQmx.h>
#include <windows.h>
#include "PIDcontroller.h"

class MyDAQmxController
{
public:
	MyDAQmxController();
	~MyDAQmxController();
	int Start(double target);
	double getElapsedMilliseconds();
	void printDebugInfo();
	TaskHandle	taskPulsTeller;
	TaskHandle	taskSpenningUt;
	LARGE_INTEGER counterFrequency;
	LARGE_INTEGER oldPerformanceCount;
	PIDcontroller pid;
};

