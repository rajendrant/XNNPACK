// Auto-generated file. Do not edit!
//   Template: src/f32-vbinary/vopc-hvx.c.in
//   Generator: tools/xngen
//

#include <assert.h>

#include <hvx_hexagon_protos.h>
#include <hexagon_protos.h>
#include <hexagon_types.h>

#include <xnnpack/common.h>
#include <xnnpack/intrinsics-polyfill.h>
#include <xnnpack/math.h>
#include <xnnpack/vbinary.h>

void xnn_f32_vmulc_minmax_ukernel__hvx_u128(
    size_t batch,
    const float* input_a,
    const float* input_b,
    float* output,
    const union xnn_f32_minmax_params params[restrict XNN_MIN_ELEMENTS(1)]) XNN_OOB_READS
{
  assert(batch != 0);
  assert(batch % sizeof(float) == 0);
  assert(input_a != NULL);
  assert(input_b != NULL);
  assert(output != NULL);

  const HVX_Vector voutput_min = Q6_V_vsplat_R(*((uint32_t *) &params->scalar.min));
  const HVX_Vector voutput_max = Q6_V_vsplat_R(*((uint32_t *) &params->scalar.max));
  HVX_Vector vb = Q6_V_vsplat_R(*((int32_t*) input_b));

  for (; batch >= 128 * sizeof(float); batch -= 128 * sizeof(float)) {
    HVX_Vector va0 = *((HVX_UVector*) input_a);
    HVX_Vector va1 = *((HVX_UVector*)(input_a + 32));
    HVX_Vector va2 = *((HVX_UVector*)(input_a + 64));
    HVX_Vector va3 = *((HVX_UVector*)(input_a + 96));
    input_a += 128;

    HVX_Vector vacc0 = Q6_Vsf_vmpy_VsfVsf(va0, vb);
    HVX_Vector vacc1 = Q6_Vsf_vmpy_VsfVsf(va1, vb);
    HVX_Vector vacc2 = Q6_Vsf_vmpy_VsfVsf(va2, vb);
    HVX_Vector vacc3 = Q6_Vsf_vmpy_VsfVsf(va3, vb);


    vacc0 = Q6_Vsf_vmax_VsfVsf(vacc0, voutput_min);
    vacc1 = Q6_Vsf_vmax_VsfVsf(vacc1, voutput_min);
    vacc2 = Q6_Vsf_vmax_VsfVsf(vacc2, voutput_min);
    vacc3 = Q6_Vsf_vmax_VsfVsf(vacc3, voutput_min);

    vacc0 = Q6_Vsf_vmin_VsfVsf(vacc0, voutput_max);
    vacc1 = Q6_Vsf_vmin_VsfVsf(vacc1, voutput_max);
    vacc2 = Q6_Vsf_vmin_VsfVsf(vacc2, voutput_max);
    vacc3 = Q6_Vsf_vmin_VsfVsf(vacc3, voutput_max);

    *((HVX_UVector *) output) = vacc0;
    *((HVX_UVector *)(output + 32)) = vacc1;
    *((HVX_UVector *)(output + 64)) = vacc2;
    *((HVX_UVector *)(output + 96)) = vacc3;
    output += 128;
  }
  for (; batch >= 32 * sizeof(float); batch -= 32 * sizeof(float)) {
    HVX_Vector va = *((HVX_UVector*) input_a);
    input_a += 32;

    HVX_Vector vacc = Q6_Vsf_vmpy_VsfVsf(va, vb);
    vacc = Q6_Vsf_vmax_VsfVsf(vacc, voutput_min);
    vacc = Q6_Vsf_vmin_VsfVsf(vacc, voutput_max);

    *((HVX_UVector *) output) = vacc;
    output+= 32;
  }
  if XNN_UNLIKELY(batch != 0) {
    HVX_Vector va = *((HVX_UVector*) input_a);

    HVX_Vector vacc = Q6_Vsf_vmpy_VsfVsf(va, vb);
    vacc = Q6_Vsf_vmax_VsfVsf(vacc, voutput_min);
    vacc = Q6_Vsf_vmin_VsfVsf(vacc, voutput_max);

    Q6_V_vstu_variable(output, batch, vacc);
  }
}
