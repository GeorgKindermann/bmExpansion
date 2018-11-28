#include <iostream>
#include <string>

#include "bmExpansion.h"

using namespace std;

int main() {
  bmExp::bmexp bmexp;
  bmExp::bmExpReadCoef(bmexp, "coef.csv");
  bmExp::bmExpReadCoefLinkSpecies(bmexp, "coefLinkSpecies.csv");

  string species = "PiAb";
  double dbh = 50.;
  double h = 30.;
  double hKa = 14.;

  cout << "Species: " << species << endl;
  cout << "DBH: " << dbh << " cm" << endl;
  cout << "Height: " << h << " m" << endl;
  cout << "Height Crownbase: " << hKa << " m" << endl;
  cout << "Leaf biomass: " << bmexp.g(bmExp::compartment::leaf, species, dbh, h, hKa) << " kg" << endl;
  cout << "Branch biomass: " << bmexp.g(bmExp::compartment::branch, species, dbh, h, hKa) << " kg" << endl;
  cout << "Stem biomass: " << bmexp.g(bmExp::compartment::stem, species, dbh, h, hKa)*bmexp.raumdichte[species] << " kg" << endl;
  cout << "Corse root biomass: " << 0.2 * bmexp.g(bmExp::compartment::stem, species, dbh, h, hKa)*bmexp.raumdichte[species] << " kg" << endl; //Corseroot=0.2Stem
  cout << "Fine root biomass: " << 1. * bmexp.g(bmExp::compartment::leaf, species, dbh, h, hKa) << " kg" << endl; //Fineroot == Leave
  
  return(0);
}
