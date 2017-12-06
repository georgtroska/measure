int testScan1D() {
   HP33120A hp("/dev/ttyUSB0",9600);
   cout << hp.getIDN() << endl;
   K2000 k("/dev/ttyUSB1",9600);
   
   hp.setVoltageUnit(HP33120A::kVpp);
   hp.setType(HP33120A::kSinusoid);
   hp.setAmplitude(10);
   hp.setOffset(0);
   
   //cout << k.getIDN() << endl;
   k.setMode(K2000::kVoltageAC);

   Scan1D *scan = new Scan1D(&hp.freq);
   ScanSave *ssave = new ScanSave(scan);
   scan->addChannel(&k.input, 10, 0);
   //scan->drawOnline();
   
   //scan->RunLogDecades(10,100000,3,0, false);
   //ssave->write("test.root"); 
   
   return 0;
   
}
