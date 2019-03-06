#include "PIDcontroller.h"

PIDcontroller::PIDcontroller() {
	integral = 0.0;
	old_error = 0.0;
	kP = 5.0;
	kI = 1.1;
	kD = 1.01;
	oMax = 5.0;
	oMin = 0.0;
}

PIDcontroller::~PIDcontroller() {
}

double PIDcontroller::input(double error, double dt) {
	integral += error * dt;
	double derived = (error - old_error) / dt;
	double output = kP * error + kI * integral + kD * derived;
	old_error = error;
	if (output > oMax) output = oMax;
	if (output < oMin) output = oMin;
	return output;
}
