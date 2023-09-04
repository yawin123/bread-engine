#include <BreadEngine/system/render.hpp>

extern "C" {
  #include <SDL2/SDL.h>
  #include <SDL2/SDL_image.h>
  #include <SDL2/SDL_ttf.h>
}

#include <iostream>

namespace brd
{
  Render::Render()
  {
    SDL_Init(SDL_INIT_EVERYTHING);
    std::cout << "SDL_Init(SDL_INIT_EVERYTHING)" << std::endl;

    IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);
    std::cout << "IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG)" << std::endl;

    TTF_Init();
    std::cout << "TTF_Init()" << std::endl;
  }

  Render::~Render()
  {
    TTF_Quit();
    std::cout << "TTF_Quit()" << std::endl;
  	IMG_Quit();
    std::cout << "IMG_Quit()" << std::endl;
    SDL_Quit();
    std::cout << "SDL_Quit()" << std::endl;
  }

  void Render::Configure(core::SystemConfiguration& conf)
  {
    configuration = reinterpret_cast<RenderConfiguration&>(conf);
    window = Window_ptr(SDL_CreateWindow(
                  configuration.title.c_str(),
                  SDL_WINDOWPOS_CENTERED,
                  SDL_WINDOWPOS_CENTERED,
                  configuration.width,
                  configuration.height,
                  configuration.windowMode
              ));

    renderer = Renderer_ptr(SDL_CreateRenderer(window.get(), configuration.index, configuration.rendererMode));
    SDL_SetRenderDrawColor(renderer.get(),0,0,0,255);
  }

  void Render::Update()
  {
    SDL_RenderClear(renderer.get());
    SDL_RenderPresent(renderer.get());
  }
};
