/* Copyright 2017 The TensorFlow Authors. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
==============================================================================*/
#ifndef TENSORFLOW_CONTRIB_LITE_EXPERIMENTAL_RISCV_KERNELS_OPTIMIZED_INTRINSIC_RISCV_ML_EXTENSION_H_
#define TENSORFLOW_CONTRIB_LITE_EXPERIMENTAL_RISCV_KERNELS_OPTIMIZED_INTRINSIC_RISCV_ML_EXTENSION_H_

enum vconfig {
  kElementWidthMax8 = 0x120,
  kElementWidthMax16 = 0x220,
  kElementWidthMax32 = 0x320,
  kElementWidthMax64 = 0x420
};  // element width

enum maxvlen {
  kMaxVectorLength8 = 32,
  kMaxVectorLength16 = 16,
  kMaxVectorLength32 = 8,
  kMaxVectorLength64 = 4
};

inline void SetVcfg(unsigned int config) {
  asm("csrw vcfg, %0\t\n" : : "r"(config));
}

inline void SetVl(unsigned int len) { asm("csrw vl, %0\t\n" : : "r"(len)); }

inline void SetConfig(unsigned int maxew, unsigned int maxvl) {
  asm("csrw vcfg, %0 \t\n"
      "csrw vl, %1 \t\n"
      :
      : "r"(maxew), "r"(maxvl));
}

inline void __VectorLoad(const float* load_address1,
                         const float* load_address2) {
  asm volatile(
      "vlsd va1, 0(%0), v \t\n"
      "vlsd va2, 0(%1), v \t\n"
      :
      : "r"(load_address1), "r"(load_address2));
}

inline void __VectorLoadInput1(const float* load_address) {
  asm volatile("vlsd va1, 0(%0), v \t\n" : : "r"(load_address));
}

inline void __VectorLoadInput2(const float* load_address) {
  asm volatile("vlsd va2, 0(%0), v \t\n" : : "r"(load_address));
}

inline void __VectorLoadInput1(const float* load_address, int stride) {
  asm volatile("vlsd va1, 0(%0), %1, v \t\n"
               :
               : "r"(load_address), "r"(stride));
}

inline void __VectorLoadActivationInput(float* load_address) {
  asm volatile("vlsd vt11, 0(%0), v\t\n"
               :
               : "r"(load_address));
}


inline void __VectorLoadActivationInput(float* load_address, int stride) {
  asm volatile("vlsd vt11, 0(%0), %1, v \t\n"
               :
               : "r"(load_address), "r"(stride));
}

inline void __VectorLoadInput2(const float* load_address, int stride) {
  asm volatile("vlsd va2, 0(%0), %1, v \t\n"
               :
               : "r"(load_address), "r"(stride));
}

inline void __VectorLoadBias(const float* load_address) {
  asm volatile("vlsd vt4, 0(%0), v \t\n" : : "r"(load_address));
}

inline void __VectorLoadBias(const float* load_address, int stride) {
  asm volatile("vlsd vt4, 0(%0), %1, v \t\n"
               :
               : "r"(load_address), "r"(stride));
}

inline void __VectorLoadPartialOutput(float* load_address) {
  asm volatile("vlsd vt4, 0(%0), v \t\n" : : "r"(load_address));
}

inline void __VectorLoad(const float* load_address1, const float* load_address2,
                         int stride) {
  asm volatile(
      "vlsd va1, 0(%0), v \t\n"
      "vlsd va2, 0(%1), %2, v \t\n"
      :
      : "r"(load_address1), "r"(load_address2), "r"(stride));
}

inline void __VectorLoadPartialOutput(float* load_address, int stride) {
  asm volatile("vlsd vt4, 0(%0), %1, v \t\n"
               :
               : "r"(load_address), "r"(stride));
}

inline void __VectorBroadcastAccum(float accum) {
  asm volatile("vbcastx vt4, %0 \t\n" : : "r"(accum));
}

inline void __VectorBroadcastInput(int input_index) {
  asm volatile("vsplat va1, vt2, %0 \t\n" : : "r"(input_index));
}

inline void __VectorBroadcastMinMax(float value_minimum, float value_maximum) {
  asm volatile(
      "vbcastx vt2, %0 \t\n"
      "vbcastx vt3, %1 \t\n"
      :
      : "r"(value_minimum), "r"(value_maximum));
}

inline void __VectorReduceAccumFloat() {
  asm volatile("vfredsum vt11, vt4 \t\n");
}

inline void __VectorAddFloat() { asm volatile("vfadd vt11, va1, va2, v \t\n"); }

inline void __VectorMulFloat() { asm volatile("vfmul vt11, va1, va2, v \t\n"); }

inline void __VectorMulAccFloat() {
  asm volatile("vfmadd vt4, va1, va2, vt4, v \t\n");
}

inline void __VectorAccFloat() { asm volatile("vfadd vt4, va1, vt4, v \t\n"); }

inline void __VectorMinMaxFloat() {
  asm volatile(
      "vfmax vt11, vt11, vt2 \t\n"
      "vfmin vt11, vt11, vt3 \t\n");
}

inline void __VectorMergeFloat() {
  asm volatile("vmerge vt11, vt11, vt10, t \t\n");
}
inline void __VectorSetMask(unsigned int idx, unsigned int val) {
  asm volatile("vinsx vt1, %1, %0, v \t\n" : : "r"(idx), "r"(val));
}

inline void __VectorResetMask(unsigned int idx) {
  asm volatile("vinsx vt1, zero, %0, v \t\n" : : "r"(idx));
}

inline void __VectorResetMaskAll() { asm volatile("vbcastx vt1, zero \t\n"); }

inline void __VectorStore(float* store_address) {
  asm volatile("vssd vt11, 0(%0), v \t\n" : : "r"(store_address));
}

inline void __VectorStorePartialOutput(float* store_address, int stride) {
  asm volatile("vssd vt4, 0(%0), %1, v \t\n"
               :
               : "r"(store_address), "r"(stride));
}

inline void __VectorStorePartialOutput(float* store_address) {
  asm volatile("vssd vt4, 0(%0), v \t\n" : : "r"(store_address));
}

inline void __VectorStoreAccum(float* store_address) {
  asm volatile("vssd vt11, 0(%0), s \t\n" : : "r"(store_address));
}

void VectorVectorAdd(const float* input1, const float* input2, float* output,
                     int len, int batch_size = 1);

void VectorVectorAddMinMax(const float* input1, const float* input2,
                           float output_min, float output_max, float* output,
                           int len, int batch_size = 1);

void VectorVectorMultiply(const float* input1, const float* input2,
                          float* output, int len, int batch_size = 1);

void VectorVectorMultiplyAccumulate(const float* input1, const float* input2,
                                    float* output, int len, int batch_size = 1);

void VectorMatrixMultiplyAccumulate(const float* vector, const float* matrix,
                                    float* output, int matrix_rows,
                                    int matrix_cols, int batch_size = 1);

void VectorVectorMultiplyAccumulateDepthwise(const float* input1,
                                             const float* input2, float* output,
                                             int depth, int depth_multiplier,
                                             const float* bias, bool use_bias);

void VectorAveragePooling(const float* input, float* output, int depth,
                          bool use_zero);

void VectorActivationFunctionWithMinMax(float* data, float activation_min, float activation_max, int length);
#endif  // TENSORFLOW_CONTRIB_LITE_EXPERIMENTAL_RISCV_KERNELS_OPTIMIZED_INTRINSIC_RISCV_ML_EXTENSION_H_