#ifndef MOTOR_H
#define MOTOR_H

#include <Arduino.h>
#include <VescUart.h>
#include "Util/DataStructs/Spoilerpos/Spoilerpos.h"

#define CLAMPVAL 200.0f

typedef enum {
	UNKNOWN,
	REFFING,
	REFFED,
	INPOSITION,
	MOVINGFW,
	MOVINGBW
} MotorStates;

typedef enum {
	FRONT,
	BACK
} MotorTypes;


class Motors{
	private:
		int vrefPin;
		int hrefPin;
		Spoilerpos startPos;
		Spoilerpos *goalPos;
		MotorStates vmotorState = UNKNOWN;
		MotorStates hmotorState = UNKNOWN;
		uint32_t deltaMillis;
		uint32_t prevMillis;
		uint32_t refTime;
		uint64_t deltaMicros;
		uint64_t prevMicros;
		void posLoop(MotorTypes mt);
		void refLoop(MotorTypes mt);
		void updatePos();
		void updateTime();
	public:
		VescUart vUART;
		VescUart hUART;
		float minspeed = 0.3f;
		float maxspeed = 0.9f;
		Spoilerpos currPos;
		Motors(HardwareSerial *vSerial, HardwareSerial *hSerial, int vrefPin, int hrefPin);
		void motorLoop();
		void goPos(Spoilerpos *pos);
		void startRef();
};

#endif