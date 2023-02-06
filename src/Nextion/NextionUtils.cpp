#include "NextionUtils.h"

void sendNextionCommand(String cmd)
{
	DISPLAYSERIAL.print(cmd);
	DISPLAYSERIAL.write(0xff);
	DISPLAYSERIAL.write(0xff);
	DISPLAYSERIAL.write(0xff);
}

void updateVals(){
    sendNextionCommand("page0.pos1v.val=" + String(int(pos1.posv)));
    sendNextionCommand("page0.pos1h.val=" + String(int(pos1.posh)));
    sendNextionCommand("page0.pos2v.val=" + String(int(pos2.posv)));
    sendNextionCommand("page0.pos2h.val=" + String(int(pos2.posh)));
    sendNextionCommand("page0.posv.val=" + String(int(mots.currPos.posv)));
    sendNextionCommand("page0.posh.val=" + String(int(mots.currPos.posh)));
    sendNextionCommand("page0.spmax.val=" + String(int(mots.maxspeed*100)));
    sendNextionCommand("page0.spmin.val=" + String(int(mots.minspeed*100)));
    sendNextionCommand("page0.sp.val=" + String(int(mots.vUART.data.rpm)));
}

