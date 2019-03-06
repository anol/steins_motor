#include <iostream>
#include "SteinsMotor_1.h"
#include "MotorController.h"

int main()
{
	MotorController motorController;
    std::cout << "Steins Motor\n"; 
	motorController.Start();
}
