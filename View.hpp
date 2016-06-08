//
//  View.hpp
//  PhotonMap
//
//  Created by Tyler Bertrand on 5/26/16.
//  Copyright © 2016 Tyler Bertrand. All rights reserved.
//

#ifndef View_hpp
#define View_hpp

#include <stdio.h>
#include <armadillo>

class View {
private:
  // Location of the viewing eye
  arma::vec3 eye;
  
  // Orthonormal basis for the viewing eye
  arma::vec3 u, v, w;
  
  // Center of interest - point where the viewing eye is looking
  arma::vec3 coi;
  
  // Indicates the "up" direction in the scene
  arma::vec3 up;
  
  // Aspect ratio of the viewport
  double aspect_ratio;
  
  // Width and height of the viewport in world units
  double viewport_width;
  double viewport_height;
  
  // Width and height of the viewport in pixels
  int x_res;
  int y_res;
  
  double focal_length;
  
public:
  View();
  View(std::istream &ins);
  ~View();
  
  void read(std::istream &ins);
  
  double get_x_res();
  double get_y_res();
  double get_view_width();
  double get_view_height();
  double get_focal();
  arma::vec3 get_u();
  arma::vec3 get_v();
  arma::vec3 get_w();
  arma::vec3 get_eye();
  
  friend std::ostream& operator << (std::ostream& os, const View &v);
};

#endif /* View_hpp */
