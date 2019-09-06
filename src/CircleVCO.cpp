#include "plugin.hpp"

struct CircleVCO : Module {
  enum ParamIds {
    FREQ_PARAM,
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
    configParam(FREQ_PARAM, -54.0f, 54.0f, 0.0f, "Frequency", " Hz", dsp::FREQ_SEMITONE, dsp::FREQ_C4);
  }
  void process(const ProcessArgs &args) override;
};


void CircleVCO::process(const ProcessArgs &args) {
  float deltaTime = 1.0f / args.sampleRate;

  float pitch = params[FREQ_PARAM].getValue() / 12.f;
  pitch += inputs[PITCH_INPUT].getVoltage();
  float freq = dsp::FREQ_C4 * powf(2.f, pitch);

  phase += freq * deltaTime;
  while (phase >= 1.0f)
    phase -= 1.0f;

  float sine = sin(2.0f * M_PI * phase);
  float cosn = cos(2.0f * M_PI * phase);
  outputs[SIN_OUTPUT].setVoltage(sine * 5.0f);
  outputs[COS_OUTPUT].setVoltage(cosn * 5.0f);
  outputs[PHS_OUTPUT].setVoltage(phase * 5.0f);
}


struct CircleVCOWidget : ModuleWidget {
  CircleVCOWidget(CircleVCO *module);
};

CircleVCOWidget::CircleVCOWidget(CircleVCO *module) {
  setModule(module);
  setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/CircleVCO.svg")));

  addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
  addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

  PJ301MPort p;
  RoundSmallBlackKnob k;
  Vec center = Vec(box.size.x, 0).minus(p.box.size).div(2);
  Vec kcenter = Vec(box.size.x, 0).minus(k.box.size).div(2);

  addParam(createParam<RoundSmallBlackKnob>(kcenter.plus(Vec(0, 90)), module, CircleVCO::FREQ_PARAM));

  addInput(createInput<PJ301MPort>(center.plus(Vec(0, 144)), module, CircleVCO::PITCH_INPUT));

  addOutput(createOutput<PJ301MPort>(center.plus(Vec(0, 218)), module, CircleVCO::SIN_OUTPUT));
  addOutput(createOutput<PJ301MPort>(center.plus(Vec(0, 268)), module, CircleVCO::COS_OUTPUT));
  addOutput(createOutput<PJ301MPort>(center.plus(Vec(0, 318)), module, CircleVCO::PHS_OUTPUT));
}

Model *modelCircleVCO = createModel<CircleVCO, CircleVCOWidget>("CircleVCO");
