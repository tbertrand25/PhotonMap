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

#include "Material.hpp"

class Hit
{
private:
  double t = std::numeric_limits<double>::infinity();
  arma::vec2 tex_coords;
  arma::vec3 pt;
  arma::vec3 normal;
  std::shared_ptr<Material> mat;
  
public:
  Hit();
  Hit(double t, arma::vec3 pt, arma::vec3 normal, std::shared_ptr<Material> mat);
  Hit(double t, arma::vec3 pt, arma::vec3 normal, arma::vec2 tex_coords,
      std::shared_ptr<Material> mat);
  ~Hit();
  
  double get_t();
  arma::vec3 get_pt();
  arma::vec3 get_normal();
  std::shared_ptr<Material> get_material();
  arma::vec2 get_tex_coords();
  
  Hit& operator= (const Hit &h);
};

#endif /* Hit_hpp */
