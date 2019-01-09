#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

using namespace std;

int main(int argc, char** argv) {
  if(argc > 1) {
    cout << "rwHwPbfl jahr gHa vHa" << endl;

    ifstream infile;
    string line;
    string rwHwPbflL = "";
    int jahrL = -99999;
    double gHa=0.;
    double vHa=0.;
    infile.open(argv[1], ios::in);
    while(getline(infile, line)) {
      if(line.size() > 0 && line[0] != '#') { //Skip comment lines
	istringstream linestream(line);
	string rwHwPbfl;
	int jahr;
	string BaumNr;
	string fraktion;
	string BaumArt;
	double BHD;
	double HT;
	double KA;
	double Nrepjeha;
	int tot;
	double BaumVol;
	int JAb;
	linestream >> rwHwPbfl >> jahr >> BaumNr >> fraktion >> BaumArt >> BHD >> HT >> KA >> Nrepjeha >> tot >> BaumVol >> JAb;
	if(BaumVol < 0.) {BaumVol = 0.;}
	if(fraktion == "Verb" && !tot) {
	  gHa += pow(BHD/200, 2) * M_PI * Nrepjeha;
	  vHa += BaumVol * Nrepjeha;
	}
	if(jahrL != jahr || rwHwPbflL != rwHwPbfl) {
	  if(jahrL > -99999) {cout << rwHwPbflL << " " << jahrL << " " << round(gHa*10.)/10. << " " << round(vHa) << "\n";}
	  gHa=0.;
	  vHa=0.;
	  jahrL = jahr;
	  rwHwPbflL = rwHwPbfl;
	}
      }
    }
    cout << rwHwPbflL << " " << jahrL << " " << round(gHa*10.)/10. << " " << round(vHa) << "\n";
  }
  return(0);
}
