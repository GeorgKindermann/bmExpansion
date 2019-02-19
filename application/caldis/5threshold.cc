#include <iostream>
#include <string>
#include <fstream>
#include <array>
#include <vector>
#include <utility>
#include <map>
#include <deque>
#include <algorithm>
#include <tuple>
#include <cmath>
#include <cstdlib>
#include <limits>

using namespace std;

namespace {
  void print(const string &rwHwPbfl, const int &year, map<double, array<double, 4> > &yassoIn, const double &infallGroundVeg) {
    if(infallGroundVeg > 0.) {
      auto search = yassoIn.find(0.);
      if (search != yassoIn.end()) {
	search->second[0] += infallGroundVeg;
      } else {
	array<double, 4> infallGV {infallGroundVeg,0.,0.,0.};
	yassoIn[0] = infallGV;
      }
    }
    for(auto & [size, infall] : yassoIn) {
      double csum = 0.;
      for(int i=0; i<4; ++i) {
	infall[i] = round(infall[i]*10000.)/10000.;
	csum += infall[i];
      }
      if(csum > 0.) {
	cout << rwHwPbfl
	     << " " << size
	     << " " << year;
	for(int i=0; i<4; ++i) {cout << " " << infall[i];}
	cout << endl;
      }
    }
  }
}

int main(int argc, char** argv) {
  const double thresholdLitter = argc > 1 ? atof(argv[1]) : numeric_limits<double>::infinity();
  const int timeSpanSmooth = argc > 2 ? atoi(argv[2]) : 0;
  const int yearLow = argc > 3 ? atoi(argv[3]) : std::numeric_limits<double>::max();
  const int yearHigh = argc > 4 ? atoi(argv[4]) : std::numeric_limits<double>::min();
  int yearHighNow = yearHigh;
  int yearLowNow = yearLow;
  cout << "#rwHwPbfl size year a w e n\n";
  string line;
  getline(cin,line);
  string rwHwPbflL = "";
  string rwHwPbfl;
  int jahr;
  bool litter;
  array<double, 5> dline;
  vector<tuple<int, bool, array<double, 5> > > data;
  map<int, double> sum;
  for(;;) {
    cin >> rwHwPbfl >> jahr >> litter;
    for(int i=0; i<5; ++i) {cin >> dline[i];}
    if(rwHwPbflL != rwHwPbfl || !cin.good()) {
      if(data.size() > 0) {
	map<int, double> infallGroundVeg;
	for(int i=yearLowNow; i<=yearHighNow; ++i) {
	  sum[i] += 0.;
	  infallGroundVeg[i] = 0.;
	}
	if(timeSpanSmooth > 0) {
	  deque<double> q15;
	  auto iter = sum.begin();
	  for(int i=0; i<timeSpanSmooth; ++i) {
	    if(iter != sum.end()) {q15.push_back(min(thresholdLitter,iter->second)); ++iter;}
	  }
	  iter = sum.begin();
	  for(int i=0; i<timeSpanSmooth; ++i) {
	    if(iter != sum.end()) {q15.push_back(min(thresholdLitter,iter->second)); ++iter;}
	  }
	  double smoothInfall = sum.begin()->second;
	  double lastInfall = sum.begin()->second;
	  for(auto const& [year, infall] : sum) {
	    double maxInfall = *max_element(q15.begin(), q15.end());
	    smoothInfall += min(0., infall - lastInfall); //Removal
	    smoothInfall += (maxInfall - smoothInfall) / 7.;
	    if(smoothInfall < 0.) {smoothInfall = 0.;}
	    if(smoothInfall > maxInfall) {smoothInfall = maxInfall;}
	    if(smoothInfall > infall) {
	      infallGroundVeg[year] = smoothInfall - infall;
	    } else {infallGroundVeg[year] = 0.;}
	    if(iter != sum.end()) {
	      q15.pop_front();
	      q15.push_back(min(thresholdLitter, iter->second));
	      ++iter;
	    }
	    lastInfall = infall;
	  }
	}
	map<double, array<double, 4> > yassoIn;
	int currentYear = yearLowNow;
	for(auto const& [year, litter, sizeInfall] : data) {
	  array<double, 4> infall;
	  for(;currentYear<year && currentYear <= yearHighNow; ++currentYear) {
	    print(rwHwPbflL, currentYear, yassoIn,infallGroundVeg[currentYear]);
	    yassoIn.clear();
	  }
	  double size = sizeInfall[0];
	  for(int i=0; i<4; ++i) {infall[i] = sizeInfall[i+1];}
	  if(litter && sum[year] > thresholdLitter) {
	    for(int i=0; i<4; ++i) {infall[i] *= thresholdLitter / sum[year];}
	  }
	  auto search = yassoIn.find(size);
	  if (search != yassoIn.end()) {
	    for(int i=0; i<4; ++i) {search->second[i] += infall[i];}
	  } else {
	    yassoIn[size] = infall;
	  }
	}
	for(;currentYear <= yearHighNow; ++currentYear) {
	  print(rwHwPbflL, currentYear, yassoIn,infallGroundVeg[currentYear]);
	  yassoIn.clear();
	}
	data.clear();
      }
      sum.clear();
      yearHighNow = yearHigh;
      yearLowNow = yearLow;
      rwHwPbflL = rwHwPbfl;
    }
    if(!cin.good()) {break;}
    data.push_back(make_tuple(jahr, litter, dline));
    if(litter) {
      for(int i=1; i<5; ++i) {sum[jahr] += dline[i];}
    }
    if(yearHighNow < jahr) {yearHighNow = jahr;}
    if(yearLowNow > jahr) {yearLowNow = jahr;}
  }
  return(0);
}
