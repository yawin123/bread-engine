#pragma once

#include <memory>
#include <cstdint>
#include <string>

#include <BreadEngine/typedeclarations.hpp>
#include <BreadEngine/core/system.hpp>

namespace brd
{
  struct RenderConfiguration : public core::SystemConfiguration
  {
    int width {0}, height {0};

    std::string title {""};
    int      index {0};
    uint32_t windowMode {SDL_WINDOW_OPENGL | SDL_WINDOW_MOUSE_FOCUS};
    uint32_t rendererMode {SDL_RENDERER_ACCELERATED};
  };

  class Render : public core::System
  {
    public:
      explicit Render();
      virtual ~Render();

      virtual void Configure(core::SystemConfiguration& conf) override;
      virtual void Update(core::Context& ctxt);

    private:
      RenderConfiguration configuration;

      Window_ptr window;
      Renderer_ptr renderer;
  };
};
