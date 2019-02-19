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
  void print(const string &rwHwPbfl, const int &jahr, const map<pair<bool, double>, valarray<double> > &z) {
    for(auto const& [key, val] : z) {
  cout << rwHwPbfl << " " << jahr << " " << key.first  << " " << key.second;
      for_each(begin(val), end(val), [](const double& y) {cout << " " << y;});
      cout << endl;
    }
  }

  void add(const pair<bool, double> key, map<pair<bool, double>, valarray<double> > &vmap, const valarray<double> &infall) {
    auto search = vmap.find(key);
    if (search != vmap.end()) {
      search->second += infall;      
    } else {
      vmap[key] = infall;
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
  bool litter;
  double cLeaf;
  double cBranch;
  double cStem;
  double cCoarseRoot;
  double cFineRoot;
  double cSeed;
  double cStump;
  map<pair<bool, double>, valarray<double> > infall;
  cout << "#rwHwPbfl year litter size a w e n\n";
  while(cin >> rwHwPbfl >> jahr >> species >> BHD >> litter >> cLeaf >> cBranch >> cStem >> cCoarseRoot >> cFineRoot >> cSeed >> cStump) {
    if(jahrL != jahr || rwHwPbflL != rwHwPbfl) {
      if(jahrL > -99999) {
	print(rwHwPbflL, jahrL, infall);
      }
      infall.clear();
      jahrL = jahr;
      rwHwPbflL = rwHwPbfl;
    }
    valarray<double> chem = chemicalComposition[make_pair("leaf", species)];
    add(make_pair(litter, 0.), infall, (cLeaf + cSeed) * chem);
    chem = chemicalComposition[make_pair("branch", species)];
    add(make_pair(litter, 2.), infall,  cBranch * chem);
    chem = chemicalComposition[make_pair("stem", species)];
    if(BHD > 10.) {
      add(make_pair(litter, 20.), infall, (cStem + cCoarseRoot + cStump)*chem);
    } else {
      add(make_pair(litter, 2.), infall, (cStem + cCoarseRoot + cStump)*chem);
    }
    chem = chemicalComposition[make_pair("fineRoot", species)];
    add(make_pair(litter, 0.), infall, cFineRoot * chem);
  }
  print(rwHwPbflL, jahrL, infall);
  return(0);
}
