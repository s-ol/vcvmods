#include "s-ol.hpp"

struct WrapComp : Module {
  enum ParamIds {
    A_OUT_PARAM,
    B_OUT_PARAM,
    OFFSET_PARAM,
    RANGE_PARAM,
    NUM_PARAMS
  };
  enum InputIds {
    SIGNAL_INPUT,
    OFFSET_INPUT,
    RANGE_INPUT,
    NUM_INPUTS
  };
  enum OutputIds {
    MAIN_OUTPUT,
    NUM_OUTPUTS
  };
  enum LightIds {
    A_LIGHT,
    B_LIGHT,
    NUM_LIGHTS
  };

  WrapComp() {
    config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
    configParam(OFFSET_PARAM, -5.0, 5.0, 0.0);
    configParam(RANGE_PARAM, -5.0, 5.0, 0.0);
    configParam(A_OUT_PARAM, -8.0, 8.0, 1.0);
    configParam(B_OUT_PARAM, -8.0, 8.0, 0.0);
  }
  void process(const ProcessArgs &args) override;
};


void WrapComp::process(const ProcessArgs &args) {
  float input = inputs[SIGNAL_INPUT].getVoltage() + inputs[OFFSET_INPUT].getNormalVoltage(0.0) + params[OFFSET_PARAM].getValue();
  float width = clamp((inputs[RANGE_INPUT].getNormalVoltage(0.0f) + params[RANGE_PARAM].getValue()) / 2.0f, 0.0f, 5.0f);

  while (input > 5.0)
    input -= 5.0;
  while (input < 0.0)
    input += 5.0;

  bool a = input < width || input > (5.0 - width);

  outputs[MAIN_OUTPUT].setVoltage(a ? params[A_OUT_PARAM].getValue() : params[B_OUT_PARAM].getValue());
  lights[A_LIGHT].value = a ? 1.0 : 0.0;
  lights[B_LIGHT].value = a ? 0.0 : 1.0;
}


struct WrapCompWidget : ModuleWidget {
  WrapCompWidget(WrapComp *module);
};

WrapCompWidget::WrapCompWidget(WrapComp *module) {
  setModule(module);
  setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/WrapComp.svg")));

  addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
  addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
  addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
  addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

  PJ301MPort p;
  RoundSmallBlackKnob k;
  MediumLight<RedLight> l;
  Vec center = Vec(box.size.x, 0).minus(p.box.size).div(2);
  Vec kcenter = Vec(box.size.x, 0).minus(k.box.size).div(2);
  Vec lcenter = Vec(box.size.x, 0).minus(l.box.size).div(2);

  addInput(createInput<PJ301MPort>(center.plus(Vec(0, 70)), module, WrapComp::SIGNAL_INPUT));

  addParam(createParam<RoundSmallBlackKnob>(kcenter.plus(Vec(25, 135)), module, WrapComp::OFFSET_PARAM));
  addInput(createInput<PJ301MPort>(center.plus(Vec(-25, 135)), module, WrapComp::OFFSET_INPUT));

  addParam(createParam<RoundSmallBlackKnob>(kcenter.plus(Vec(25, 210)), module, WrapComp::RANGE_PARAM));
  addInput(createInput<PJ301MPort>(center.plus(Vec(-25, 210)), module, WrapComp::RANGE_INPUT));

  addChild(createLight<MediumLight<RedLight>>(lcenter.plus(Vec(-15, 255)), module, WrapComp::A_LIGHT));
  addChild(createLight<MediumLight<RedLight>>(lcenter.plus(Vec( 15, 255)), module, WrapComp::B_LIGHT));

  addParam(createParam<RoundSmallBlackKnob>(kcenter.plus(Vec(-25, 280)), module, WrapComp::A_OUT_PARAM));
  addParam(createParam<RoundSmallBlackKnob>(kcenter.plus(Vec( 25, 280)), module, WrapComp::B_OUT_PARAM));

  addOutput(createOutput<PJ301MPort>(center.plus(Vec(0, 315)), module, WrapComp::MAIN_OUTPUT));
}

Model *modelWrapComp = createModel<WrapComp, WrapCompWidget>("WrapComp");
