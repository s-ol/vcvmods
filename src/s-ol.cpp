#include "s-ol.hpp"

Plugin *plugin;

void init(rack::Plugin *p) {
  plugin = p;

  p->slug = "s-ol";
#ifdef VERSION
  p->version = TOSTRING(VERSION);
#endif
  p->website = "https://github.com/s-ol/vcvmods";
  p->manual = "https://github.com/s-ol/vcvmods";

  p->addModel(createModel<CircleVCOWidget>("s-ol", "CircleVCO", "Circle VCO", OSCILLATOR_TAG, VISUAL_TAG));
  p->addModel(createModel<WrapCompWidget>("s-ol", "WrapComp", "Wrapping Comparator", LOGIC_TAG));
  p->addModel(createModel<ModuloWidget>("s-ol", "Modulo", "Modulo", LOGIC_TAG));
}
