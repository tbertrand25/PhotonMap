//
//  Quad.cpp
//  PhotonMap
//
//  Created by Tyler Bertrand on 6/11/16.
//  Copyright © 2016 Tyler Bertrand. All rights reserved.
//

#include "Quad.hpp"
#include "Tracer.h"

Quad::Quad(std::istream &ins)
{
  read(ins);
}

Quad::~Quad()
{
  
}

bool Quad::intersect(std::shared_ptr<Ray> r, std::shared_ptr<Hit> h)
{
  // Holds first 3 vertices of each face of the quad
  arma::vec3 a, b, c;
  
  // Holds the normal for each face of the quad
  arma::vec3 normal;
  
  // Holds the distance ray r travels to hit each face of the quad
  double t;
  
  // Holds value of t's denominator to check for division by zero
  double t_denominator;
  
  // Holds point where ray r hits each plane in the quad
  arma::vec3 hit_pt;
  
  // Holds indices for two axes to use in Jordan Curve algorithm
  int u, v;
  
  // Holds distance to translate each point on the face for Jordan Curve algorithm
  double trans_u, trans_v;
  
  const int x_comp = 0;
  const int y_comp = 1;
  const int z_comp = 2;
  
  const int u_comp = 0;
  const int v_comp = 1;
  
  int nc;
  int sh;
  int nsh;
  int next;
  double ua, ub, va, vb;
  
  std::vector<arma::vec2> trans_pts;
  arma::vec2 trans_pt;
  
  // Iterate through all faces in the quad
  for(auto face : vert_order)
  {
    //calculate normal of polygon face
    arma::vec3 pt1 = vertices[face[0]];
    
    arma::vec3 pt2 = vertices[face[1]];
    
    arma::vec3 pt3 = vertices[face[2]];
    
    arma::vec3 pt4 = vertices[face[3]];
    
    arma::vec3 normal = arma::normalise(arma::cross(pt2 - pt1, pt3 - pt1));
    
    //determine the points on the plane
    t_denominator = dot(r->get_direction(), normal);
    
    if(t_denominator < tracer::epsilon && t_denominator > -tracer::epsilon)
      continue;
    
    t = arma::dot(pt1 - r->get_origin(), normal) / t_denominator;    //calculate t value for hit
    
    if(t < -tracer::epsilon)
      continue;
    
    arma::vec3 hit_pt = r->get_origin() + (t * r->get_direction());
    
    //--------- JORDAN CURVE ----------
    
    //determine which axis to throw out, converting to (u,v) space
    if(std::abs(normal[2]) > std::abs(normal[0]) && std::abs(normal[2]) > std::abs(normal[1]))
    {
      u = x_comp;
      v = y_comp;
    }
    else if(std::abs(normal[1]) > std::abs(normal[0]))
    {
      u = x_comp;
      v = z_comp;
    }
    else
    {
      u = y_comp;
      v = z_comp;
    }
    
    //determine how far to translate each point in the face
    trans_u = -hit_pt[u];
    trans_v = -hit_pt[v];
    
    //load a vector with the points of the face translated appropriately
    for(double pt : face)
    {
      trans_pt = arma::vec2({vertices[pt][u] + trans_u, vertices[pt][v] + trans_v});
      trans_pts.push_back(trans_pt);
    }
    
    nc = 0;
    sh = tracer::signum(trans_pts[0][v_comp]);
    for(int i = 0; i < trans_pts.size(); i++) //loop along each edge
    {
      next = (i+1)%trans_pts.size();
      ua = trans_pts[i][u_comp];             //u component of vertex a
      va = trans_pts[i][v_comp];             //v component of vertex a
      ub = trans_pts[next][u_comp];          //u component of vertex b
      vb = trans_pts[next][v_comp];          //v component of vertex b
      
      nsh = tracer::signum(vb);
      
      if(sh != nsh)
      {
        if(ua > tracer::epsilon && ub > tracer::epsilon)   //if both u components are positive
        {
          nc = nc + 1;
        }
        else if(ua < -tracer::epsilon && ub < -tracer::epsilon) //if both u components are negative
        {
          sh = nsh;
          continue;
        }
        else if(ua - (va * ((ub - ua)/(vb - va))) > tracer::epsilon)
        {
          nc = nc + 1;
        }
      }
      sh = nsh;
    }
    trans_pts.clear();
    
    //---------------------------------
    
    if(t < h->get_t() && nc % 2 == 1 && t > tracer::epsilon)  //if this hit is closer than the last hit
    {
      if(mat->get_is_textured() == true)
      {
        //Create vectors along two adjacent edges and vector to hit point
        arma::vec3 u_tex_vec = pt2 - pt1;
        arma::vec3 v_tex_vec = pt4 - pt1;
        arma::vec3 m_tex_vec = hit_pt - pt1;
        
        double v_length = sqrt((v_tex_vec(0) * v_tex_vec(0)) +
                               (v_tex_vec(1) * v_tex_vec(1)) +
                               (v_tex_vec(2) * v_tex_vec(2)));
        double u_length = sqrt((u_tex_vec(0) * u_tex_vec(0)) +
                               (u_tex_vec(1) * u_tex_vec(1)) +
                               (u_tex_vec(2) * u_tex_vec(2)));
    
        
        //Calculate projection of m_tex_vec onto u_tex_vec and v_tex_vec
        double alpha = arma::dot(m_tex_vec, v_tex_vec) / v_length;
        double beta = arma::dot(m_tex_vec, u_tex_vec) / u_length;
        
        //Calculate texture coordinates u, v
        int u = round(beta * (mat->get_tex_x_size() - 1));
        int v = round(alpha * (mat->get_tex_y_size() - 1));
        
        //Normalize texture coordinates u, v
        u /= u_length;
        v /= v_length;
        
        //Apply texture coordinates to hit
        arma::vec2 tex_coords = arma::vec2({static_cast<double>(v), static_cast<double>(u)});

        *h = Hit(t, hit_pt, normal, tex_coords, mat);
      }
      else
        *h = Hit(t, hit_pt, normal, mat);
    }
  }
  
  if(h->get_t() != std::numeric_limits<double>::infinity())
    return true;
  else
    return false;
}

