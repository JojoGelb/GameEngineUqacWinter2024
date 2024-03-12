#pragma once

#include <simplege/pch/precomp.h>
#include <simplege/components/texture.h>

#include <simplege/components/visual.h>

namespace SimpleGE
{
  class LayerComponent : public VisualComponent
  {
  public:
    static constexpr auto Type = "Layer";

    using ShaderProgram = TextureComponent::ShaderProgram;

    LayerComponent(Entity& owner) : Component(owner) {}

    void Display(const CameraComponent& camera, const Timing& timing) override;
  
    private:
    std::unique_ptr<TextureComponent::ShaderProgram::VertexBufferType> combinedVertexBuffer;
      std::unique_ptr<TextureComponent::ShaderProgram::IndexBufferType> combinedIndexBuffer;

  private:
    int numberOfSprite = 0;
  };


} // namespace SimpleGE
