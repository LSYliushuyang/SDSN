#ifndef POINT_TO_POINT_WIRELESS_CHANNEL_H
#define POINT_TO_POINT_WIRELESS_CHANNEL_H

#include <list>
#include "ns3/channel.h"
#include "ns3/ptr.h"
#include "ns3/nstime.h"
#include "ns3/data-rate.h"
#include "ns3/traced-callback.h"
#include "ns3/spherical-position-mobility-model.h"

#include "ns3/propagation-delay-model.h"
#include "point-to-point-wireless-net-device.h"

namespace ns3 {

class PointToPointWirelessNetDevice;
class Packet;
class ConstantSpeedPropagationDelayModel;

class PointToPointWirelessChannel : public Channel
{
public:
  static TypeId GetTypeId (void);
  PointToPointWirelessChannel();

  void Attach (Ptr<PointToPointWirelessNetDevice> device);
  virtual bool TransmitStart (Ptr<const Packet> p, Ptr<PointToPointWirelessNetDevice> src, Time txTime);
  virtual std::size_t GetNDevices (void) const;
  Ptr<PointToPointWirelessNetDevice> GetPointToPointDevice (std::size_t i) const;
  virtual Ptr<NetDevice> GetDevice (std::size_t i) const;
  void SetPropagationDelayModel (const Ptr<ConstantSpeedPropagationDelayModel> delay);
  Ptr<ConstantSpeedPropagationDelayModel> GetPropagationDelayModel(void);
  Time GetDelay(void);
protected:
  bool IsInitialized (void) const;
  Ptr<PointToPointWirelessNetDevice> GetSource (uint32_t i) const;
  Ptr<PointToPointWirelessNetDevice> GetDestination (uint32_t i) const;
  typedef void (* TxRxAnimationCallback)
     (Ptr<const Packet> packet,
      Ptr<NetDevice> txDevice, Ptr<NetDevice> rxDevice,
      Time duration, Time lastBitTime);
private:

  static const std::size_t N_DEVICES = 2;
  Ptr<ConstantSpeedPropagationDelayModel> m_delay;
  std::size_t        m_nDevices;
  TracedCallback<Ptr<const Packet>,     // Packet being transmitted
                 Ptr<NetDevice>,  // Transmitting NetDevice
                 Ptr<NetDevice>,  // Receiving NetDevice
                 Time,                  // Amount of time to transmit the pkt
                 Time                   // Last bit receive time (relative to now)
                 > m_txrxPointToPoint;
  enum WireState
   {
     /** Initializing state */
     INITIALIZING,
     /** Idle state (no transmission from NetDevice) */
     IDLE,
     /** Transmitting state (data being transmitted from NetDevice. */
     TRANSMITTING,
     /** Propagating state (data is being propagated in the channel. */
     PROPAGATING
   };

  class Link
  {
public:
    /** \brief Create the link, it will be in INITIALIZING state
     *
     */
    Link() : m_state (INITIALIZING), m_src (0), m_dst (0) {}

    WireState                  m_state; //!< State of the link
    Ptr<PointToPointWirelessNetDevice> m_src;   //!< First NetDevice
    Ptr<PointToPointWirelessNetDevice> m_dst;   //!< Second NetDevice
  };

  Link    m_link[N_DEVICES]; //!< Link model

};


}

#endif
