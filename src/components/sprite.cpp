#include <BreadEngine/components/sprite.hpp>

namespace brd
{
  Sprite::Sprite(core::brdID entityID) : core::Component(entityID)
  {
    graph = Surface_ptr(SDL_CreateRGBSurface(0,50,50,32,0,0,0,0));

    SDL_LockSurface(graph.get());
    SDL_memset(graph.get()->pixels, 0xFF0000, graph.get()->h * graph.get()->pitch);

    SDL_UnlockSurface(graph.get());
  }
};
