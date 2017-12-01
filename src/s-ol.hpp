#include "rack.hpp"


using namespace rack;


extern Plugin *plugin;

////////////////////
// module widgets
////////////////////

struct CircleVCOWidget : ModuleWidget {
  CircleVCOWidget();
};

struct WrapCompWidget : ModuleWidget {
  WrapCompWidget();
};

struct ModuloWidget : ModuleWidget {
  ModuloWidget();
};
