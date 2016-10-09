int testHP33120A() {
	HP33120A hp("/dev/ttyUSB2",9600);
	cout << hp.getIDN() << endl;
	hp.setVoltageUnit(HP33120A::kVpp);
	hp.setType(HP33120A::kSinusoid);
	hp.setFrequency(42000);
	hp.setAmplitude(0.42);
	hp.setOffset(-0.42);
return 0;
}
