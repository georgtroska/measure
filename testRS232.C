#include <unistd.h>
int testRS232() {
	
	dce::RS232Link *l = new dce::RS232Link("/dev/ttyUSB1",9600);
	l->setTerminationStringSend("\n");
		l->setTerminationStringRecv("\r");
	
	cout << l->query("SYSTEM:REMOTE",100) << endl;
	cout << l->query("*RST",100) << endl;
	cout << l->query("*CLS",100) << endl;
	usleep(100000);
	std::string answer = l->query("*IDN?",3000);
	cout << answer << endl;
	cout << answer.find(0x0d) << " " << answer.find(0x0a) << endl;

/*	
	for (int i = 10; i< 20; i++) {
		char out[255];
		sprintf(out, "APPL:SIN %d",i);
		cout << l->query(out,100) << endl;
		usleep(10000);
		cout << l->query("APPL?",200) << endl;
		
	}
*/
/*
l->query("DISP:TEXT \"GEORG\"",1500);
	usleep(100000);
	l->query("DISP:TEXT \"LIEBT\"",1500);
	usleep(100000);
	l->query("DISP:TEXT \"TANJA\"",1000);
	usleep(100000);
*/
	//cout << l->query("DISP?\n",500) << endl;
	
	l->close();

return 0;
}
