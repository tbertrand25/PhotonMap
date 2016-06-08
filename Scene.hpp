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
#include <memory>
#include <armadillo>
#include <vector>

#include "View.hpp"
#include "Surface.hpp"

class Scene
{
private:
  std::vector<std::shared_ptr<Surface>> surfaces;
  View viewer;
  
public:
  Scene(View v, std::vector<std::shared_ptr<Surface>> s);
  ~Scene();
  
  std::vector<std::shared_ptr<Surface>> get_surfaces();
  View get_view();
  
  friend std::ostream& operator << ( std::ostream& os, const Scene &s);
};

#endif /* Scene_hpp */
