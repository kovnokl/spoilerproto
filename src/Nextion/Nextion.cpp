#include "Nextion.h"

String nextionDataRaw;


void setupNextion()
{
	DISPLAYSERIAL.begin(115200);
}


void handleNextionData()
{
	while(DISPLAYSERIAL.available())
	{
		nextionDataRaw += (char)DISPLAYSERIAL.read();
		delay(3);
	}	
	while(nextionDataRaw.length())
	{
        //Serial.println(nextionDataRaw);
		uint32_t index = nextionDataRaw.indexOf(0xff);
		if(index==-1) index = nextionDataRaw.length()-1;
		String nextionData = nextionDataRaw.substring(0, index);
		nextionDataRaw.remove(0, index + 1);
        if(nextionData.substring(0, 3).equals("ref"))
            mots.startRef();
        else if(nextionData.substring(0, 6).equals("spminp"))
            mots.minspeed += 0.01f;
        else if(nextionData.substring(0, 6).equals("spminm"))
            mots.minspeed -= 0.01f;
        else if(nextionData.substring(0, 6).equals("spmaxp"))
            mots.maxspeed += 0.01f;
        else if(nextionData.substring(0, 6).equals("spmaxm"))
            mots.maxspeed -= 0.01f;
        else if(nextionData.substring(0, 3).equals("pos")){
            Spoilerpos *pos;
            if(nextionData.charAt(3) == '1')
                pos = &pos1;
            else if(nextionData.charAt(3) == '2')
                pos = &pos2;
            
            if(nextionData.charAt(4) == 'g')
            {
                mots.goPos(pos);
                break;
            }
            float *value;
            if(nextionData.charAt(4) == 'v'){
                value = &pos->posv;
            }
            else if(nextionData.charAt(4) == 'h'){
                value = &pos->posh;
            }
            else {
                break;
            }
            uint8_t changeval = atoi(nextionData.substring(6, index).c_str());
            if (nextionData.charAt(5) == 'm')
                *value -= changeval;
            else if(nextionData.charAt(5) == 'p')
                *value += changeval;
        }
	}
}
