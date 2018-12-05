#include "bmExpansion.h"

//uncomment to disable assert()
//#define NDEBUG
#include <cassert>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace {

  //Funktionstyp Pollanschuetz
  double ffPollanschuetz(const std::vector<double> &c, const double& d, const double& h, const double& hKa __attribute__((unused))) {
    double ret=std::numeric_limits<double>::quiet_NaN();
    if(c.size()==7) {
      if(d>0. && h>0.) {
	ret = c[0] + c[1]*std::pow(log(d),2) + c[2]/h + c[3]/d + c[4]/(d*d) +
	  c[5]/(d*h) + c[6]/(d*d*h);
	if(ret < 0.) {ret = 0.;
	} else {
	  if(ret > 2.) {ret = 2.;}
	  ret *= pow(d/10./2.,2) * M_PI * h/10.;
	}
      } else {ret = 0.;}
    }
    return(ret);
  }

  //Funktionstyp log
  double ffLog(const std::vector<double> &c, const double& d, const double& h, const double& hKa) {
    double ret=std::numeric_limits<double>::quiet_NaN();
    if(c.size()==6) {
      double cr = 0.5;
      if(h>0.) {cr = (h - hKa) / h;}
      ret = exp(c[0] + c[1] * log(d) + c[2] * log(h) + c[3] * log(cr)
		+ c[4] * h/d + c[5] * (1.-cr));
      if(!std::isfinite(ret)) {ret = 0.;}
    }
    return(ret);
  }

  //Allometric
  double ffAllometric(const std::vector<double> &c, const double& d, const double& h, const double& hKa __attribute__((unused))) {
    double ret=std::numeric_limits<double>::quiet_NaN();
    if(c.size()==3) {
      if(d>0. && h>0.) {
	ret = c[0] * pow(d, c[1]) * pow(h, c[2]);
      } else {ret = 0.;}
    }
    return(ret);
  }

  //Albrektson
  double ffAlbrektson(const std::vector<double> &c, const double& d, const double& h, const double& hKa) {
    double ret=std::numeric_limits<double>::quiet_NaN();
    if(c.size()==3) {
      if(d>0.) {
	ret = exp(c[0] * pow(1.+d, c[1]) * pow(h-hKa, c[2]));
      } else {ret = 0.;}
    }
    return(ret);
  }

}

namespace bmExp {

  bmexp::bmexp() {
    functions.push_back(&ffPollanschuetz); //0
    functions.push_back(&ffLog); //1
    functions.push_back(&ffAllometric); //2
    functions.push_back(&ffAlbrektson); //3
  }

  double bmexp::g(const compartment &compart, const std::string &species
		  , const double &d, const double &h, const double &hKa) {
    double ret=std::numeric_limits<double>::quiet_NaN();
    auto coefName = coefNameFromSpecies[species][size_t(compart)];
    auto iiter = bmf.equal_range(std::make_pair(compart, coefName));
    //go to all suitable functions
    unsigned int n=0;
    double sumRes=0.;
    for(auto iter=iiter.first; iter!=iiter.second; ++iter) {
      bm &k = iter->second;
      //Range is OK
      if(k.dmin <= d && k.dmax >= d && k.hmin <= h && k.hmax >= h) {
	sumRes += k.resMul * k.fun(k.coef, d*k.dmul, h*k.hmul, hKa*k.hmul);
	++n;
      }
    }
    if(n>0) {ret = sumRes/static_cast<double>(n);}
    return(ret);
  };

  void bmexp::i(const compartment &compart, const std::string &coefName
		, const size_t &funNr, const std::vector<double> &coef
		, const double &dmin, const double &dmax
		, const double &hmin, const double &hmax
		, const double &dmul, const double &hmul
		, const double &resMul) {
    bm tmp;
    assert(funNr < functions.size());
    tmp.coef = coef;
    tmp.dmin = dmin; tmp.dmax = dmax; tmp.hmin = hmin; tmp.hmax = hmax;
    tmp.dmul = dmul; tmp.hmul = hmul; tmp.resMul = resMul;
    tmp.fun = functions[funNr];
    bmf.insert(std::make_pair(std::make_pair(compart, coefName), tmp));
  }

