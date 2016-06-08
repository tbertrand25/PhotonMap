//
//  View.cpp
//  PhotonMap
//
//  Created by Tyler Bertrand on 5/26/16.
//  Copyright © 2016 Tyler Bertrand. All rights reserved.
//

#include <math.h>
#include "View.hpp"

View::View()
{
  
}

View::View(std::istream &ins)
{
  read(ins);
}

View::~View()
{
    
}

void View::read(std::istream &ins)
{
  // Tags for view parameters
  const std::string END_VIEW = "end_view";
  const std::string COI_TAG = "coi";
  const std::string EYE_TAG = "eye";
  const std::string VIEWPORT_WIDTH_TAG = "vwidth";
  const std::string ASPECT_TAG = "aspect";
  const std::string X_RES_TAG = "xres";
  const std::string UP_TAG = "up";
  const std::string FOCAL_TAG = "focal";
  
  // Tracks whether or not the required paramters have been seen
  bool seen_end = false;
  bool seen_coi = false;
  bool seen_eye = false;
  bool seen_viewport_width = false;
  bool seen_aspect = false;
  bool seen_x_res = false;
  bool seen_up = false;
  bool seen_focal = false;
  
  // Holds current chunk of istream
  std::string tok;
  
  while(!ins.eof() && !seen_end)
  {
    ins >> tok;
    
    if(tok == END_VIEW)
    {
      seen_end = true;
    }
    else if(tok == COI_TAG)
    {
      seen_coi = true;
      
      ins >> coi(0);
      ins >> coi(1);
      ins >> coi(2);
    }
    else if(tok == EYE_TAG)
    {
      seen_eye = true;
      
      ins >> eye(0);
      ins >> eye(1);
      ins >> eye(2);
    }
    else if(tok == VIEWPORT_WIDTH_TAG)
    {
      seen_viewport_width = true;
      ins >> viewport_width;
    }
    else if(tok == ASPECT_TAG)
    {
      seen_aspect = true;
      ins >> aspect_ratio;
    }
    else if(tok == X_RES_TAG)
    {
      seen_x_res = true;
      ins >> x_res;
    }
    else if(tok == UP_TAG)
    {
      seen_up = true;
      
      ins >> up(0);
      ins >> up(1);
      ins >> up(2);
    }
    else if(tok == FOCAL_TAG)
    {
      seen_focal = true;
      ins >> focal_length;
    }
  }
  
  // TODO: check that all required parameters have been seen
  
  viewport_height = viewport_width / aspect_ratio;
  y_res = round(x_res / aspect_ratio);
  
  w = arma::normalise(-(coi - eye));
  u = arma::cross(up, w);
  v = arma::cross(w, u);
}

std::ostream &operator << (std::ostream &os, const View &v )
{
  os << "coi: [" << v.coi(0) << ", " << v.coi(1) << ", " << v.coi(2) << "]\n";
  os << "eye: [" << v.eye(0) << ", " << v.eye(1) << ", " << v.eye(2) << "]\n";
  os << "width x height: " << v.viewport_width   << " x " << v.viewport_height << "\n";
  os << "aspect: "         << v.aspect_ratio << "\n";
  os << "xres x yres: "    << v.x_res << " x " << v.y_res << "\n";
  os << "focal: "          << v.focal_length << "\n";
  os << "up: [" << v.up(0) << ", " << v.up(1) << ", " << v.up(2) << "]\n";
  os << "u: [" << v.u(0) << ", " << v.u(1) << ", " << v.u(2) << "]\n";
  os << "v: [" << v.v(0) << ", " << v.v(1) << ", " << v.v(2) << "]\n";
  os << "w: [" << v.w(0) << ", " << v.w(1) << ", " << v.w(2) << "]\n";
  return os;
}