void Quad::read(std::istream &ins)
{
  const std::string END_QUAD_TAG = "end_quad";
  
  const std::string VERTICES_TAG = "vertex";
  const std::string VERT_ORDER_TAG = "poly";
  const std::string MATERIAL_TAG = "begin_material";
  
  bool seen_end_tag = false;
  bool seen_vertices = false;
  bool seen_vert_order = false;
  bool seen_material = false;
  
  std::string tok;
  
  while(!ins.eof() && !seen_end_tag)
  {
    ins >> tok;
    
    if(tok == END_QUAD_TAG)
    {
      seen_end_tag = true;
    }
    else if(tok == VERTICES_TAG)
    {
      seen_vertices = true;
      double x, y, z;

      ins >> x;
      ins >> y;
      ins >> z;
      
      vertices.push_back(arma::vec3({x, y, z}));
    }
    else if(tok == VERT_ORDER_TAG)
    {
      seen_vert_order = true;
      int a, b, c, d;
      
      ins >> a;
      ins >> b;
      ins >> c;
      ins >> d;
    
      std::vector<int> temp({a, b, c, d});
      
      vert_order.push_back(temp);
    }
    else if(tok == MATERIAL_TAG)
    {
      seen_material = true;
      mat = std::make_shared<Material>(ins);
    }
  }
  // TODO: check that all required parameters have been seen
}

void Quad::print(std::ostream &os)
{
  os << "vertices:\n";
  for(auto i : vertices)
  {
    os << "  [" << i(0) << ", " << i(1) << ", " << i(2) << "]\n";
  }
  
  os << "vert_order:\n";
  for(auto i : vert_order)
  {
    os << "  [" << i[0] << ", " << i[1] << ", " << i[2] << ", " << i[3] << "]\n";
  }
  mat->print(os);
}