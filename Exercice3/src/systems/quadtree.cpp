#include "simplege/systems/quadtree.h"
#include <iostream>
#include <string>
namespace SimpleGE
{

  void QuadTree::DisplayTree()
  {
    std::cout << "-----------NEW TREE-------------\n";
    if (childrens.size() == 0)
    {
      for (int i = 0; i < colliders.size(); i++)
      {
        std::cout << "-"
                  << " Possible collide: x=" << colliders[i]->GetArea().x() << "/ y=" << colliders[i]->GetArea().y()
                  << std::endl;
      }
      return;
    }

    std::cout << "NW: \n";
    childrens[0].DisplayTreeRecur(1);
    std::cout << "NE: \n";
    childrens[1].DisplayTreeRecur(1);
    std::cout << "SE: \n";
    childrens[2].DisplayTreeRecur(1);
    std::cout << "SW: \n";
    childrens[3].DisplayTreeRecur(1);

    std::cout << "-----------END TREE-------------\n";
  }

  void QuadTree::DisplayTreeRecur(int depth)
  {
    std::string str = "";
    for (int j = 0; j < depth; j++)
    {
      str += "-";
    }

    if (childrens.size() == 0)
    {
      for (int i = 0; i < colliders.size(); i++)
      {
        std::cout << str << " Possible collide: x=" << colliders[i]->GetArea().x()
                  << "/ y=" << colliders[i]->GetArea().y() << std::endl;
      }
    }
    else
    {
      std::cout << str << "NW: \n";
      childrens[0].DisplayTreeRecur(depth + 1);
      std::cout << str << "NE: \n";
      childrens[1].DisplayTreeRecur(depth + 1);
      std::cout << str << "SE: \n";
      childrens[2].DisplayTreeRecur(depth + 1);
      std::cout << str << "SW: \n";
      childrens[3].DisplayTreeRecur(depth + 1);
    }
  }

  void QuadTree::GetPotentialCollisions(
      std::vector<std::pair<gsl::not_null<ColliderComponent*>, gsl::not_null<ColliderComponent*>>>& result)
  {
    if (childrens.size() == 0)
    {
      for (int i = 0; i < std::size(colliders); i++)
      {
        auto c1 = colliders[i];

        for (int j = i + 1; j < std::size(colliders); j++)
        {
          auto c2 = colliders[j];

          if (c1->Collides(*c2))
          {
            result.emplace_back(c1, c2);
          }
        }
      }
    }
    else
    {
      for (int i = 0; i < std::size(childrens); i++)
      {
        childrens[i].GetPotentialCollisions(result);
      }
    }
  }

  bool QuadTree::Insert(ColliderComponent* collider)
  {
    if (!boundaries.Intersects(collider->GetArea()))
      return false;

    if (colliders.size() < capacity && childrens.size() == 0)
    {
      colliders.push_back(collider);
      return true;
    }
    else
    {
      if (childrens.size() == 0)
      {
        Subdivide();
      }

      for (int i = 0; i < childrens.size(); i++) // Insert new collision in children squares
      {
        if (childrens[i].boundaries.Intersects(collider->GetArea()))
        {
          return childrens[i].Insert(collider);
        }
      }

      return false;
    }
  }

  void QuadTree::Subdivide()
  {
    float x = boundaries.x();
    float y = boundaries.y();
    float w = boundaries.width();
    float h = boundaries.height();

    // NW
    childrens.push_back(QuadTree(Area(x - w / 4, y - h / 4, w / 2, h / 2), capacity));
    // NE
    childrens.push_back(QuadTree(Area(x + w / 4, y - h / 4, w / 2, h / 2), capacity));
    // SE
    childrens.push_back(QuadTree(Area(x + w / 4, y + h / 4, w / 2, h / 2), capacity));
    // SW
    childrens.push_back(QuadTree(Area(x - w / 4, y + h / 4, w / 2, h / 2), capacity));

    for (int j = colliders.size() - 1; j > -1; j--) // Give colliders to children squares
    {
      for (int i = 0; i < childrens.size(); i++)
      {
        if (childrens[i].Insert(colliders[j]))
        {
          colliders.pop_back();
          break;
        }
      }
    }
    colliders.clear();
  }

} // namespace SimpleGE