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
  bool is_refractive = false;
  bool is_phong = false;
  bool is_reflective = false;
  bool is_textured = false;
  
  arma::vec3 color;
  arma::vec3 ambient;
  arma::vec3 reflectivity;
  double diffuse;
  double phong;
  
  std::string texture;
  std::vector<std::vector<arma::vec3>> tex_colors;
  
  void read(std::istream &ins);
  
public:
  Material(std::istream &ins);
  ~Material();
  
  arma::vec3 get_color();
  arma::vec3 get_ambient();
  arma::vec3 get_reflectivity();
  double get_diffuse();
  double get_phong();
  
  unsigned long get_tex_x_size();
  unsigned long get_tex_y_size();
  arma::vec3 get_tex_color(int x, int y);
  //std::vector<std::vector<arma::vec3>> get_tex_colors();
  
  bool get_is_reflective();
  bool get_is_phong();
  bool get_is_textured();
  void print(std::ostream &os);
};

#endif /* Material_hpp */
