#include <iostream>
#include <string>
#include <fstream>
#include <map>
#include <sstream>

namespace {
  struct compartments {
    double leaf;
    double branch;
    double stem;
    double coarseRoot;
    double fineRoot;
    double seed;
    double stump;
  };
}

using namespace std;

int main() {
  map<string, compartments> trunoverRates;
  map<string, compartments> bm2c;
  ifstream infile;
  string line;
  infile.open("coefTrunoverRates.csv", ios::in);
  while(getline(infile, line)) {
    if(line.size() > 0 && line[0] != '#') { //Skip comment lines
      istringstream linestream(line);
      string item;
      size_t n=0;
      string species;
      compartments tmp;
      while(getline(linestream, item, ',')) {
	switch(n) {
	case 0 : species = item; break;
	case 1 : tmp.leaf = std::atof(item.c_str()); break;
	case 2 : tmp.branch = std::atof(item.c_str()); break;
	case 3 : tmp.stem = std::atof(item.c_str()); break;
	case 4 : tmp.coarseRoot = std::atof(item.c_str()); break;
	case 5 : tmp.fineRoot = std::atof(item.c_str()); break;
	case 6 : tmp.seed = std::atof(item.c_str()); break;
	case 7 : tmp.stump = 0;
	}
	++n;
      }
      trunoverRates[species] = tmp;
    }
  }
  infile.close();
  infile.open("coefBm2C.csv", ios::in);
  while(getline(infile, line)) {
    if(line.size() > 0 && line[0] != '#') { //Skip comment lines
      istringstream linestream(line);
      string item;
      size_t n=0;
      string species;
      compartments tmp;
      while(getline(linestream, item, ',')) {
	switch(n) {
	case 0 : species = item; break;
	case 1 : tmp.leaf = std::atof(item.c_str()); break;
	case 2 : tmp.branch = std::atof(item.c_str()); break;
	case 3 : tmp.stem = std::atof(item.c_str());
	  tmp.stump = tmp.stem;
	  break;
	case 4 : tmp.coarseRoot = std::atof(item.c_str()); break;
	case 5 : tmp.fineRoot = std::atof(item.c_str()); break;
	case 6 : tmp.seed = std::atof(item.c_str());
	}
	++n;
      }
      bm2c[species] = tmp;
    }
  }

  getline(cin,line);
  string rwHwPbfl;
  int jahr;
  string fraktion;
  int tot;
  int JAb;
  string species;
  double Nrepjeha;
  double BHD;
  double bmLeaf;
  double bmBranch;
  double bmStem;
  double bmCoarseRoot;
  double bmFineRoot;
  double bmStump;
  double bmSeed;
  cout << "#rwHwPbfl jahr species BHD litter cLeaf cBranch cStem cCoarseRoot cFineRoot cSeed cStump\n";
  while(cin >>  rwHwPbfl >> jahr >> fraktion >> tot >> JAb >> species >> Nrepjeha >> BHD >> bmLeaf >> bmBranch >> bmStem >> bmCoarseRoot >> bmFineRoot >> bmStump >> bmSeed) {
    compartments infall {0.,0.,0.,0.,0.,0.,0.};
    bool litter = false;
    if(fraktion == "Verb") {
      if(!tot) {
	litter = true;
	infall.leaf = trunoverRates[species].leaf;
	infall.branch = trunoverRates[species].branch;
	infall.stem = trunoverRates[species].stem;
	infall.coarseRoot = trunoverRates[species].coarseRoot;
	infall.fineRoot = trunoverRates[species].fineRoot;
	infall.seed = trunoverRates[species].seed;
	infall.stump = trunoverRates[species].stump;
      } else if(JAb == jahr) {
	infall.leaf = 1.;
	infall.branch = 1.;
	infall.coarseRoot = 1.;
	infall.fineRoot = 1.;
	infall.seed = 1.;
      }
    } else if(fraktion == "NatAbgang") {
      infall.stem = 1.;
      infall.stump = 1.;
      if(!tot) {
	infall.leaf = 1.;
	infall.branch = 1.;
	infall.coarseRoot = 1.;
	infall.fineRoot = 1.;
	infall.seed = 1.;
      }
    } else if(fraktion == "Zufallsnutzung" || fraktion == "VN" || fraktion == "EN") {
      infall.stem = .15;
      infall.stump = 1.;
      if(!tot || JAb == jahr) {
	infall.leaf = 1.;
	infall.branch = 1.;
	infall.coarseRoot = 1.;
	infall.fineRoot = 1.;
	infall.seed = 1.;
      }
    }
    if(infall.leaf>0. || infall.branch>0. || infall.stem>0. || infall.coarseRoot>0. || infall.fineRoot>0. || infall.seed>0. || infall.stump>0.) {
      cout << rwHwPbfl << " " << jahr << " " << species << " " << BHD
	   << " " << litter
	   << " " << Nrepjeha / 1000. * bmLeaf * bm2c[species].leaf * infall.leaf
	   << " " << Nrepjeha / 1000. * bmBranch * bm2c[species].branch * infall.branch
	   << " " << Nrepjeha / 1000. * bmStem * bm2c[species].stem * infall.stem
	   << " " << Nrepjeha / 1000. * bmCoarseRoot * bm2c[species].coarseRoot * infall.coarseRoot
	   << " " << Nrepjeha / 1000. * bmFineRoot * bm2c[species].fineRoot * infall.fineRoot
	   << " " << Nrepjeha / 1000. * bmSeed * bm2c[species].seed * infall.seed
	   << " " << Nrepjeha / 1000. * bmStump * bm2c[species].stump * infall.stump
	   << "\n";
    }
  }
    
  return(0);
}
