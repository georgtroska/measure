R__LOAD_LIBRARY(build/Release/measure)
#include "src/K2000.h"
#include "src/HP33120A.h"
#include "Scan1D.h"
#include "ScanSave.h"
int testScan1D() {
   HP33120A hp("COM6",9600);
   cout << hp.getIDN() << endl;
   K2000 k("COM4",9600);
   cout << k.getIDN() << endl;
   hp.setVoltageUnit(HP33120A::kVpp);
   hp.setType(HP33120A::kSinusoid);
   hp.setAmplitude(0.1);
   hp.setOffset(0);
   
   
   k.setMode(K2000::kVoltageAC);
   
   Scan1D *scan = new Scan1D(&hp.freq);
   ScanSave *ssave = new ScanSave(scan);
   scan->addChannel(&k.voltage, 5, 0);
   scan->addChannel(&k.current, 5, 0);
   scan->drawOnline();
   
   scan->RunLogDecades(10,10000,12,0, false);
   hp.setAmplitude(0);
   ssave->write("test.root"); 
   
   return 0;
   
}
