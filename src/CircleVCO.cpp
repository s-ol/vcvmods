#include "s-ol.hpp"

struct CircleVCO : Module {
  enum ParamIds {
    PITCH_PARAM,
    NUM_PARAMS
  };
  enum InputIds {
    PITCH_INPUT,
    NUM_INPUTS
  };
  enum OutputIds {
    SIN_OUTPUT,
    COS_OUTPUT,
    PHS_OUTPUT,
    NUM_OUTPUTS
  };
  enum LightIds {
    NUM_LIGHTS
  };

  float phase = 0.0;

  CircleVCO() : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS) {}
  void step() override;
};


void CircleVCO::step() {
  float deltaTime = 1.0 / engineGetSampleRate();

  float pitch = params[PITCH_PARAM].value;
  pitch += inputs[PITCH_INPUT].value;
  pitch = clampf(pitch, -4.0, 4.0);
  float freq = 200 * (powf(2.0, pitch) - powf(2.0, -4.0));

  phase += freq * deltaTime;
  while (phase >= 1.0)
    phase -= 1.0;

  float sine = sinf(2 * M_PI * phase);
  float cosn = cosf(2 * M_PI * phase);
  outputs[SIN_OUTPUT].value = sine * 5;
  outputs[COS_OUTPUT].value = cosn * 5;
  outputs[PHS_OUTPUT].value = phase * 5;
}


CircleVCOWidget::CircleVCOWidget() {
  CircleVCO *module = new CircleVCO();
  setModule(module);
  setPanel(SVG::load(assetPlugin(plugin, "res/CircleVCO.svg")));

  addChild(createScrew<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
  addChild(createScrew<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

  PJ301MPort p;
  RoundSmallBlackKnob k;
  Vec center = Vec(box.size.x, 0).minus(p.box.size).div(2);
  Vec kcenter = Vec(box.size.x, 0).minus(k.box.size).div(2);

  addParam(createParam<RoundSmallBlackKnob>(kcenter.plus(Vec(0, 90)), module, CircleVCO::PITCH_PARAM, 0.0, 1.0, 0.0));

  addInput(createInput<PJ301MPort>(center.plus(Vec(0, 144)), module, CircleVCO::PITCH_INPUT));

  addOutput(createOutput<PJ301MPort>(center.plus(Vec(0, 218)), module, CircleVCO::SIN_OUTPUT));
  addOutput(createOutput<PJ301MPort>(center.plus(Vec(0, 268)), module, CircleVCO::COS_OUTPUT));
  addOutput(createOutput<PJ301MPort>(center.plus(Vec(0, 318)), module, CircleVCO::PHS_OUTPUT));
}
