#include <iostream>
#include <string>
#include <fstream>
#include <map>
#include <cmath>
#include <algorithm>
#include <sstream>

#include "../../bmExpansion.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

using namespace std;

int main(int argc, char** argv) {
  bmExp::bmexp bmexp;
  bmExp::bmExpReadCoef(bmexp, "../../coef.csv");
  bmExp::bmExpReadCoefLinkSpecies(bmexp, "../../coefLinkSpecies.csv");

  map<string, string> speciesTranslate;
  ifstream infile;
  string line;
  infile.open("coefSpeciesTranslate.csv", ios::in);
  while(getline(infile, line)) {
    if(line.size() > 0 && line[0] != '#') { //Skip comment lines
      istringstream linestream(line);
      string item;
      size_t n=0;
      string species;
      while(getline(linestream, item, ',')) {
	if(n==1) {species = item;
	} else if(n==2) {speciesTranslate[item] = species;}
	++n;
      }
    }
  }
  infile.close();

  if(argc > 1) {
    cout << "#rwHwPbfl jahr fraktion tot JAb species Nrepjeha BHD bmLeaf bmBranch bmStem bmCorseRoot bmFineRoot bmStump bmSeed" << endl;

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
	if(KA < 0.) {KA = .5 * HT;}
	string species = "PiAb";
	auto speciesSearch = speciesTranslate.find(BaumArt);
	if (speciesSearch != speciesTranslate.end()) {
	  species = speciesSearch->second;
	} else {
	  cerr << "Species: " << BaumArt << " Not found!\n";
	}
	double rDens = bmexp.raumdichte[species];
	double bmLeaf = bmexp.g(bmExp::compartment::leaf, species, BHD, HT, KA);
	double bmBranch = bmexp.g(bmExp::compartment::branch, species, BHD, HT, KA);
	double bmStem = rDens * BaumVol;
	if(bmStem <= 0.) {
	  bmStem = rDens * pow((BHD+min(1.3,HT))/200., 2) * M_PI * HT * 0.5;
	}
	if(bmLeaf <= 0. && bmStem > 0.) {
	  bmLeaf = 0.005 * pow((BHD+min(1.3,HT)), 2);
	}
	if(bmBranch <= 0. && bmStem > 0.) {
	  bmBranch = rDens/50000. * pow((BHD+min(1.3,HT)), 2);
	}
	double bmCorseRoot = bmexp.stem2corseroot[species] * bmStem;
	double bmFineRoot = bmexp.leaf2fineroot[species] * bmLeaf;
	double bmSeed = bmexp.leaf2seed[species] * bmLeaf;
	double bmStump = rDens * pow((BHD+min(1.3,HT))/200., 2) * M_PI * min(0.3, 0.5 * HT);
 	cout << rwHwPbfl
	     << " " << jahr
	     << " " << fraktion
	     << " " << tot
	     << " " << JAb
	     << " " << species
	     << " " << Nrepjeha
	     << " " << BHD
	     << " " << bmLeaf
	     << " " << bmBranch
	     << " " << bmStem
	     << " " << bmCorseRoot
	     << " " << bmFineRoot
	     << " " << bmStump
	     << " " << bmSeed
	     << "\n";
     }
    }
    infile.close();
  }
  
  return(0);
}
