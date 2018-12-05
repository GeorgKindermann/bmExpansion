#include <iostream>
#include <string>
#include <fstream>
#include <map>
#include <sstream>
#include <utility>
#include <valarray>
#include <algorithm>
#include <cstdio>

using namespace std;

namespace {
  void print(string &rwHwPbfl, int &jahr, const string &sz, valarray<double> x) {
    x = x.apply([](double y)->double {return(round(y*10000.)/10000.);});
    if(x.sum() > 0.) {cout << rwHwPbfl << " " << jahr << " " << sz;
      for_each(begin(x), end(x), [](const double& y) {cout << " " << y;});
      cout << "\n";
    }
  }
}

int main() {
  map<pair<string, string>, valarray<double> > chemicalComposition;
  ifstream infile;
  string line;
  infile.open("coefChemicalComposition.csv", ios::in);
  while(getline(infile, line)) {
    if(line.size() > 0 && line[0] != '#') { //Skip comment lines
      istringstream linestream(line);
      string item;
      size_t n=0;
      string species;
      string litterType;
      valarray<double> tmp(4);
      while(getline(linestream, item, ',') && n<6) {
	switch(n) {
	case 0 : litterType = item; break;
	case 1 : species = item; break;
	default : tmp[n-2] = std::atof(item.c_str());
	}
	++n;
      }
      if(tmp.sum() > 0.) {
	tmp /= tmp.sum();
	chemicalComposition[make_pair(litterType, species)] = tmp;
      }
    }
  }
  infile.close();

  getline(cin,line);
  string rwHwPbflL = "";
  int jahrL = -99999;
  string rwHwPbfl;
  int jahr;
  string species;
  double BHD;
  double cLeaf;
  double cBranch;
  double cStem;
  double cCoarseRoot;
  double cFineRoot;
  double cSeed;
  double cStump;
  valarray<double> infall0(4);
  valarray<double> infall2(4);
  valarray<double> infall20(4);
  cout << "#rwHwPbfl year size a w e n\n";
  while(cin >> rwHwPbfl >> jahr >> species >> BHD >> cLeaf >> cBranch >> cStem >> cCoarseRoot >> cFineRoot >> cSeed >> cStump) {
    if(jahrL != jahr || rwHwPbflL != rwHwPbfl) {
      print(rwHwPbflL, jahrL, "0", infall0);
      print(rwHwPbflL, jahrL, "2", infall2);
      print(rwHwPbflL, jahrL, "20", infall20);
      infall0 = 0.;
      infall2 = 0.;
      infall20 = 0.;
      jahrL = jahr;
      rwHwPbflL = rwHwPbfl;
    }
    valarray<double> chem = chemicalComposition[make_pair("leaf", species)];
    infall0 += (cLeaf + cSeed) * chem;
    chem = chemicalComposition[make_pair("branch", species)];
    infall2 += cBranch * chem;
    chem = chemicalComposition[make_pair("stem", species)];
    if(BHD > 10.) {
      infall20 += (cStem + cCoarseRoot + cStump) * chem;
    } else {
      infall2 += (cStem + cCoarseRoot + cStump) * chem;
    }
    chem = chemicalComposition[make_pair("fineRoot", species)];
    infall0 += cFineRoot * chem;
  }
  print(rwHwPbflL, jahrL, "0", infall0);
  print(rwHwPbflL, jahrL, "2", infall2);
  print(rwHwPbflL, jahrL, "20", infall20);
  
  return(0);
}