  void bmexp::i(const std::string &species
		, const std::vector<std::string> &coefNames) {
    assert(coefNames.size() == 5);
    coefNameFromSpecies[species] = coefNames;
  }

  void bmExpReadCoefLinkSpecies(bmexp &bm, const std::string& filename) {
    std::ifstream fin(filename.c_str());
    std::string line;
    while(getline(fin, line)) {
      if(line.size() > 0 && line[0] != '#') { //Skip comment lines
	//Split line into tokens
	std::istringstream linestream(line);
	std::string item;
	std::string species;
	std::vector<std::string> coefNames;
	size_t n=0;
        while(getline(linestream, item, ',')) {
	  if(item.size() > 0 && item[0] == '#') {break;} //Skip comments
	  if(n==0) {species = item;
	  } else if(n==6) {bm.raumdichte[species] = std::atof(item.c_str());
	  } else if(n==7) {bm.stem2corseroot[species] = std::atof(item.c_str());
	  } else if(n==8) {bm.leaf2fineroot[species] = std::atof(item.c_str());
	  } else if(n==9) {bm.leaf2seed[species] = std::atof(item.c_str());
	  } else {coefNames.push_back(item);}
	  ++n;
	}
	if(coefNames.size()==5) {
	  bm.i(species, coefNames);
	}
      }
    }
  }
  
  void bmExpReadCoef(bmexp &bm, const std::string& filename) {
    std::ifstream fin(filename.c_str());
    std::string line;
    while(getline(fin, line)) {
      if(line.size() > 0 && line[0] != '#') { //Skip comment lines
	//Split line into tokens
	std::istringstream linestream(line);
	std::string item;
	compartment comp;
	std::string coefName;
	size_t funNr;
	std::vector<double> ra; //Range deffinition values
	std::vector<double> mul; //Multiplication values
	std::vector<double> coef; //Coeficients
	unsigned int n=0;
        while(getline(linestream, item, ',')) {
	  if(item.size() > 0 && item[0] == '#') {break;} //Skip comments
	  if(n<3) {
	    assert(item.size() > 0);
	    switch (n) {
	    case 0: comp = compartment(std::atoi(item.c_str())); break;
	    case 1: coefName = item; break;
	    case 2: funNr = std::atoi(item.c_str());
	    }
	  } else {
	    double value = std::numeric_limits<double>::quiet_NaN();
	    if(item.size()>0) {value = std::atof(item.c_str());}
	    if(n<7) {ra.push_back(value);}
	    else if(n<10) {mul.push_back(value);}
	    else {coef.push_back(value);}
	  }
	  ++n;
	}
	//Set undef values to default
	if(!std::isfinite(ra[0])) {ra[0]=0.;}  //dmin
	if(!std::isfinite(ra[1])) {ra[1]=std::numeric_limits<double>::infinity();} //dmax
	if(!std::isfinite(ra[2])) {ra[2]=0.;} //hmin
	if(!std::isfinite(ra[3])) {ra[3]=std::numeric_limits<double>::infinity();} //hmax
	if(!std::isfinite(mul[0])) {mul[0]=1.;}  //dmul
	if(!std::isfinite(mul[1])) {mul[1]=1.;}  //hmul
	if(!std::isfinite(mul[2])) {mul[2]=1.;}  //resmul
	for(unsigned int i=0; i<coef.size(); ++i) {
	  if(!std::isfinite(coef[i])) {coef[i] = 0.;}
	}
	bm.i(comp, coefName, funNr, coef, ra[0], ra[1], ra[2], ra[3]
	     , mul[0], mul[1], mul[2]);
      }
    }
    fin.close();
  }
  
}
