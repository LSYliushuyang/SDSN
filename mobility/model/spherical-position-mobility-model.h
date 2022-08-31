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

#ifndef SPHERICAL_POSITION_MOBILITY_MODEL_H
#define SPHERICAL_POSITION_MOBILITY_MODEL_H

#include "mobility-model.h"
#include "ns3/nstime.h"
#include "geographic-positions.h"

namespace ns3 {

class SphericalPositionMobilityModel : public MobilityModel
{
public:
  static TypeId GetTypeId (void);

  SphericalPositionMobilityModel();
  virtual ~SphericalPositionMobilityModel();

  void SetVelocity(const Vector &velocity);

  virtual double GetDistanceFrom (Ptr<const SphericalPositionMobilityModel> position) const;


private:
  virtual Vector DoGetPosition (void) const;
  virtual void DoSetPosition (const Vector &position);
  virtual Vector DoGetVelocity (void) const;



  Time m_baseTime;  //!< the base time
  Vector m_baseSphericalPosition; //!< the base velocity
  Vector m_baseSphericalVelocity;
};
}

#endif


