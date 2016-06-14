//
//  Material.hpp
//  PhotonMap
//
//  Created by Tyler Bertrand on 6/13/16.
//  Copyright © 2016 Tyler Bertrand. All rights reserved.
//

#ifndef Material_hpp
#define Material_hpp

#include <stdio.h>
#include <iostream>
#include <armadillo>

class Material {
private:
  arma::vec3 color;
  arma::vec3 ambient;
  arma::vec3 reflectivity;
  double diffuse;
  double phong;
  
  void read(std::istream &ins);
  
public:
  Material(std::istream &ins);
  ~Material();
  
  arma::vec3 get_color();
  arma::vec3 get_ambient();
  arma::vec3 get_reflectivity();
  double get_diffuse();
  double get_phong();
  void print(std::ostream &os);
};

#endif /* Material_hpp */
