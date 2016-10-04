#include <unistd.h>
int testRS232K2000() {
	
	dce::RS232Link *l = new dce::RS232Link("/dev/ttyUSB0",9600);

	cout << l->query("*IDN?",1000) << endl;
	
	l->close();

return 0;
}
