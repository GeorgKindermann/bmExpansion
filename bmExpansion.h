#ifndef BMEXPANSION_H_GUARD
#define BMEXPANSION_H_GUARD

#include <string>
#include <cmath>
#include <vector>
#include <map>
#include <utility>
#include <limits>
#include <fstream>
#include <sstream>

namespace bmExp {

  enum class compartment {leaf=0, branch=1, stem=2, coarseRoot=3, fineRoot=4};
  
  class bmexp {
  public:
    bmexp();
    double g(const compartment &compart, const std::string &species
	     , const double &d, const double &h, const double &hKa);
    void i(const compartment &compart, const std::string &coefName
	   , const size_t &funNr, const std::vector<double> &coef
	   , const double &dmin, const double &dmax
	   , const double &hmin, const double &hmax
	   , const double &dmul, const double &hmul, const double &resMul);
    void i(const std::string &species
	   , const std::vector<std::string> &coefNames);
    std::map<std::string, double> raumdichte;
  private:
    struct bm {
      double dmin;
      double dmax;
      double hmin;
      double hmax;
      double dmul;
      double hmul;
      double resMul;  //Ergebniss wird damit multipliziert
      std::vector<double> coef;
      double (*fun)(const std::vector<double> &coef, const double& d, const double& h, const double& hKa);
    };
    std::multimap<std::pair<compartment, std::string>, bm> bmf;
    std::vector<double (*)(const std::vector<double> &coef, const double& d, const double& h, const double& hKa)> functions;
    std::map<std::string, std::vector<std::string> > coefNameFromSpecies;
  };

  void bmExpReadCoef(bmexp &, const std::string& filename);
  void bmExpReadCoefLinkSpecies(bmexp &, const std::string& filename);

}

#endif
