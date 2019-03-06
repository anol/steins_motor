#include "MyDAQmxController.h"

int32 error = 0;
#define DAQmxErrChk(functionCall) if( DAQmxFailed(error=(functionCall)) ) throw error; 

MyDAQmxController::MyDAQmxController() {
	QueryPerformanceFrequency(&counterFrequency);
	printDebugInfo();
	DAQmxErrChk(DAQmxCreateTask("PulsTeller", &taskPulsTeller));
	DAQmxErrChk(DAQmxCreateCICountEdgesChan(taskPulsTeller, "Dev1/ctr0", "Ch1", DAQmx_Val_Rising, 0, DAQmx_Val_CountUp));
	DAQmxErrChk(DAQmxCreateTask("SpenningUt", &taskSpenningUt));
	DAQmxErrChk(DAQmxCreateAOVoltageChan(taskSpenningUt, "Dev1/ao0", "", 0.0, 5.0, DAQmx_Val_Volts, ""));
}

MyDAQmxController::~MyDAQmxController() {
	if (taskPulsTeller != 0) {
		DAQmxStopTask(taskPulsTeller);
		DAQmxClearTask(taskPulsTeller);
		taskPulsTeller = 0;
	}
	if (taskSpenningUt != 0) {
		DAQmxStopTask(taskSpenningUt);
		DAQmxClearTask(taskSpenningUt);
		taskSpenningUt = 0;
	}
}

int MyDAQmxController::Start() {
	uInt32      value;
	uInt32		oldValue = 0;
	float64     spenningUt[1] = { 0.0 };
	LARGE_INTEGER oldPerformanceCount = { 0 };
	DAQmxErrChk(DAQmxStartTask(taskPulsTeller));
	DAQmxErrChk(DAQmxStartTask(taskSpenningUt));
	spenningUt[0] = 0.0;
	for (int32 count = 0; count < 400; count++) {
		LARGE_INTEGER elapsedTime;
		LARGE_INTEGER performanceCount;
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
	return 0;
}

void MyDAQmxController::printDebugInfo() {
	char name[100];
	int32 data[100] = { 0 };
	printf("f=%d\n", counterFrequency.QuadPart);
	DAQmxErrChk(DAQmxGetDevCIPhysicalChans("Dev1", name, 100));
	printf("%s\n", name);
	DAQmxErrChk(DAQmxGetPhysicalChanCISupportedMeasTypes("Dev1/ctr0", &data[0], 10));
	for (int gpc = 0; gpc < 10 && data[gpc] > 0; gpc++) {
		printf("%d\n", data[gpc]);
	}
}
