#pragma once
class PIDcontroller {
public:
	PIDcontroller();
	~PIDcontroller();
	double input(double error, double dt);
	double integral;
	double old_error;
	double kP;
	double kI;
	double kD;
	double oMax;
	double oMin;
};

