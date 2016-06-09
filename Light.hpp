//
//  Light.hpp
//  PhotonMap
//
//  Created by Tyler Bertrand on 6/9/16.
//  Copyright © 2016 Tyler Bertrand. All rights reserved.
//

#ifndef Light_hpp
#define Light_hpp

#include <stdio.h>
#include <armadillo>

class Light
{
private:
  arma::vec3 position;
  arma::vec3 color;
  
  void read(std::istream &ins);
  
public:
  Light(std::istream &ins);
  ~Light();
  
  arma::vec3 get_position();
  arma::vec3 get_color();
  
  friend std::ostream& operator << (std::ostream &os, const Light &l);
};

#endif /* Light_hpp */
