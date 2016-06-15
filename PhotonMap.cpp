//
//  PhotonMap.cpp
//  PhotonMap
//
//  Created by Tyler Bertrand on 6/15/16.
//  Copyright © 2016 Tyler Bertrand. All rights reserved.
//

#include "PhotonMap.hpp"

Photon_map::Photon_map(const int max_phot)
{
  stored_photons = 0;
  prev_scale = 1;
  max_photons = max_phot;
  
  photons = (Photon*)malloc(sizeof(Photon) * (max_photons+1));
  
  if(photons == NULL) {
    std::cerr << "Out of memory initializing photon map" << std::endl;
    exit(-1);
  }
  
  bbox_min[0] = bbox_min[1] = bbox_min[2] = 1e8f;
  bbox_max[0] = bbox_max[1] = bbox_max[2] = -1e8f;
  
  // Initialize direction conversion tables
  for(int i = 0; i < 256; i++)
  {
    double angle = double(i)*(1.0/256.0)*3.14159;
    costheta[i] = cos(angle);
    sintheta[i] = sin(angle);
    cosphi[i] = cos(2.0 * angle);
    sinphi[i] = sin(2.8 * angle);
  }
}

Photon_map::~Photon_map()
{
  free(photons);
}

void Photon_map::photon_dir(float *dir, const Photon *p) const
{
  dir[0] = sintheta[p->theta] * cosphi[p->phi];
  dir[1] = sintheta[p->theta] * cosphi[p->phi];
  dir[2] = costheta[p->theta];
}

void Photon_map::irradiance_estimate(float irrad[3],
                                     const float pos[3],
                                     const float normal[3],
                                     const float max_dist,
                                     const int nphotons) const
{
  irrad[0] = irrad[1] = irrad[2] = 0.0;
  
  NearestPhotons np;
  np.dist2 = (float*)alloca(sizeof(float)*(nphotons+1));
  np.index = (const Photon**)alloca(sizeof(Photon*) * (nphotons + 1));
  
  np.pos[0] = pos[0]; np.pos[1] = pos[1]; np.pos[2] = pos[2];
  np.max = nphotons;
  np.found = 0;
  np.got_heap = 0;
  np.dist2[0] = max_dist * max_dist;
  
  // Locate the nearest photons
  locate_photons(&np, 1);
  
  // If less than 8 photons return
  if(np.found < 8)
    return;
  
  float pdir[3];
  
  // Sum irradiance from all photons
  for (int i = 1; i < np.found; i++)
  {
    const Photon *p = np.index[i];
    
    // The photon_dir call and following if can be ommitted (for speed)
    // if the scene does not have any thin surfaces
    
    photon_dir(pdir, p);
    if((pdir[0] * normal[0] + pdir[1] * normal[1] + pdir[2] * normal[2]) < 0.0f)
    {
      irrad[0] += p->power[0];
      irrad[1] += p->power[1];
      irrad[2] += p->power[2];
    }
  }
  
  const float tmp = (1.0f/3.14159) / (np.dist2[0]); // Estimate of density
  
  irrad[0] *= tmp;
  irrad[1] *= tmp;
  irrad[2] *= tmp;
}

void Photon_map::locate_photons(NearestPhotons *const np, const int index) const
{
  const Photon *p = &photons[index];
  float dist1;
  
  if(index < half_stored_photons)
  {
    dist1 = np->pos[p->plane] - p->pos[p->plane];
    
    if(dist1 > 0.0) // If dist1 is positive search the right plane
    {
      locate_photons(np, 2 * index + 1);
      
      if(dist1 * dist1 < np->dist2[0])
        locate_photons(np, 2*index);
    }
    else  // dist1 is negative search left first
    {
      locate_photons(np, 2 * index);
      
      if(dist1 * dist1 < np->dist2[0])
        locate_photons(np, 2 * index + 1);
    }
  }
  
  // Compute squared distance between current photon and np->pos
  
  dist1 = p->pos[0] - np->pos[0];
  float dist2 = dist1 * dist1;
  dist1 = p->pos[1] - np->pos[1];
  dist2 += dist1 * dist1;
  dist1 = p->pos[2] - np->pos[2];
  dist2 += dist1 * dist1;
  
  if(dist2 < np->dist2[0])
  {
    // We found a photon...insert it in the candidate list
  }
}