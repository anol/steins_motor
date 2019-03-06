#include <iostream>
#include "SteinsMotor_1.h"
#include "MyDAQmxController.h"

int main()
{
	std::cout << "Steins Motor\n";
	try {
		MyDAQmxController motorController;
		motorController.Start(10.0);
	}
	catch (int32 error) {
		char errBuff[2048] = { '\0' };
		if (DAQmxFailed(error))
			DAQmxGetExtendedErrorInfo(errBuff, 2048);
		if (DAQmxFailed(error))
			printf("DAQmx Error: %s\n", errBuff);
	}
	std::cout << "End of program, press Enter key to quit\n";
	getchar();
}
