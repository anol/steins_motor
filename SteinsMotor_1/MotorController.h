#pragma once
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <NIDAQmx.h>
#include <windows.h>

class MotorController
{
public:
	MotorController();
	~MotorController();
	int Start();
	TaskHandle	taskPulsTeller;
	TaskHandle	taskSpenningUt;

};

