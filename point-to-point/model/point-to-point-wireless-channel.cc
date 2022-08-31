#include "point-to-point-wireless-channel.h"

#include "ns3/trace-source-accessor.h"
#include "ns3/packet.h"
#include "ns3/simulator.h"
#include "ns3/log.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("PointToPointWirelessChannel");

NS_OBJECT_ENSURE_REGISTERED (PointToPointWirelessChannel);

TypeId
PointToPointWirelessChannel::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::PointToPointWirelessChannel")
    .SetParent<Channel> ()
    .SetGroupName ("PointToPoint")
    .AddConstructor<PointToPointWirelessChannel> ()
    .AddTraceSource ("TxRxPointToPoint",
                     "Trace source indicating transmission of packet "
                     "from the PointToPointChannel, used by the Animation "
                     "interface.",
                     MakeTraceSourceAccessor (&PointToPointWirelessChannel::m_txrxPointToPoint),
                     "ns3::PointToPointWirelessChannel::TxRxAnimationCallback")
  ;
  return tid;
}

PointToPointWirelessChannel::PointToPointWirelessChannel()
  :
    Channel (),
    m_nDevices (0)
{
  NS_LOG_FUNCTION_NOARGS ();
}


void
PointToPointWirelessChannel::Attach (Ptr<PointToPointWirelessNetDevice> device)
{
  NS_LOG_FUNCTION (this << device);
  NS_ASSERT_MSG (m_nDevices < N_DEVICES, "Only two devices permitted");
  NS_ASSERT (device != 0);

  m_link[m_nDevices++].m_src = device;
//
// If we have both devices connected to the channel, then finish introducing
// the two halves and set the links to IDLE.
//
  if (m_nDevices == N_DEVICES)
    {
      m_link[0].m_dst = m_link[1].m_src;
      m_link[1].m_dst = m_link[0].m_src;
      m_link[0].m_state = IDLE;
      m_link[1].m_state = IDLE;
    }
}

void
PointToPointWirelessChannel::SetPropagationDelayModel (const Ptr<ConstantSpeedPropagationDelayModel> delay)
{
  NS_LOG_FUNCTION (this << delay);
  m_delay = delay;
}

Ptr<ConstantSpeedPropagationDelayModel>
PointToPointWirelessChannel::GetPropagationDelayModel(void)
{
  return m_delay;
}
Time
PointToPointWirelessChannel::GetDelay(void)
{

  Ptr<MobilityModel> a =   this->GetDevice(0)->GetNode()->GetObject<MobilityModel>();
  Ptr<MobilityModel> b = this->GetDevice(1)->GetNode()->GetObject<MobilityModel>();
  return m_delay->GetDelay(a, b);
}

bool
PointToPointWirelessChannel::TransmitStart (
  Ptr<const Packet> p,
  Ptr<PointToPointWirelessNetDevice> src,
  Time txTime)
{
  NS_LOG_FUNCTION (this << p << src);
  NS_LOG_LOGIC ("UID is " << p->GetUid () << ")");

  NS_ASSERT (m_link[0].m_state != INITIALIZING);
  NS_ASSERT (m_link[1].m_state != INITIALIZING);

  uint32_t wire = src == m_link[0].m_src ? 0 : 1;

  Ptr<MobilityModel> sendermobility = src->GetNode()->GetObject<MobilityModel>();
  Ptr<MobilityModel> receivermobility = m_link[wire].m_dst->GetNode ()->GetObject<MobilityModel>();


  Time delay;
  delay = m_delay->GetDelay(sendermobility, receivermobility);

  Simulator::ScheduleWithContext (m_link[wire].m_dst->GetNode ()->GetId (),
                                  txTime + delay, &PointToPointWirelessNetDevice::Receive,
                                  m_link[wire].m_dst, p->Copy ());

  // Call the tx anim callback on the net device
  m_txrxPointToPoint (p, src, m_link[wire].m_dst, txTime, txTime + delay);
  return true;
}

std::size_t
PointToPointWirelessChannel::GetNDevices (void) const
{
  NS_LOG_FUNCTION_NOARGS ();
  return m_nDevices;
}

Ptr<PointToPointWirelessNetDevice>
PointToPointWirelessChannel::GetPointToPointDevice (std::size_t i) const
{
  NS_LOG_FUNCTION_NOARGS ();
  NS_ASSERT (i < 2);
  return m_link[i].m_src;
}

Ptr<NetDevice>
PointToPointWirelessChannel::GetDevice (std::size_t i) const
{
  NS_LOG_FUNCTION_NOARGS ();
  return GetPointToPointDevice (i);
}

Ptr<PointToPointWirelessNetDevice>
PointToPointWirelessChannel::GetSource (uint32_t i) const
{
  return m_link[i].m_src;
}

Ptr<PointToPointWirelessNetDevice>
PointToPointWirelessChannel::GetDestination (uint32_t i) const
{
  return m_link[i].m_dst;
}

bool
PointToPointWirelessChannel::IsInitialized (void) const
{
  NS_ASSERT (m_link[0].m_state != INITIALIZING);
  NS_ASSERT (m_link[1].m_state != INITIALIZING);
  return true;
}

}
