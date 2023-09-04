#pragma once

extern "C" {
  #include <SDL2/SDL.h>
}

namespace brd
{
  struct SDLWindowDeleter{
    void operator()(SDL_Window* window) {
      SDL_DestroyWindow(window);
    }
  };
  struct SDLRendererDeleter{
    void operator()(SDL_Renderer* renderer) {
      SDL_DestroyRenderer(renderer);
    }
  };
  struct SDLSurfaceDeleter{
    void operator()(SDL_Surface* surface) {
      SDL_FreeSurface(surface);
    }
  };

  using Window_ptr = std::unique_ptr<SDL_Window, SDLWindowDeleter>;
  using Renderer_ptr = std::unique_ptr<SDL_Renderer, SDLRendererDeleter>;
  using Surface_ptr = std::unique_ptr<SDL_Surface, SDLSurfaceDeleter>;

  using Vector2 = std::tuple<float&,float&>;
  using constVector2 = const std::tuple<const float&, const float&>;
};
