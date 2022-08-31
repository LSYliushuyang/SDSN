/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Author: lsy
 */

#include "spherical-position-mobility-model.h"
#include "ns3/simulator.h"

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (SphericalPositionMobilityModel);

TypeId SphericalPositionMobilityModel::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::SphericalPositionMobilityModel")
    .SetParent<MobilityModel> ()
    .SetGroupName ("Mobility")
    .AddConstructor<SphericalPositionMobilityModel> ();
  return tid;
}

SphericalPositionMobilityModel::SphericalPositionMobilityModel()
{
}

SphericalPositionMobilityModel::~SphericalPositionMobilityModel()
{
}

void
SphericalPositionMobilityModel::SetVelocity(const Vector &velocity) // Vector(rad,rad,km)
{
  m_baseSphericalVelocity = velocity;
  NotifyCourseChange ();
}

Vector
SphericalPositionMobilityModel::DoGetVelocity (void) const
{
  double t = (Simulator::Now () - m_baseTime).GetSeconds ();
  Vector CurVelocity;
  CurVelocity.y = m_baseSphericalVelocity.y;
  CurVelocity.z = m_baseSphericalVelocity.z;
  double T = 2 * M_PI * sqrt(pow( m_baseSphericalPosition.z,3)/
                             (GRAVITATIONAL_CONST * EARTH_MASS));
 // std::cout<<"T(780km): "<<T<<std::endl;
  double angv = 2 * M_PI / T;
  while(t > T){
      t -= T;
  }
  if(m_baseSphericalVelocity.x > 0){
      if(t < (M_PI / 2 - m_baseSphericalPosition.x) / angv ||
          t >= (3 * M_PI / 2 - m_baseSphericalPosition.x) / angv){
          CurVelocity.x = m_baseSphericalVelocity.x;
      }
      else CurVelocity.x = - m_baseSphericalVelocity.x;
  }
  else {
      if(t < (m_baseSphericalPosition.x - M_PI / 2) / angv ||
          t >= (m_baseSphericalPosition.x - 3 * M_PI / 2) / angv){
          CurVelocity.x = m_baseSphericalVelocity.x;
      }
      else CurVelocity.x = - m_baseSphericalVelocity.x;
  }
  return CurVelocity;
}

Vector
SphericalPositionMobilityModel::DoGetPosition (void) const
{
  double t = (Simulator::Now () - m_baseTime).GetSeconds ();
  double angv = 1.0 / sqrt(pow( m_baseSphericalPosition.z,3)/
                           (GRAVITATIONAL_CONST * EARTH_MASS));
 // std::cout<<"angv(780km): "<<angv<<std::endl;
  Vector CurPosition;
  CurPosition.z = m_baseSphericalPosition.z;
  CurPosition.y = m_baseSphericalPosition.y - t * EARTH_ROTATION_ANGV < - M_PI ?
      m_baseSphericalPosition.y - t * EARTH_ROTATION_ANGV + 2 * M_PI :
      m_baseSphericalPosition.y - t * EARTH_ROTATION_ANGV;
  double delta_rad = t * angv;
  while(delta_rad > 2 * M_PI){
      delta_rad -= 2 * M_PI;
  }
  double temp_rad;
  if(m_baseSphericalVelocity.x > 0){
      temp_rad = m_baseSphericalPosition.x + delta_rad;
      if(temp_rad <= M_PI / 2){
          CurPosition.x = temp_rad;
      }
      else if(temp_rad <= 3 * M_PI /2){
          CurPosition.x = M_PI - temp_rad;
      }
      else
        CurPosition.x = temp_rad - 2 * M_PI;
  }
  else {
      temp_rad = m_baseSphericalPosition.x - delta_rad;
      if(temp_rad >= - M_PI / 2){
          CurPosition.x = temp_rad;
      }
      else if(temp_rad >= - 3 * M_PI /2){
          CurPosition.x = - M_PI - temp_rad;
      }
      else
        CurPosition.x = temp_rad + 2 * M_PI;
  }
  return CurPosition;
}

void
SphericalPositionMobilityModel::DoSetPosition (const Vector &position)
{
  m_baseTime = Simulator::Now ();
  m_baseSphericalPosition = Vector (position.x,position.y,position.z);
  NotifyCourseChange ();
}

double
SphericalPositionMobilityModel::GetDistanceFrom (Ptr<const SphericalPositionMobilityModel> other) const
{
  Vector oPosition = other->DoGetPosition ();
  Vector position = DoGetPosition ();

 // std::cout<<"("<<oPosition.x*RAD2DEG<<" "<<oPosition.y*RAD2DEG<<") & ("<<
 //     position.x*RAD2DEG<<" "<<position.y*RAD2DEG<<" "<<")"<<std::endl;


  Vector a = GeographicPositions::GeographicToCartesianCoordinates
      (position.x * RAD2DEG,position.y * RAD2DEG,(position.z-EARTH_RADIUS)*1000.0,GeographicPositions::SPHERE);
  Vector b = GeographicPositions::GeographicToCartesianCoordinates
      (oPosition.x * RAD2DEG,oPosition.y * RAD2DEG,(oPosition.z-EARTH_RADIUS)*1000.0,GeographicPositions::SPHERE);
  //std::cout<<"distance:"<<sqrt(pow(a.x-b.x,2)+pow(a.y-b.y,2)+pow(a.z-b.z,2))/1000.0<<"km "<< CalculateDistance (a,b)<<std::endl;

  return sqrt(pow(a.x-b.x,2)+pow(a.y-b.y,2)+pow(a.z-b.z,2))/1000.0;//CalculateDistance (a,b);
}



}
