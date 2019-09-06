#include "s-ol.hpp"

Plugin *pluginInstance;

void init(rack::Plugin *p) {
  pluginInstance = p;

  p->addModel(modelCircleVCO);
  p->addModel(modelWrapComp);
  p->addModel(modelModulo);
}
