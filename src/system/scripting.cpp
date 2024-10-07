#include <BreadEngine/system/scripting.hpp>
#include <iostream>

namespace brd
{
  void Scripting::Configure(core::SystemConfiguration& conf)
  {
    configuration = static_cast<ScriptingConfiguration&>(conf);
  }

  void Scripting::Update(core::Context& ctxt)
  {
    auto& sws = configuration.ctxt.value()->GetComponents<ScriptWrapper>().data();
    for(auto& sw : sws)
    {
      sw.script->Update(*configuration.ctxt.value());
    }
  }
}
