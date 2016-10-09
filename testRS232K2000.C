#include <unistd.h>
int testRS232K2000() {
	/*
	dce::RS232Link *l = new dce::RS232Link("/dev/ttyUSB0",9600);
	cout << l->query("*IDN?",1000) << endl;
	* l->close();
	*/
	
	K2000 k("/dev/ttyUSB1",9600);
	cout << k.getIDN() << endl;
	//cout << k.query("CONF:VOLT:DC\n",1000) << endl;
	//cout << k.query("INIT\n",1000) << endl;
	
	//cout << k.query("CONF?\n",1000) << endl;
	k.setMode(K2000::kVoltageAC);
	//cout << k.query("MEAS:VOLT:AC?\n",1000) << endl;
	

return 0;
}
