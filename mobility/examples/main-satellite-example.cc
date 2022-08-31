#include "ns3/core-module.h"
#include "ns3/mobility-module.h"

using namespace ns3;

static void PrintPosition(Ptr<Node> node)
{
  Vector Position;
  Vector Velocity;
  Position = node->GetObject<MobilityModel>()->GetPosition();
  Velocity = node->GetObject<SphericalPositionMobilityModel>()->GetVelocity();
  std::cout <<"Time: "<<Simulator::Now().GetSeconds()<<" x: "<< Position.x * 180 / M_PI << " y: " << Position.y * 180 / M_PI << " z: " << Position.z << std::endl;
  //std::cout <<"Velocity: "<<" x: "<<Velocity.x *180 / M_PI << " y: " <<Velocity.y *180 / M_PI << " z: " << Velocity.z <<std::endl;
}

int main (int argc, char *argv[])
{
  CommandLine cmd (__FILE__);
  cmd.Parse (argc, argv);

  NodeContainer c;
  c.Create (2);

  MobilityHelper mobility;
  mobility.SetPositionAllocator("ns3::ListPositionAllocator");

  mobility.SetMobilityModel("ns3::SphericalPositionMobilityModel");

  Ptr<PositionAllocator> position = mobility.GetPositionAllocator();
  position->GetObject<ListPositionAllocator>()->Add(Vector(0,0,6371e3 + 780));
  position->GetObject<ListPositionAllocator>()->Add(Vector(0,M_PI,6371e3 + 780));



  mobility.Install(c);


  c.Get(0)->GetObject<SphericalPositionMobilityModel>()->SetVelocity(Vector(3.14/2/10,0.0,0.0));
  c.Get(1)->GetObject<SphericalPositionMobilityModel>()->SetVelocity(Vector(-3.14/2/10,0.0,0.0));
  Simulator::Stop (Seconds (24*3600.0));
  for(int i = 1;i< 24 * 60;i++){
      Simulator::Schedule(Seconds(i * 60), &PrintPosition, c.Get(0));
      Simulator::Schedule(Seconds(i * 60), &PrintPosition, c.Get(1));
  }

  Simulator::Run ();



  Simulator::Destroy ();


  return 0;
}
