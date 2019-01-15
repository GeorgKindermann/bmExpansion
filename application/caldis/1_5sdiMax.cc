#include <iostream>
#include <string>
#include <sstream>
#include <cmath>
#include <vector>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

using namespace std;

namespace {
  struct dat {
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
  };
}

int main(int argc, char** argv) {
  const double sdiMax = argc > 1 ? atof(argv[1]) : numeric_limits<double>::infinity();
  string rwHwPbflL = "";
  int jahrL = -99999;
  double gHa=0.;
  double nHa=0.;
  string line;
  vector<dat> data;
  for(;;) {
    getline(cin, line);
    if(line.size() > 0 && line[0] == '#') { //comment lines
      cout << line << endl;
      continue;
    }
    if(line.size() > 0 || !cin.good()) {
      dat d;
      istringstream linestream(line);
      linestream >> d.rwHwPbfl >> d.jahr >> d.BaumNr >> d.fraktion >> d.BaumArt >> d.BHD >> d.HT >> d.KA >> d.Nrepjeha >> d.tot >> d.BaumVol >> d.JAb;
      if(jahrL != d.jahr || rwHwPbflL != d.rwHwPbfl || !cin.good()) {
	double dg = 0.;
	if(nHa > 0.) {dg = sqrt(gHa/nHa/M_PI)*200.;}
	double sdi = 0.;
	if(dg > 0.) {sdi = nHa * pow(25./dg, -1.605);}
	double nMul = sdi>sdiMax ? sdiMax / sdi : 1.;
	for(auto & x: data) {
	  cout << x.rwHwPbfl << " " << x.jahr << " " << x.BaumNr << " " << x.fraktion << " " << x.BaumArt << " " << x.BHD << " " << x.HT << " " << x.KA << " " << x.Nrepjeha * nMul << " " << x.tot << " " << x.BaumVol << " " << x.JAb << "\n";
	}
	if(!cin.good()) {break;}
	gHa=0.;
	nHa=0.;
	jahrL = d.jahr;
	rwHwPbflL = d.rwHwPbfl;
	data.clear();
      }
      if(d.fraktion == "Verb" && !d.tot) {
	gHa += pow(d.BHD/200, 2) * M_PI * d.Nrepjeha;
	nHa += d.Nrepjeha;
      }
      data.push_back(d);
    }
  }
  return(0);
}
