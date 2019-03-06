#include "MotorController.h"



#define DAQmxErrChk(functionCall) if( DAQmxFailed(error=(functionCall)) ) goto Error; else


MotorController::MotorController()
{
}


MotorController::~MotorController()
{
	if (taskPulsTeller != 0)
	{
		DAQmxStopTask(taskPulsTeller);
		DAQmxClearTask(taskPulsTeller);
		taskPulsTeller = 0;
	}
	if (taskSpenningUt != 0)
	{
		DAQmxStopTask(taskSpenningUt);
		DAQmxClearTask(taskSpenningUt);
		taskSpenningUt = 0;
	}
}


int MotorController::Start()
{
	int32       error = 0;
	int32      data[1000] = { 0 };
	uInt32      value;
	uInt32		oldValue = 0;
	char        errBuff[2048] = { '\0' };
	char		name[100];
	int32       sampsPerChanRead = 0;
	int32       count = 1;
	float64     spenningUt[1] = { 0.0 };

	LARGE_INTEGER performanceCount;
	LARGE_INTEGER oldPerformanceCount = { 0 };
	LARGE_INTEGER counterFrequency;
	LARGE_INTEGER elapsedTime;

	DAQmxErrChk(DAQmxCreateTask("PulsTeller", &taskPulsTeller));
	DAQmxErrChk(DAQmxCreateCICountEdgesChan(taskPulsTeller, "Dev1/ctr0", "Ch1", DAQmx_Val_Rising, 0, DAQmx_Val_CountUp));
	DAQmxErrChk(DAQmxCreateTask("SpenningUt", &taskSpenningUt));
	DAQmxErrChk(DAQmxCreateAOVoltageChan(taskSpenningUt, "Dev1/ao0", "", 0.0, 5.0, DAQmx_Val_Volts, ""));

	/*********************************************/
	// DAQmx Start Code
	/*********************************************/
	DAQmxErrChk(DAQmxStartTask(taskPulsTeller));
	DAQmxErrChk(DAQmxStartTask(taskSpenningUt));

	DAQmxErrChk(DAQmxGetDevCIPhysicalChans("Dev1", name, 100));
	printf("%s\n", name);

	DAQmxErrChk(DAQmxGetPhysicalChanCISupportedMeasTypes("Dev1/ctr0", &data[0], 10));
	for (int gpc = 0; gpc < 10 && data[gpc] > 0; gpc++) {
		printf("%d\n", data[gpc]);
	}

	QueryPerformanceFrequency(&counterFrequency);
	printf("f=%d\n", counterFrequency.QuadPart);

	spenningUt[0] = 0.0;
	/*********************************************
	/
	/*********************************************/
	for (count = 0; count < 400; count++)
	{
		spenningUt[0] += 0.01;
		DAQmxErrChk(DAQmxReadCounterScalarU32(taskPulsTeller, 10.0, &value, NULL));
		QueryPerformanceCounter(&performanceCount);
		elapsedTime.QuadPart = (performanceCount.QuadPart - oldPerformanceCount.QuadPart);
		printf("%d - %1.2F - %d - %d \n",
			count,
			spenningUt[0],
			value - oldValue,
			(1000 * elapsedTime.QuadPart) / counterFrequency.QuadPart);
		fflush(stdout);
		oldValue = value;
		oldPerformanceCount = performanceCount;
		DAQmxErrChk(DAQmxWriteAnalogF64(taskSpenningUt, 1, 1, 10.0, DAQmx_Val_GroupByChannel, spenningUt, NULL, NULL));
		Sleep(199);
	}

	spenningUt[0] = 0.0;
	DAQmxErrChk(DAQmxWriteAnalogF64(taskSpenningUt, 1, 1, 10.0, DAQmx_Val_GroupByChannel, spenningUt, NULL, NULL));

Error:
	if (DAQmxFailed(error))
		DAQmxGetExtendedErrorInfo(errBuff, 2048);
	if (taskPulsTeller != 0) {
		/*********************************************/
		// DAQmx Stop Code
		/*********************************************/
		DAQmxStopTask(taskPulsTeller);
		DAQmxClearTask(taskPulsTeller);
	}
	if (DAQmxFailed(error))
		printf("DAQmx Error: %s\n", errBuff);
	printf("End of program, press Enter key to quit\n");
	getchar();
	return 0;
}
