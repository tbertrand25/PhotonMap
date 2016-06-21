//
//  Ray.hpp
//  PhotonMap
//
//  Created by Tyler Bertrand on 5/27/16.
//  Copyright © 2016 Tyler Bertrand. All rights reserved.
//

#ifndef Ray_hpp
#define Ray_hpp

#include <stdio.h>
#include <armadillo>

class Ray
{
private:
  arma::vec3 origin;
  arma::vec3 direction;
  
public:
  Ray();
  Ray(arma::vec3 origin, arma::vec3 direction);
  ~Ray();
  
  arma::vec3 get_origin();
  arma::vec3 get_direction();
  
  Ray& operator= (const Ray &r);
};

#endif /* Ray_hpp */
