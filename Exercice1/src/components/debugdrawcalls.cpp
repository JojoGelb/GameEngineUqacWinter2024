#include <simplege/pch/precomp.h>

#include <simplege/simplege.h>

namespace SimpleGE
{
  using namespace std::chrono_literals;

  static constexpr auto DebugInterval = 5s;

  DebugDrawCallsComponent::DebugDrawCallsComponent(Entity& owner) : Component(owner) {}

  DebugDrawCallsComponent::~DebugDrawCallsComponent() = default;

  void DebugDrawCallsComponent::UpdateLogic(const Timing& timing)
  {
    frames++;
    elapsed += timing.delta;
    if (elapsed > DebugInterval)
    {
      auto calls = GFXAPI::CommandBuffer::GetAndResetDrawCount();
      float avgCalls = float(calls) / float(frames);
      fmt::print("Nb appels de rendu moyens par trame: {}\n", avgCalls);
      frames = 0;
      elapsed = {};
    }
  }
} // namespace SimpleGE
