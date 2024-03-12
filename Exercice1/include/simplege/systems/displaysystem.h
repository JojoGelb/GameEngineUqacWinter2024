#pragma once

#include <simplege/pch/precomp.h>

#include <simplege/systems/system.h>

namespace SimpleGE
{
  class CameraComponent;
  class VisualComponent;

  struct DisplaySystemImpl;

  struct DisplaySystemConfig
  {
    std::string_view title;
    std::uint32_t width{};
    std::uint32_t height{};
  };

  class DisplaySystem : public ISystem
  {
  public:
    static DisplaySystem& Instance()
    {
      static DisplaySystem instance;
      return instance;
    }

    static void SetConfig(const DisplaySystemConfig& config);

    DisplaySystem(const DisplaySystem&) = delete;
    DisplaySystem(DisplaySystem&&) = delete;
    DisplaySystem& operator=(const DisplaySystem&) = delete;
    DisplaySystem& operator=(DisplaySystem&&) = delete;
    ~DisplaySystem() override;

    void Register(gsl::not_null<CameraComponent*> comp);
    void Unregister(gsl::not_null<CameraComponent*> comp);
    void Register(gsl::not_null<VisualComponent*> comp);
    void Unregister(gsl::not_null<VisualComponent*> comp);

    void Iterate(const Timing& timing) override;

    [[nodiscard]] GFXAPI::Window& GetWindow();
    [[nodiscard]] GFXAPI::SwapChain& GetSwapChain();

  private:
    DisplaySystem();

    std::unique_ptr<DisplaySystemImpl> impl;
  };
} // namespace SimpleGE