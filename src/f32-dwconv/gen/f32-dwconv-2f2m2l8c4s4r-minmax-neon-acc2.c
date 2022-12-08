// Auto-generated file. Do not edit!
//   Template: src/f32-dwconv/multipass-neon.c.in
//   Generator: tools/xngen
//
// Copyright 2022 Google LLC
//
// This source code is licensed under the BSD-style license found in the
// LICENSE file in the root directory of this source tree.

#include <assert.h>
#include <stddef.h>
#include <stdint.h>

#include <arm_neon.h>

#include <xnnpack/dwconv.h>
#include <xnnpack/math.h>


void xnn_f32_dwconv_minmax_ukernel_2f2m2l8c4s4r__neon_acc2(
    size_t channels,
    size_t output_width,
    const float** input,
    const float* weights,
    float* output,
    intptr_t input_stride,
    size_t output_increment,
    size_t input_offset,
    const float* zero,
    size_t kernel_size,
    float* buffer,
    const union xnn_f32_minmax_params params[restrict XNN_MIN_ELEMENTS(1)]) XNN_OOB_READS
{
  assert(channels != 0);
  assert(output_width != 0);
  assert(kernel_size > 2);

  const float32x4_t vmax = vld1q_dup_f32(&params->scalar.max);
  const float32x4_t vmin = vld1q_dup_f32(&params->scalar.min);
  do {
    const float* w = weights;

    // First pass to process 2 inputs.
    {
      float* b = buffer;
      const float* i0 = input[0];
      assert(i0 != NULL);
      if XNN_UNPREDICTABLE(i0 != zero) {
        i0 = (const float*) ((uintptr_t) i0 + input_offset);
      }
      const float* i1 = input[1];
      assert(i1 != NULL);
      if XNN_UNPREDICTABLE(i1 != zero) {
        i1 = (const float*) ((uintptr_t) i1 + input_offset);
      }
      input += 2;

      // Process c channels and write to buffer.
      size_t c = round_up_po2(channels, 4);
      for (; c >= 8; c -= 8) {
        float32x4_t vacc0123p0 = vld1q_f32(w); w += 4;
        float32x4_t vacc4567p0 = vld1q_f32(w); w += 4;


        const float32x4_t vi0x0123 = vld1q_f32(i0); i0 += 4;
        const float32x4_t vi0x4567 = vld1q_f32(i0); i0 += 4;

        const float32x4_t vk0x0123 = vld1q_f32(w); w += 4;
        const float32x4_t vk0x4567 = vld1q_f32(w); w += 4;
        vacc0123p0 = vmlaq_f32(vacc0123p0, vi0x0123, vk0x0123);
        vacc4567p0 = vmlaq_f32(vacc4567p0, vi0x4567, vk0x4567);

        const float32x4_t vi1x0123 = vld1q_f32(i1); i1 += 4;
        const float32x4_t vi1x4567 = vld1q_f32(i1); i1 += 4;

        const float32x4_t vk1x0123 = vld1q_f32(w); w += 4;
        const float32x4_t vk1x4567 = vld1q_f32(w); w += 4;
        float32x4_t vacc0123p1 = vmulq_f32(vi1x0123, vk1x0123);
        float32x4_t vacc4567p1 = vmulq_f32(vi1x4567, vk1x4567);

        // Add up all accumulators to vacc0123p0
        vacc0123p0 = vaddq_f32(vacc0123p0, vacc0123p1);
        vacc4567p0 = vaddq_f32(vacc4567p0, vacc4567p1);

        vst1q_f32(b, vacc0123p0); b += 4;
        vst1q_f32(b, vacc4567p0); b += 4;
      }

      if (c != 0) {
        float32x4_t vacc0123p0 = vld1q_f32(w); w += 4;


        const float32x4_t vi0x0123 = vld1q_f32(i0); i0 += 4;

        const float32x4_t vk0x0123 = vld1q_f32(w); w += 4;
        vacc0123p0 = vmlaq_f32(vacc0123p0, vi0x0123, vk0x0123);

        const float32x4_t vi1x0123 = vld1q_f32(i1); i1 += 4;

        const float32x4_t vk1x0123 = vld1q_f32(w); w += 4;
        float32x4_t vacc0123p1 = vmulq_f32(vi1x0123, vk1x0123);

        // Add up all accumulators to vacc0123p0
        vacc0123p0 = vaddq_f32(vacc0123p0, vacc0123p1);

        vst1q_f32(b, vacc0123p0); b += 4;
      }
    }

    // Middle pass to process 2 inputs in each iteration.
    for (size_t ks = kernel_size - 2; ks > 2; ks -= 2) {
      float* b = buffer;
      const float* i0 = input[0];
      assert(i0 != NULL);
      if XNN_UNPREDICTABLE(i0 != zero) {
        i0 = (const float*) ((uintptr_t) i0 + input_offset);
      }
      const float* i1 = input[1];
      assert(i1 != NULL);
      if XNN_UNPREDICTABLE(i1 != zero) {
        i1 = (const float*) ((uintptr_t) i1 + input_offset);
      }
      input += 2;

      size_t c = round_up_po2(channels, 4);
      for (; c >= 8; c -= 8) {
        float32x4_t vacc0123p0 = vld1q_f32(b);
        float32x4_t vacc4567p0 = vld1q_f32(b + 4);


        const float32x4_t vi0x0123 = vld1q_f32(i0); i0 += 4;
        const float32x4_t vi0x4567 = vld1q_f32(i0); i0 += 4;

        const float32x4_t vk0x0123 = vld1q_f32(w); w += 4;
        const float32x4_t vk0x4567 = vld1q_f32(w); w += 4;
        vacc0123p0 = vmlaq_f32(vacc0123p0, vi0x0123, vk0x0123);
        vacc4567p0 = vmlaq_f32(vacc4567p0, vi0x4567, vk0x4567);

        const float32x4_t vi1x0123 = vld1q_f32(i1); i1 += 4;
        const float32x4_t vi1x4567 = vld1q_f32(i1); i1 += 4;

        const float32x4_t vk1x0123 = vld1q_f32(w); w += 4;
        const float32x4_t vk1x4567 = vld1q_f32(w); w += 4;
        float32x4_t vacc0123p1 = vmulq_f32(vi1x0123, vk1x0123);
        float32x4_t vacc4567p1 = vmulq_f32(vi1x4567, vk1x4567);

        // Add up all accumulators to vacc0123p0
        vacc0123p0 = vaddq_f32(vacc0123p0, vacc0123p1);
        vacc4567p0 = vaddq_f32(vacc4567p0, vacc4567p1);

        vst1q_f32(b, vacc0123p0); b += 4;
        vst1q_f32(b, vacc4567p0); b += 4;
      }

      if (c != 0) {
        float32x4_t vacc0123p0 = vld1q_f32(b);


        const float32x4_t vi0x0123 = vld1q_f32(i0); i0 += 4;

        const float32x4_t vk0x0123 = vld1q_f32(w); w += 4;
        vacc0123p0 = vmlaq_f32(vacc0123p0, vi0x0123, vk0x0123);

        const float32x4_t vi1x0123 = vld1q_f32(i1); i1 += 4;

        const float32x4_t vk1x0123 = vld1q_f32(w); w += 4;
        float32x4_t vacc0123p1 = vmulq_f32(vi1x0123, vk1x0123);

        // Add up all accumulators to vacc0123p0
        vacc0123p0 = vaddq_f32(vacc0123p0, vacc0123p1);

        vst1q_f32(b, vacc0123p0); b += 4;
      }
    }

    // Last pass to process up to 2 inputs.
    {
      float* b = buffer;
      const float* i0 = input[0];
      assert(i0 != NULL);
      if XNN_UNPREDICTABLE(i0 != zero) {
        i0 = (const float*) ((uintptr_t) i0 + input_offset);
      }
      const float* i1 = input[1];
      assert(i1 != NULL);
      if XNN_UNPREDICTABLE(i1 != zero) {
        i1 = (const float*) ((uintptr_t) i1 + input_offset);
      }

      size_t c = channels;
      for (; c >= 8; c -= 8) {
        float32x4_t vacc0123p0 = vld1q_f32(b); b += 4;
        float32x4_t vacc4567p0 = vld1q_f32(b); b += 4;


        const float32x4_t vi0x0123 = vld1q_f32(i0); i0 += 4;
        const float32x4_t vi0x4567 = vld1q_f32(i0); i0 += 4;

        float32x4_t vk0x0123 = vld1q_f32(w); w += 4;
        float32x4_t vk0x4567 = vld1q_f32(w); w += 4;

        vacc0123p0 = vmlaq_f32(vacc0123p0, vi0x0123, vk0x0123);
        vacc4567p0 = vmlaq_f32(vacc4567p0, vi0x4567, vk0x4567);

        const float32x4_t vi1x0123 = vld1q_f32(i1); i1 += 4;
        const float32x4_t vi1x4567 = vld1q_f32(i1); i1 += 4;

        float32x4_t vk1x0123 = vld1q_f32(w); w += 4;
        float32x4_t vk1x4567 = vld1q_f32(w); w += 4;

        float32x4_t vacc0123p1 = vmulq_f32(vi1x0123, vk1x0123);
        float32x4_t vacc4567p1 = vmulq_f32(vi1x4567, vk1x4567);

        // Add up all accumulators to vacc0123p0
        vacc0123p0 = vaddq_f32(vacc0123p0, vacc0123p1);
        vacc4567p0 = vaddq_f32(vacc4567p0, vacc4567p1);

        float32x4_t vacc0123 = vmaxq_f32(vacc0123p0, vmin);
        float32x4_t vacc4567 = vmaxq_f32(vacc4567p0, vmin);

        vacc0123 = vminq_f32(vacc0123, vmax);
        vacc4567 = vminq_f32(vacc4567, vmax);

        vst1q_f32(output, vacc0123); output += 4;
        vst1q_f32(output, vacc4567); output += 4;
      }


      for (; c >= 4; c -= 4) {
        float32x4_t vacc0123p0 = vld1q_f32(b); b += 4;


        const float32x4_t vi0x0123 = vld1q_f32(i0); i0 += 4;

        float32x4_t vk0x0123 = vld1q_f32(w); w += 4;

        vacc0123p0 = vmlaq_f32(vacc0123p0, vi0x0123, vk0x0123);

        const float32x4_t vi1x0123 = vld1q_f32(i1); i1 += 4;

        float32x4_t vk1x0123 = vld1q_f32(w); w += 4;

        float32x4_t vacc0123p1 = vmulq_f32(vi1x0123, vk1x0123);


        // Add up all accumulators to vacc0123p0
        vacc0123p0 = vaddq_f32(vacc0123p0, vacc0123p1);

        float32x4_t vacc0123 = vmaxq_f32(vacc0123p0, vmin);

        vacc0123 = vminq_f32(vacc0123, vmax);

        vst1q_f32(output, vacc0123); output += 4;
      }

      if XNN_UNLIKELY(c != 0) {
        float32x4_t vacc0123p0 = vld1q_f32(b);

        const float32x4_t vi0x0123 = vld1q_f32(i0);
        float32x4_t vk0x0123 = vld1q_f32(w); w += 4;
        vacc0123p0 = vmlaq_f32(vacc0123p0, vi0x0123, vk0x0123);

        const float32x4_t vi1x0123 = vld1q_f32(i1);
        float32x4_t vk1x0123 = vld1q_f32(w); w += 4;
        float32x4_t vacc0123p1 = vmulq_f32(vi1x0123, vk1x0123);

        // Add up all accumulators to vacc0123p0
        vacc0123p0 = vaddq_f32(vacc0123p0, vacc0123p1);

        float32x4_t vacc0123 = vmaxq_f32(vacc0123p0, vmin);
        vacc0123 = vminq_f32(vacc0123, vmax);

        float32x2_t vacc01 = vget_low_f32(vacc0123);
        if (c & 2) {
          vst1_f32(output, vacc01); output += 2;
          vacc01 = vget_high_f32(vacc0123);
        }
        if (c & 1) {
          vst1_lane_f32(output, vacc01, 0); output += 1;
        }
      }

    }
    input = (const float**) ((uintptr_t) input + input_stride);
    output = (float*) ((uintptr_t) output + output_increment);
  } while (--output_width != 0);
}