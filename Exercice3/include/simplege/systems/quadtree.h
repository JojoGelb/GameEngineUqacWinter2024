#pragma once

#include <simplege/components/collider.h>
#include <simplege/pch/precomp.h>

namespace SimpleGE
{
  class QuadTree
  {
  public:
    QuadTree(Area boundaries, int capacity) : boundaries(boundaries), capacity(capacity) {}

    bool Insert(ColliderComponent* collider);
    void Subdivide();

    void GetPotentialCollisions(
        std::vector<std::pair<gsl::not_null<ColliderComponent*>, gsl::not_null<ColliderComponent*>>>& result);

    void DisplayTree();

  private:
    void DisplayTreeRecur(int depth);

    Area boundaries;
    int capacity;

    std::vector<gsl::not_null<ColliderComponent*>> colliders;
    std::vector<QuadTree> childrens;
  };

} // namespace SimpleGE
