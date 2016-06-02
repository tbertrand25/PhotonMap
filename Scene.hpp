//
//  Scene.hpp
//  PhotonMap
//
//  Created by Tyler Bertrand on 5/26/16.
//  Copyright © 2016 Tyler Bertrand. All rights reserved.
//

#ifndef Scene_hpp
#define Scene_hpp

#include <stdio.h>
#include <armadillo>
#include <vector>

#include "View.hpp"
#include "Surface.hpp"

class Scene
{
private:
  std::vector<Surface*> surfaces;
  View viewer;
  
public:
  Scene(View v, std::vector<Surface*> s);
  ~Scene();
  
  std::vector<Surface*> get_surfaces();
  View get_view();
  
  friend std::ostream& operator << ( std::ostream& os, const Scene &s);
};

#endif /* Scene_hpp */
