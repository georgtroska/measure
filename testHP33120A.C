R__LOAD_LIBRARY(build/Release/measure)
#include "src/HP33120A.h"
int testHP33120A() {
   HP33120A hp("COM6",9600);
   cout << hp.getIDN() << endl;
   //hp.setVoltageUnit(HP33120A::kVpp);
   //hp.setType(HP33120A::kSinusoid);
   /*
   hp.setFrequency(42000);
   hp.setAmplitude(0.42);
   hp.setOffset(-0.42);
   */
   
   hp.freq(380);
   hp.ampl(0.1);
   hp.offset(0);
   
   hp.freq(400);
   hp.freq(500);
   
return 0;
}
