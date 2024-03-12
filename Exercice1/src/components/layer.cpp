#include <simplege/pch/precomp.h>

#include <simplege/simplege.h>

#include <iostream>

namespace SimpleGE
{
  static std::vector<gsl::not_null<SpriteComponent*>> ListSprites(const EntityRef& owner)
  {
    std::queue<EntityRef> queue;
    queue.push(owner);

    std::vector<gsl::not_null<SpriteComponent*>> sprites;

    while (!queue.empty())
    {
      auto node = queue.front();
      queue.pop();

      for (const auto& child : node->GetChildren())
      {
        if (child->Active())
        {
          queue.push(child);
        }

        auto* comp = child->GetComponent<SpriteComponent>();
        if (comp != nullptr)
        {
          sprites.emplace_back(comp);
        }
      }
    }

    return sprites;
  }

  void LayerComponent::Display(const CameraComponent& camera, const Timing& timing)
  {        

    auto layerSprites = ListSprites(Owner().shared_from_this());
    if (layerSprites.empty())
    {
      return;
    }

    auto spriteSheet = gsl::at(layerSprites, 0)->GetSpriteSheet();
    Ensures(spriteSheet.Ready());

    auto indiceMemorySize = std::size(SpriteComponent::Indices);

    if (numberOfSprite != layerSprites.size())
    {
      numberOfSprite = layerSprites.size();
      combinedVertexBuffer = TextureComponent::ShaderProgram::CreateVertexBuffer(numberOfSprite * 4);
      combinedIndexBuffer = TextureComponent::ShaderProgram::CreateIndexBuffer(numberOfSprite * indiceMemorySize);
    }

    auto& ubo = spriteSheet->GetUniformBuffer();
    ubo.model = glm::identity<glm::mat4>();
    ubo.view = glm::identity<glm::mat4>();
    ubo.proj = camera.Projection();

        
    int indexVBuffer = 0;
    int indexIBuffer = 0;

    std::array<ShaderProgram::Index, 6> tempIndices;
    tempIndices = SpriteComponent::Indices;

    for (int i = 0; i < numberOfSprite; i++)
    {
      auto sprite = gsl::at(layerSprites, i);

      auto bufferVertices = combinedVertexBuffer->Map({indexVBuffer, std::size(sprite.get()->vertexArray)});
      std::copy(sprite.get()->vertexArray.begin(), sprite.get()->vertexArray.end(), bufferVertices.Get().begin());
      indexVBuffer += sizeof(sprite.get()->vertexArray);

      auto bufferIndices = combinedIndexBuffer->Map({(long long) (indexIBuffer * sizeof(tempIndices)), indiceMemorySize});
      std::copy(tempIndices.begin(), tempIndices.end(), bufferIndices.Get().begin());
      indexIBuffer += 1;

      for (int j = 0; j < std::size(tempIndices); j++)
      {
        tempIndices.at(j) += std::size(sprite.get()->vertexArray);
      }
    }

    spriteSheet->Draw(camera, *combinedVertexBuffer, *combinedIndexBuffer, indiceMemorySize*numberOfSprite);
  }
} // namespace SimpleGE