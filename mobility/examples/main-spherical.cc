#include "ns3/core-module.h"
#include "ns3/mobility-module.h"

using namespace ns3;

static void PrintPosition(Ptr<Node> node);

int main(){

  NodeContainer c;
  c.Create(1);
  ObjectFactory mobility;
  mobility.SetTypeId("ns3::SphericalPositionMobilityModel");


  Ptr<Object> object = c.Get(0);
  Ptr<MobilityModel> model = object->GetObject<MobilityModel>();
  model = mobility.Create()->GetObject<MobilityModel>();
  object->AggregateObject(model);
  model->SetPosition(Vector(0.0,0.0,6371e3 + 780));
  model->GetObject<SphericalPositionMobilityModel>()->SetVelocity(Vector(3.14/2/10,2 * 3.14 / 20 ,0.0));
  Simulator::Stop (Seconds (24*3600.0));
  for(int i = 1;i< 24 * 60;i++){
      Simulator::Schedule(Seconds(i * 60), &PrintPosition, c.Get(0));
  }

  Simulator::Run ();



  Simulator::Destroy ();

  return 0;
}

static void PrintPosition(Ptr<Node> node)
{
  Vector Position;
  Vector Velocity;
  Position = node->GetObject<MobilityModel>()->GetPosition();
  Velocity = node->GetObject<SphericalPositionMobilityModel>()->GetVelocity();
  std::cout <<"Time: "<<Simulator::Now().GetSeconds()<<" x: "<< Position.x * 180 / M_PI << " y: " << Position.y * 180 / M_PI << " z: " << Position.z << std::endl;
  std::cout <<"Velocity: "<<" x: "<<Velocity.x *180 / M_PI << " y: " <<Velocity.y *180 / M_PI << " z: " << Velocity.z <<std::endl;
}
