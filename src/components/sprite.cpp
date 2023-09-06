#include <BreadEngine/components/sprite.hpp>

namespace brd
{
  Sprite::Sprite(core::brdID entityID) : core::Component(entityID)
  {
    graph = SDL_CreateRGBSurface(0,50,50,32,0,0,0,0);

    SDL_LockSurface(graph);

    Uint32 color = SDL_MapRGB(graph->format, 255, 0, 0);
    //SDL_memset(graph->pixels, color, graph->h * graph->pitch);
    for(int j = 0; j < graph->h; ++j)
    {
      for(int i = 0; i < graph->w; ++i)
      {
        int bpp = graph->format->BytesPerPixel;
        Uint8 *p = (Uint8 *)graph->pixels + j * graph->pitch + i * bpp;
        *(Uint32 *)p = color;
      }
    }

    SDL_UnlockSurface(graph);
  }

  Sprite::~Sprite()
  {
    SDL_FreeSurface(graph);
  }
};
