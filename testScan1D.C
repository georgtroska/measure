int testScan1D() {
	HP33120A hp("/dev/ttyUSB2",9600);
	cout << hp.getIDN() << endl;
	K2000 k("/dev/ttyUSB1",9600);
	
	hp.setVoltageUnit(HP33120A::kVpp);
	hp.setType(HP33120A::kSinusoid);
	hp.setAmplitude(0.050);
	hp.setOffset(0);
	
	cout << k.getIDN() << endl;
	k.setMode(K2000::kVoltageAC);

	Scan1D *scan = new Scan1D(&hp.freq);
	scan->addChannel(&k.input, 5, 0.1);
	scan->drawOnline();
	
	scan->Run(1000,20000,1000,1);
	
	return 0;
	
}
