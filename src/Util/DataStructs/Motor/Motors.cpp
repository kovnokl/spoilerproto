#include "Motors.h"

Motors::Motors(HardwareSerial *vSerial, HardwareSerial *hSerial, int vrefPin, int hrefPin){
	vSerial->begin(115200, SERIAL_8N1, 9, 10);
	hSerial->begin(115200, SERIAL_8N1, 13, 14);
	pinMode(vrefPin, INPUT);
	pinMode(hrefPin, INPUT);
	this->vrefPin = vrefPin;
	this->hrefPin = hrefPin;
	vUART.setSerialPort(vSerial);
	hUART.setSerialPort(hSerial);
	prevMillis = millis();
	prevMicros = micros();
}

void Motors::updateTime(){
	deltaMillis = millis() - prevMillis;
	prevMillis = millis();
	deltaMicros = micros() - prevMicros;
	prevMicros = micros();
}

void Motors::updatePos(){
	vUART.getVescValues();
	hUART.getVescValues();
    currPos.posv += float(vUART.data.rpm) / float(60000000.0f / float(deltaMicros));
    currPos.posh += float(hUART.data.rpm) / float(60000000.0f / float(deltaMicros));
}

void Motors::refLoop(MotorTypes mt){
	if(refTime < 1500){
		refTime += deltaMillis;
		vUART.setDuty(minspeed);
		hUART.setDuty(minspeed);
		return;
	}
	VescUart *tempUART;
	int *tempRefPin;
	MotorStates *usedStates;
	if (mt == FRONT)
	{
		tempUART = &vUART;
		tempRefPin = &vrefPin;
		usedStates = &vmotorState;
		tempUART->setDuty(minspeed * -1);
	}
	else if (mt == BACK){
		tempUART = &hUART;
		tempRefPin = &hrefPin;
		usedStates = &hmotorState;
		tempUART->setDuty(minspeed * -1);
	}
	if(digitalRead(*tempRefPin) == HIGH)
	{
		Serial.println(mt == BACK ? "BACK" : "FRONT");
		tempUART->setDuty(0);
		currPos = Spoilerpos();
		*usedStates = REFFED;
	}
}

void Motors::posLoop(MotorTypes mt){
	VescUart *tempUART;
	MotorStates *usedStates;
	float *goalMotorPos;
	float *currMotorPos;
	float *startMotorPos;
	if (mt == FRONT){
		tempUART = &vUART;
		usedStates = &vmotorState;
		goalMotorPos = &goalPos->posv;
		currMotorPos = &currPos.posv;
		startMotorPos = &startPos.posv;
	}else if(mt == BACK){
		tempUART = &hUART;
		usedStates = &hmotorState;
		goalMotorPos = &goalPos->posh;
		currMotorPos = &currPos.posh;
		startMotorPos = &startPos.posh;
	}
	
	if(*usedStates == MOVINGFW){
		if(*currMotorPos < *goalMotorPos)
		{
			if (*currMotorPos - *startMotorPos < CLAMPVAL)
			{
				float value = minspeed + (float(*currMotorPos - *startMotorPos) * maxspeed / CLAMPVAL) * (1.0f - minspeed);
				tempUART->setDuty(value);
			}else if (*goalMotorPos - *currMotorPos <= CLAMPVAL)
			{
				float value = minspeed + (float(*goalMotorPos - *currMotorPos) * maxspeed / CLAMPVAL) * (1.0f - minspeed);
				tempUART->setDuty(value);
			} else {
				tempUART->setDuty(maxspeed);
			}

		}else{
			tempUART->setDuty(0);
			*usedStates = INPOSITION;
		}
	}else if(*usedStates == MOVINGBW){
		if(*currMotorPos > *goalMotorPos)
		{
			if (*startMotorPos - *currMotorPos < CLAMPVAL)
			{
				float value = minspeed - (float(*currMotorPos - *startMotorPos) * maxspeed / CLAMPVAL) * (1.0f - minspeed);
				tempUART->setDuty(-value);
			}else if (*currMotorPos - *goalMotorPos < CLAMPVAL)
			{
				float value = minspeed - (float(*goalMotorPos - *currMotorPos) * maxspeed / CLAMPVAL) * (1.0f - minspeed);
				tempUART->setDuty(-value);
			} else {
				tempUART->setDuty(-maxspeed);
			}
		}else{
			tempUART->setDuty(0);
			*usedStates = INPOSITION;
		}
	}
}

void Motors::motorLoop(){
	updateTime();
	updatePos();
	switch (vmotorState) {
		case UNKNOWN:
			break;
		case REFFING:
			refLoop(FRONT);
			break;
		case REFFED:
			break;
		case INPOSITION:
			break;
		case MOVINGFW:
			posLoop(FRONT);
			break;
		case MOVINGBW:
			posLoop(FRONT);
			break;
	}

	switch (hmotorState) {
		case UNKNOWN:
			break;
		case REFFING:
			refLoop(BACK);
			break;
		case REFFED:
			break;
		case INPOSITION:
			break;
		case MOVINGFW:
			posLoop(BACK);
			break;
		case MOVINGBW:
			posLoop(BACK);
			break;
	}
}

void Motors::goPos(Spoilerpos *pos){
	if(vmotorState == MOVINGBW || vmotorState == MOVINGFW || vmotorState == REFFING
		|| hmotorState == MOVINGBW || hmotorState == MOVINGFW || hmotorState == REFFING)
		return ;
	goalPos = pos;
	startPos = Spoilerpos(currPos.posv, currPos.posh);
	if(goalPos->posv > currPos.posv)
		vmotorState = MOVINGFW;
	else if(goalPos->posv < currPos.posv)
		vmotorState = MOVINGBW;
	if(goalPos->posh > currPos.posh)
		hmotorState = MOVINGFW;
	else if(goalPos->posh < currPos.posh)
		hmotorState = MOVINGBW;
}

void Motors::startRef(){
	if(vmotorState == MOVINGBW || vmotorState == MOVINGFW || vmotorState == REFFING
		|| hmotorState == MOVINGBW || hmotorState == MOVINGFW || hmotorState == REFFING)
		return ;
	refTime = 0;
	vmotorState = REFFING;
	hmotorState = REFFING;
}