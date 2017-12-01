#include "s-ol.hpp"

struct Modulo : Module {
  enum ParamIds {
    GAIN_PARAM,
    NUM_PARAMS
  };
  enum InputIds {
    SIGNAL_INPUT,
    NUM_INPUTS
  };
  enum OutputIds {
    STAIR_OUTPUT,
    WRAP_OUTPUT,
    NUM_OUTPUTS
  };
  enum LightIds {
    NUM_LIGHTS
  };

  Modulo() : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS) {}
  void step() override;
};


void Modulo::step() {
  float gain = powf(2.0, params[GAIN_PARAM].value);
  float val = inputs[SIGNAL_INPUT].value * gain;
  float stair = floorf(val);
  float wrap = val - stair;

  outputs[STAIR_OUTPUT].value = stair / gain;;
  outputs[WRAP_OUTPUT].value = wrap * 5;
}


ModuloWidget::ModuloWidget() {
  Modulo *module = new Modulo();
  setModule(module);
  setPanel(SVG::load(assetPlugin(plugin, "res/Modulo.svg")));

  addChild(createScrew<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
  addChild(createScrew<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

  PJ301MPort p;
  Davies1900hBlackKnob k;
  Vec center = Vec(box.size.x, 0).minus(p.box.size).div(2);
  Vec kcenter = Vec(box.size.x, 0).minus(k.box.size).div(2);

  addParam(createParam<Davies1900hBlackKnob>(kcenter.plus(Vec(0, 90)), module, Modulo::GAIN_PARAM, -3.0, 3.0, 0.0));

  addInput(createInput<PJ301MPort>(center.plus(Vec(0, 145)), module, Modulo::SIGNAL_INPUT));

  addOutput(createOutput<PJ301MPort>(center.plus(Vec(0, 275)), module, Modulo::STAIR_OUTPUT));
  addOutput(createOutput<PJ301MPort>(center.plus(Vec(0, 347)), module, Modulo::WRAP_OUTPUT));
}
