#pragma once

#include <BreadEngine/typedeclarations.hpp>
#include <BreadEngine/core/context.hpp>
#include <BreadEngine/core/entities.hpp>
#include <BreadEngine/core/system.hpp>
#include <BreadEngine/components/script.hpp>

#include <utility>

namespace brd
{
  /*struct ScriptingConfiguration : public core::SystemConfiguration
  {
    std::optional<core::Context*> ctxt;
  };*/

  struct ScriptWrapper : public core::Component
  {
    std::unique_ptr<Script> script;

    template<typename T>
    explicit ScriptWrapper(core::brdID entityID, T&& script_) : core::Component(entityID), script(std::move(script_)) {}
  };

  class Scripting : public core::System
  {
    public:
      explicit Scripting() = default;
      virtual ~Scripting() = default;

      virtual void Configure(core::SystemConfiguration& conf) noexcept override {}
      virtual void Update(core::Context& ctxt) noexcept override;

      template<typename T>
      void AddScript(core::Entity& entity, core::Context& ctxt) noexcept
      {
        auto& sw = ctxt.AddComponent<ScriptWrapper>(entity, std::make_unique<T>(entity.id));
        sw.script->Start(ctxt);
      }
    private:
      //ScriptingConfiguration configuration;
  };
};
