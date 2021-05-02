#include <stdbool.h>
#include "usermodfx.h"
#include "biquad.hpp"
#include "float_math.h"

#define LPF_Q_BASE  0.707f	// 1/√2
#define LPF_Q_TOP   4.f
#define LPF_FC_MIN  55.f	// Hz
#define LPF_FC_MAX  23500.f	// Hz

#define NumPro 2  // Number of Process: 2 (main and sub)
#define NumCh  2  // Number of Channel: 2 (stereo)
#define NumCas 2  // Number of Filter Cascade: 2 (4-pole)
static dsp::BiQuad yui_Filter	// define our BiQuad instances
[NumCh][NumCas] = {		// TODO: shoud able to change NumCh and NumCas
  { dsp::BiQuad(), dsp::BiQuad() },
  { dsp::BiQuad(), dsp::BiQuad() }
};
static float yui_LPF_fc = LPF_FC_MAX;
static float yui_LPF_q  = LPF_Q_BASE;
static const float fs_recipro = 1.f / 48000.f;	// recipro of sample rate
static int   clip_typ = 1;
static float clip_thr = 0.3f;
static bool  dirty_param;	// set when any param has been modified


// set new cutoff freq and resonance if any param has been updated
void update_coeffs() {
  if (! dirty_param) return;
  dirty_param = false;

  // update the BiQuad coefficients
  for (int ch = 0 ; ch < NumCh ; ch++) {
    for (int cas = 0 ; cas < NumCas ; cas++) {
      // set a BiQuad filter as SOLP with coefficients
      // for our new cuttoff freq and resonance
      yui_Filter[ch][cas].mCoeffs.setSOLP(
        fx_tanpif(yui_Filter[ch][cas].mCoeffs.wc(yui_LPF_fc, fs_recipro)),
	yui_LPF_q);
    }
  }
}


float yui_clip(float x) {
  switch (clip_typ) {
  case -1:
    return fx_sat_schetzenf(x);
  case 0:
    return fx_sat_cubicf(x);
  case 1:
    return fx_softclipf(clip_thr, x);
  default:
    return x;
  }
}

// the logue-SDK callback for initialization
void MODFX_INIT(uint32_t platform, uint32_t api) {
  for (int i = 0 ; i < NumCh ; i++) {
    for (int j = 0 ; j < NumCas ; j++) {
      yui_Filter[i][j].flush();
    }
  }

  dirty_param = true;
}


// the louge-SDK callback when this FX comes back from another FX (maybe)
// // re-init processing to avoid click noize when this FX comes back (maybe)
void DELFX_RESUME() {
  for (int i = 0 ; i < NumCh ; i++) {
    for (int j = 0 ; j < NumCas ; j++) {
      yui_Filter[i][j].flush();
    }
  }

  dirty_param = true;
}


// the logue-SDK callback to process a sample buffer
void MODFX_PROCESS(const float *main_xn, float *main_yn,
                   const float *sub_xn,  float *sub_yn,
                   uint32_t frames) {
  const float * mx = main_xn;
  float * __restrict my = main_yn;
  const float * my_e = my + NumCh*frames;

  // TODO: not only processing main frames but also sub frames
// const float *sx = sub_xn;
// float * __restrict sy = sub_yn;

  // check and set new params.
  // if a knob is tweaked while processing,
  // we will update at next PROCESS callback,
  update_coeffs();

  int ch = 0;
  for (; my != my_e; my++,mx++) {
    // N-time process for N-time -12dB/oct
    float buf = *mx;
    for (int cas = 0 ; cas < NumCas ; cas++) {
      // compute a step of BiQuad
      buf = yui_Filter[ch][cas].process_so(buf);
    }
    buf = yui_clip(buf);	// clip when high level amplitude
    *my = buf;
    ch = (++ch) % NumCh;
  }
}


void MODFX_PARAM(uint8_t index, int32_t value) {
  const float valf = q31_to_f32(value);
  switch (index) {

  case k_user_modfx_param_time:
    // set a new cutoff freq along a non-linear curve
    yui_LPF_fc = (LPF_FC_MAX - LPF_FC_MIN) * (fasterpowf(10.f,valf)-(1.f-valf))/10.f + LPF_FC_MIN;
    // limit of cutoff freq
    if (LPF_FC_MAX < yui_LPF_fc) { yui_LPF_fc = LPF_FC_MAX; }
    if (yui_LPF_fc < LPF_FC_MIN) { yui_LPF_fc = LPF_FC_MIN; }
    // we will update the filter coefficients when next PROCESS time
    dirty_param = true;
    break;

  case k_user_modfx_param_depth:
    // set a new resonance value
    yui_LPF_q = (LPF_Q_TOP - LPF_Q_BASE) * valf + LPF_Q_BASE;
    // we will update the filter coefficients when next PROCESS time
    dirty_param = true;
    break;

  default:
    break;
  }
}

