#include "plugin.hpp"

struct Modulo : Module {
  enum ParamIds {
    GAIN_PARAM,
    NUM_PARAMS
  };
  enum InputIds {
    SIGNAL_INPUT,
    GAIN_INPUT,
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

  Modulo() {
    config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
    configParam(GAIN_PARAM, -3.0, 3.0, 0.0);
    configParam(GAIN_PARAM, -3.0f, 3.0f, 0.0f, "Gain", "%", 2.0f, 100.0f);
  }
  void process(const ProcessArgs &args) override;
};


void Modulo::process(const ProcessArgs &args) {
  float gain = powf(2.0, params[GAIN_PARAM].getValue()) + inputs[GAIN_INPUT].getVoltage();
  float val = inputs[SIGNAL_INPUT].getVoltage() * gain;
  float stair = floorf(val);
  float wrap = val - stair;

  outputs[STAIR_OUTPUT].setVoltage(stair / gain);;
  outputs[WRAP_OUTPUT].setVoltage(wrap * 5);
}


struct ModuloWidget : ModuleWidget {
  ModuloWidget(Modulo *module);
};

ModuloWidget::ModuloWidget(Modulo *module) {
  setModule(module);
  setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/Modulo.svg")));

  addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
  addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

  PJ301MPort p;
  RoundSmallBlackKnob k;
  Vec center = Vec(box.size.x, 0).minus(p.box.size).div(2);
  Vec kcenter = Vec(box.size.x, 0).minus(k.box.size).div(2);


  addInput(createInput<PJ301MPort>(center.plus(Vec(0, 120)), module, Modulo::SIGNAL_INPUT));
  addInput(createInput<PJ301MPort>(center.plus(Vec(-15, 180)), module, Modulo::GAIN_INPUT));

  addParam(createParam<RoundSmallBlackKnob>(kcenter.plus(Vec(15, 180)), module, Modulo::GAIN_PARAM));

  addOutput(createOutput<PJ301MPort>(center.plus(Vec(0, 265)), module, Modulo::STAIR_OUTPUT));
  addOutput(createOutput<PJ301MPort>(center.plus(Vec(0, 345)), module, Modulo::WRAP_OUTPUT));
}

Model *modelModulo = createModel<Modulo, ModuloWidget>("Modulo");
