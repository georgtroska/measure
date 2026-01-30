#include "ScanSave.h"

#include <TMap.h>
#include <TList.h>


using namespace std;

ScanSave::ScanSave(Scan1D *scan) : _scan(*scan){
	_dataTree = new TTree("Data", "Data");
	//_conditionsTree = new TTree("Conditions", "Conditions");
	//_dataTree->AddFriend("Conditions");
	//_conditionsTree->AddFriend("Data");
	_scan.Connect("onData(MeasEvent*)", "ScanSave", this, "receiveData(MeasEvent*)");
	_scan.Connect("onStartRun(MeasEvent*)", "ScanSave", this, "setBranches(MeasEvent*)");
}

void ScanSave::setSaveFileName(string name) {
	cout << "Generating Filename" << endl;
	if (name.compare("") == 0 || name.compare("noname") == 0) {
		cout << "Note:\tYou have entered no name." << endl;
		cout << "\tFile will be saved as \"noname_(date)_(irrad_info).root\" " << endl;
	} 
	
	//erase ".root" if typed in name
	size_t dot = name.rfind(".root");
	if (dot != string::npos) {
		name.erase(dot);
	}
	
	//add date and irrad-info to name
	char date[12];
	TDatime *datime = new TDatime();
	sprintf(date,"%i-%i-%i",(int)datime->GetYear(), (int)datime->GetMonth(), (int)datime->GetDay());
	name += "_";
	name += date;
	/*
	if (_cond.pIrrad == 0 && _cond.nIrrad == 0) {
		name += "_noirrad";
	} 
	else if (_cond.pIrrad == 0 && _cond.nIrrad != 0) {
		char irradInfo [255];
		sprintf(irradInfo,"_N%.1E",_cond.nIrrad);
		name += irradInfo;
	} 
	else if (_cond.pIrrad != 0 && _cond.nIrrad == 0) {
		char irradInfo [255];
		sprintf(irradInfo,"_P%.1E",_cond.pIrrad);
		name += irradInfo;
	} 
	else {
		char irradInfo [255];
		sprintf(irradInfo,"_N%.1E_P%.1E", _cond.nIrrad, _cond.pIrrad);
		name += irradInfo;
	}
	*/
	
	
	//add first not used index and append ".root"
	int index=1;
	bool file_exists =true;
	char name_dotroot[255];
	
	while (file_exists==true)
	{
		sprintf(name_dotroot, "%s_%i.root",name.c_str(),index);
		TFile *testfile = new TFile(name_dotroot,"OPEN");
		if (testfile->IsZombie()) 
		{
			//This file does not exist
			file_exists= false;
		}
		else
		{
			index++;
		}
		delete testfile;
		
	}
	
	cout <<"FILENAME IS: " << name_dotroot << endl;
	_saveFile = new TFile(name_dotroot, "RECREATE");
	_dataTree->SetDirectory(_saveFile);
	//_conditionsTree->SetDirectory(_saveFile);
}
/*
void ScanSave::setUserInfo(ScanConditions* cond) {
	TMap* map = new TMap();
	map->Add(new TObjString("Name"), new TObjString(cond->name.c_str()));
	map->Add(new TObjString("Operator"), new TObjString(cond->op.c_str()));
	char out[255];
	sprintf(out, "%.4f", cond->pIrrad);
	map->Add(new TObjString("P-Irrad"), new TObjString(out));
	sprintf(out, "%.4f", cond->nIrrad);
	map->Add(new TObjString("N-Irrad"), new TObjString(out));
	map->Add(new TObjString("Kind"), new TObjString(cond->kind.c_str()));
	map->Add(new TObjString("Comment"), new TObjString(cond->comment.c_str()));
	map->Add(new TObjString("UUID"), new TObjString(cond->uuid.c_str()));
	
	TList *list = _dataTree->GetUserInfo();
	list->Add(map);
}

void ScanSave::setConditionsTree(ScanConditions* cond) {
	_conditionsTree->Branch("Name", &(cond->name));
	_conditionsTree->Branch("Operator", &(cond->op));
	_conditionsTree->Branch("Kind", &(cond->kind));
	_conditionsTree->Branch("Comment", &(cond->comment));
	_conditionsTree->Branch("PIrrad", &(cond->pIrrad));
	_conditionsTree->Branch("NIrrad", &(cond->nIrrad));
	_conditionsTree->Branch("UUID", &(cond->uuid));
	string scanType = "Scan1D";
	_conditionsTree->Branch("ScanType", &scanType);
	vector<string> channelNames;
	const int nchannel = _scan.getNChannels();
	const vector<Channel*> channels = _scan.getChannels();
	for ( int n = 0; n<nchannel; n++){
		channelNames.push_back( channels.at(n)->getSaveName() + "_" +  channels.at(n)->getDevice().getName()  );
	} 
	_conditionsTree->Branch("Channels", &channelNames);
	vector<string> params;
	params.push_back(_scan.getParam()->getSaveName() + "_" + _scan.getParam()->getDevice().getName() );
	_conditionsTree->Branch("Params", &params);
	_conditionsTree->Fill();
}
* */

void ScanSave::write(string name) {
	//std::cout << "Being in write.." << std::endl;
	setSaveFileName(name);
	_saveFile->Write();
}
/*
void ScanSave::setConditions(ScanConditions* cond) {
	_cond = *cond;
	setConditionsTree(cond);
	setUserInfo(cond);
}
*/

void ScanSave::setBranches(MeasEvent * bore) {			//*RECEIVER*
	std::cout << "Being in setBranches..." << std::endl; 		
	_activeMeasEvent = *bore;
	const int nchannel = _scan.getNChannels();
	std::cout << "nchannel: " << nchannel << std::endl;
	_dataTree->Branch(_scan.getParam()->getSaveName().c_str(),&_activeMeasEvent.param);
	const vector<Channel*> channels = _scan.getChannels();
	for ( int n = 0; n<nchannel; n++){
		string channelName = channels.at(n)->getSaveName();
		_dataTree->Branch(channelName.c_str(),&(_activeMeasEvent.channelEvent.at(n).value));
		_dataTree->Branch((channelName+"_RMS").c_str(),&(_activeMeasEvent.channelEvent.at(n).RMS));
		_dataTree->Branch("Time",&_activeMeasEvent.time);
		_dataTree->Branch((channelName+"_single").c_str(),&(_activeMeasEvent.channelEvent.at(n).singleValue));
		
	}
	
}

void ScanSave::receiveData (MeasEvent* event) {						//*RECEIVER*
	std::cout << "Receiving Data..." << std::endl;
	if (!event->isBORE) { 
		_activeMeasEvent = *event;
		std::cout << "param: " << _activeMeasEvent.param << std::endl;
		std::cout << "value: " << _activeMeasEvent.channelEvent.at(0).value << std::endl;
		std::cout << "Filling Tree..." << std::endl;
		_dataTree->Fill();
	} else { std::cout << "Event is BORE!" << std::endl; }
}

ScanSave::~ScanSave() {
	delete _dataTree;
	//delete _conditionsTree;
	delete _saveFile;
}
