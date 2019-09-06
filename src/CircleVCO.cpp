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

  CircleVCO() {
    config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
  }
  void step() override;
};


void CircleVCO::step() {
  float deltaTime = 1.0f / engineGetSampleRate();

  float pitch = params[PITCH_PARAM].value;
  pitch += 12.0f * inputs[PITCH_INPUT].value;
  float freq = 261.626f * powf(2.0f, pitch / 12.0f);

  phase += freq * deltaTime;
  while (phase >= 1.0f)
    phase -= 1.0f;

  float sine = sin(2.0f * M_PI * phase);
  float cosn = cos(2.0f * M_PI * phase);
  outputs[SIN_OUTPUT].value = sine * 5.0f;
  outputs[COS_OUTPUT].value = cosn * 5.0f;
  outputs[PHS_OUTPUT].value = phase * 5.0f;
}


struct CircleVCOWidget : ModuleWidget {
  CircleVCOWidget(CircleVCO *module);
};

CircleVCOWidget::CircleVCOWidget(CircleVCO *module) {
  setModule(module);
  setPanel(SVG::load(assetPlugin(pluginInstance, "res/CircleVCO.svg")));

  addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
  addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

  PJ301MPort p;
  RoundSmallBlackKnob k;
  Vec center = Vec(box.size.x, 0).minus(p.box.size).div(2);
  Vec kcenter = Vec(box.size.x, 0).minus(k.box.size).div(2);

  addParam(createParam<RoundSmallBlackKnob>(kcenter.plus(Vec(0, 90)), module, CircleVCO::PITCH_PARAM, -54.0f, 54.0f, 0.0f));

  addInput(createPort<PJ301MPort>(center.plus(Vec(0, 144)), PortWidget::INPUT, module, CircleVCO::PITCH_INPUT));

  addOutput(createPort<PJ301MPort>(center.plus(Vec(0, 218)), PortWidget::OUTPUT, module, CircleVCO::SIN_OUTPUT));
  addOutput(createPort<PJ301MPort>(center.plus(Vec(0, 268)), PortWidget::OUTPUT, module, CircleVCO::COS_OUTPUT));
  addOutput(createPort<PJ301MPort>(center.plus(Vec(0, 318)), PortWidget::OUTPUT, module, CircleVCO::PHS_OUTPUT));
}

Model *modelCircleVCO = createModel<CircleVCO, CircleVCOWidget>("CircleVCO");
