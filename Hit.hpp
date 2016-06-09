//
//  Hit.hpp
//  PhotonMap
//
//  Created by Tyler Bertrand on 5/27/16.
//  Copyright © 2016 Tyler Bertrand. All rights reserved.
//

#ifndef Hit_hpp
#define Hit_hpp

#include <stdio.h>
#include <armadillo>
#include <limits>

class Hit
{
private:
  double t = std::numeric_limits<double>::infinity();
  arma::vec3 pt;
  arma::vec3 normal;
  arma::vec3 color;
  arma::vec3 ambient;
  arma::vec3 reflectivity;
  double diffuse;
  double phong;
  
public:
  Hit();
  Hit(double t, arma::vec3 pt, arma::vec3 normal, arma::vec3 color, arma::vec3 ambient, double diffuse, double phong, arma::vec3 reflectivity);
  ~Hit();
  
  double get_t();
  arma::vec3 get_color();
  arma::vec3 get_ambient();
  double get_diffuse();
  double get_phong();
  arma::vec3 get_reflectivity();
  arma::vec3 get_pt();
  arma::vec3 get_normal();
  
  Hit& operator= (const Hit &h);
};

#endif /* Hit_hpp */
