#include <BreadEngine/system/scripting.hpp>
#include <iostream>

namespace brd
{
  void Scripting::Update(core::Context& ctxt)
  {
    auto& sws = ctxt.GetComponents<ScriptWrapper>().data();
    for(auto& sw : sws)
    {
      sw.script->Update(ctxt);
    }
  }
}
