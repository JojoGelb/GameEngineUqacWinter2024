//--------------------Mon Code----------------------------
#include "chickendodge/pch/precomp.h"

namespace ChickenDodge
{
  struct NetworkSendSingleScore : public SimpleGE::Message<NetworkSendSingleScore>
  {
    static constexpr MessageType Type = FirstGameType + 3;

    std::string name;
    int score;

    NetworkSendSingleScore() = default;
    NetworkSendSingleScore(std::string name, int score) : name(name), score(score) {}

    void Serialize(SimpleGE::ISerializer& serializer) const override
    {
      BaseMessage::Serialize(serializer);
      serializer.Write(score);
      serializer.Write(name);
    }

    void Deserialize(SimpleGE::IDeserializer& deserializer) override
    {
      BaseMessage::Deserialize(deserializer);
      deserializer.Read(score);
      deserializer.Read(name);
    }
  };
} // namespace ChickenDodge
//--------------------Mon Code----------------------------