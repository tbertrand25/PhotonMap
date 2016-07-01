//
//  PhotonMap.hpp
//  PhotonMap
//
//  Implementation mostly borrowed from Henrik Wann Jensen's book
//  "Realistic Image Synthesis using Photon Mapping"
//

#ifndef PhotonMap_hpp
#define PhotonMap_hpp

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <math.h>

#include <armadillo>

// Photon
typedef struct Photon {
  float pos[3]; // Photon position
  short plane; // Splitting plane for k-d tree
  unsigned char theta, phi; // Incoming direction
  float power[3]; // Photon power
} Photon;

typedef struct NearestPhotons {
  int max;
  int found;
  int got_heap;
  float pos[3];
  float *dist2;
  const Photon **index;
} NearestPhotons;

class Photon_map {
public:
  Photon_map(int max_phot);
  ~Photon_map();
  
  void store(const float power[3],
             const float pos[3],
             const float dir[3]);
  
  void scale_photon_power(const float scale);
  
  void balance(void);
  
  void irradiance_estimate(float irrad[3],
                           const float pos[3],
                           const float normal[3],
                           const float max_dist,
                           const int nphotons) const;
  
  void locate_photons(NearestPhotons *const np,
                      const int index) const;
  
  void photon_dir(float *dir, const Photon *p) const;
  
  Photon *photons;
  
  int get_stored_photons() { return stored_photons; }
  
//  double dot(float* a, float* b) {
//    return (a[0]*b[0]) + (a[1]*b[1]) + (a[2]*b[2]);
//  }
  
private:
  void balance_segment(Photon **pbal,
                       Photon **porg,
                       const int index,
                       const int start,
                       const int end);
  
  void median_split(Photon **p,
                    const int start,
                    const int end,
                    const int median,
                    const int axis);
  
  
  
  int stored_photons;
  int half_stored_photons;
  int max_photons;
  int prev_scale;
  
  float costheta[256];
  float sintheta[256];
  float cosphi[256];
  float sinphi[256];
  
  float bbox_min[3];
  float bbox_max[3];
};

#endif /* PhotonMap_hpp */