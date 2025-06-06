// NOTE: Assertions have been autogenerated by utils/update_cc_test_checks.py UTC_ARGS: --version 2
// REQUIRES: riscv-registered-target
// RUN: %clang_cc1 -triple riscv64 -target-feature +v -target-feature +zfh \
// RUN:   -target-feature +zvfh -disable-O0-optnone  \
// RUN:   -emit-llvm %s -o - | opt -S -passes=mem2reg | \
// RUN:   FileCheck --check-prefix=CHECK-RV64 %s

#include <riscv_vector.h>

// CHECK-RV64-LABEL: define dso_local target("riscv.vector.tuple", <vscale x 8 x i8>, 7) @test_vlseg7e64ff_v_f64m1x7_tu
// CHECK-RV64-SAME: (target("riscv.vector.tuple", <vscale x 8 x i8>, 7) [[MASKEDOFF_TUPLE:%.*]], ptr noundef [[BASE:%.*]], ptr noundef [[NEW_VL:%.*]], i64 noundef [[VL:%.*]]) #[[ATTR0:[0-9]+]] {
// CHECK-RV64-NEXT:  entry:
// CHECK-RV64-NEXT:    [[TMP0:%.*]] = call { target("riscv.vector.tuple", <vscale x 8 x i8>, 7), i64 } @llvm.riscv.vlseg7ff.triscv.vector.tuple_nxv8i8_7t.i64.p0(target("riscv.vector.tuple", <vscale x 8 x i8>, 7) [[MASKEDOFF_TUPLE]], ptr [[BASE]], i64 [[VL]], i64 6)
// CHECK-RV64-NEXT:    [[TMP1:%.*]] = extractvalue { target("riscv.vector.tuple", <vscale x 8 x i8>, 7), i64 } [[TMP0]], 0
// CHECK-RV64-NEXT:    [[TMP2:%.*]] = extractvalue { target("riscv.vector.tuple", <vscale x 8 x i8>, 7), i64 } [[TMP0]], 1
// CHECK-RV64-NEXT:    store i64 [[TMP2]], ptr [[NEW_VL]], align 8
// CHECK-RV64-NEXT:    ret target("riscv.vector.tuple", <vscale x 8 x i8>, 7) [[TMP1]]
//
vfloat64m1x7_t test_vlseg7e64ff_v_f64m1x7_tu(vfloat64m1x7_t maskedoff_tuple, const double *base, size_t *new_vl, size_t vl) {
  return __riscv_vlseg7e64ff_tu(maskedoff_tuple, base, new_vl, vl);
}

// CHECK-RV64-LABEL: define dso_local target("riscv.vector.tuple", <vscale x 8 x i8>, 7) @test_vlseg7e64ff_v_i64m1x7_tu
// CHECK-RV64-SAME: (target("riscv.vector.tuple", <vscale x 8 x i8>, 7) [[MASKEDOFF_TUPLE:%.*]], ptr noundef [[BASE:%.*]], ptr noundef [[NEW_VL:%.*]], i64 noundef [[VL:%.*]]) #[[ATTR0]] {
// CHECK-RV64-NEXT:  entry:
// CHECK-RV64-NEXT:    [[TMP0:%.*]] = call { target("riscv.vector.tuple", <vscale x 8 x i8>, 7), i64 } @llvm.riscv.vlseg7ff.triscv.vector.tuple_nxv8i8_7t.i64.p0(target("riscv.vector.tuple", <vscale x 8 x i8>, 7) [[MASKEDOFF_TUPLE]], ptr [[BASE]], i64 [[VL]], i64 6)
// CHECK-RV64-NEXT:    [[TMP1:%.*]] = extractvalue { target("riscv.vector.tuple", <vscale x 8 x i8>, 7), i64 } [[TMP0]], 0
// CHECK-RV64-NEXT:    [[TMP2:%.*]] = extractvalue { target("riscv.vector.tuple", <vscale x 8 x i8>, 7), i64 } [[TMP0]], 1
// CHECK-RV64-NEXT:    store i64 [[TMP2]], ptr [[NEW_VL]], align 8
// CHECK-RV64-NEXT:    ret target("riscv.vector.tuple", <vscale x 8 x i8>, 7) [[TMP1]]
//
vint64m1x7_t test_vlseg7e64ff_v_i64m1x7_tu(vint64m1x7_t maskedoff_tuple, const int64_t *base, size_t *new_vl, size_t vl) {
  return __riscv_vlseg7e64ff_tu(maskedoff_tuple, base, new_vl, vl);
}

// CHECK-RV64-LABEL: define dso_local target("riscv.vector.tuple", <vscale x 8 x i8>, 7) @test_vlseg7e64ff_v_u64m1x7_tu
// CHECK-RV64-SAME: (target("riscv.vector.tuple", <vscale x 8 x i8>, 7) [[MASKEDOFF_TUPLE:%.*]], ptr noundef [[BASE:%.*]], ptr noundef [[NEW_VL:%.*]], i64 noundef [[VL:%.*]]) #[[ATTR0]] {
// CHECK-RV64-NEXT:  entry:
// CHECK-RV64-NEXT:    [[TMP0:%.*]] = call { target("riscv.vector.tuple", <vscale x 8 x i8>, 7), i64 } @llvm.riscv.vlseg7ff.triscv.vector.tuple_nxv8i8_7t.i64.p0(target("riscv.vector.tuple", <vscale x 8 x i8>, 7) [[MASKEDOFF_TUPLE]], ptr [[BASE]], i64 [[VL]], i64 6)
// CHECK-RV64-NEXT:    [[TMP1:%.*]] = extractvalue { target("riscv.vector.tuple", <vscale x 8 x i8>, 7), i64 } [[TMP0]], 0
// CHECK-RV64-NEXT:    [[TMP2:%.*]] = extractvalue { target("riscv.vector.tuple", <vscale x 8 x i8>, 7), i64 } [[TMP0]], 1
// CHECK-RV64-NEXT:    store i64 [[TMP2]], ptr [[NEW_VL]], align 8
// CHECK-RV64-NEXT:    ret target("riscv.vector.tuple", <vscale x 8 x i8>, 7) [[TMP1]]
//
vuint64m1x7_t test_vlseg7e64ff_v_u64m1x7_tu(vuint64m1x7_t maskedoff_tuple, const uint64_t *base, size_t *new_vl, size_t vl) {
  return __riscv_vlseg7e64ff_tu(maskedoff_tuple, base, new_vl, vl);
}

// CHECK-RV64-LABEL: define dso_local target("riscv.vector.tuple", <vscale x 8 x i8>, 7) @test_vlseg7e64ff_v_f64m1x7_tum
// CHECK-RV64-SAME: (<vscale x 1 x i1> [[MASK:%.*]], target("riscv.vector.tuple", <vscale x 8 x i8>, 7) [[MASKEDOFF_TUPLE:%.*]], ptr noundef [[BASE:%.*]], ptr noundef [[NEW_VL:%.*]], i64 noundef [[VL:%.*]]) #[[ATTR0]] {
// CHECK-RV64-NEXT:  entry:
// CHECK-RV64-NEXT:    [[TMP0:%.*]] = call { target("riscv.vector.tuple", <vscale x 8 x i8>, 7), i64 } @llvm.riscv.vlseg7ff.mask.triscv.vector.tuple_nxv8i8_7t.i64.p0.nxv1i1(target("riscv.vector.tuple", <vscale x 8 x i8>, 7) [[MASKEDOFF_TUPLE]], ptr [[BASE]], <vscale x 1 x i1> [[MASK]], i64 [[VL]], i64 2, i64 6)
// CHECK-RV64-NEXT:    [[TMP1:%.*]] = extractvalue { target("riscv.vector.tuple", <vscale x 8 x i8>, 7), i64 } [[TMP0]], 0
// CHECK-RV64-NEXT:    [[TMP2:%.*]] = extractvalue { target("riscv.vector.tuple", <vscale x 8 x i8>, 7), i64 } [[TMP0]], 1
// CHECK-RV64-NEXT:    store i64 [[TMP2]], ptr [[NEW_VL]], align 8
// CHECK-RV64-NEXT:    ret target("riscv.vector.tuple", <vscale x 8 x i8>, 7) [[TMP1]]
//
vfloat64m1x7_t test_vlseg7e64ff_v_f64m1x7_tum(vbool64_t mask, vfloat64m1x7_t maskedoff_tuple, const double *base, size_t *new_vl, size_t vl) {
  return __riscv_vlseg7e64ff_tum(mask, maskedoff_tuple, base, new_vl, vl);
}

// CHECK-RV64-LABEL: define dso_local target("riscv.vector.tuple", <vscale x 8 x i8>, 7) @test_vlseg7e64ff_v_i64m1x7_tum
// CHECK-RV64-SAME: (<vscale x 1 x i1> [[MASK:%.*]], target("riscv.vector.tuple", <vscale x 8 x i8>, 7) [[MASKEDOFF_TUPLE:%.*]], ptr noundef [[BASE:%.*]], ptr noundef [[NEW_VL:%.*]], i64 noundef [[VL:%.*]]) #[[ATTR0]] {
// CHECK-RV64-NEXT:  entry:
// CHECK-RV64-NEXT:    [[TMP0:%.*]] = call { target("riscv.vector.tuple", <vscale x 8 x i8>, 7), i64 } @llvm.riscv.vlseg7ff.mask.triscv.vector.tuple_nxv8i8_7t.i64.p0.nxv1i1(target("riscv.vector.tuple", <vscale x 8 x i8>, 7) [[MASKEDOFF_TUPLE]], ptr [[BASE]], <vscale x 1 x i1> [[MASK]], i64 [[VL]], i64 2, i64 6)
// CHECK-RV64-NEXT:    [[TMP1:%.*]] = extractvalue { target("riscv.vector.tuple", <vscale x 8 x i8>, 7), i64 } [[TMP0]], 0
// CHECK-RV64-NEXT:    [[TMP2:%.*]] = extractvalue { target("riscv.vector.tuple", <vscale x 8 x i8>, 7), i64 } [[TMP0]], 1
// CHECK-RV64-NEXT:    store i64 [[TMP2]], ptr [[NEW_VL]], align 8
// CHECK-RV64-NEXT:    ret target("riscv.vector.tuple", <vscale x 8 x i8>, 7) [[TMP1]]
//
vint64m1x7_t test_vlseg7e64ff_v_i64m1x7_tum(vbool64_t mask, vint64m1x7_t maskedoff_tuple, const int64_t *base, size_t *new_vl, size_t vl) {
  return __riscv_vlseg7e64ff_tum(mask, maskedoff_tuple, base, new_vl, vl);
}

// CHECK-RV64-LABEL: define dso_local target("riscv.vector.tuple", <vscale x 8 x i8>, 7) @test_vlseg7e64ff_v_u64m1x7_tum
// CHECK-RV64-SAME: (<vscale x 1 x i1> [[MASK:%.*]], target("riscv.vector.tuple", <vscale x 8 x i8>, 7) [[MASKEDOFF_TUPLE:%.*]], ptr noundef [[BASE:%.*]], ptr noundef [[NEW_VL:%.*]], i64 noundef [[VL:%.*]]) #[[ATTR0]] {
// CHECK-RV64-NEXT:  entry:
// CHECK-RV64-NEXT:    [[TMP0:%.*]] = call { target("riscv.vector.tuple", <vscale x 8 x i8>, 7), i64 } @llvm.riscv.vlseg7ff.mask.triscv.vector.tuple_nxv8i8_7t.i64.p0.nxv1i1(target("riscv.vector.tuple", <vscale x 8 x i8>, 7) [[MASKEDOFF_TUPLE]], ptr [[BASE]], <vscale x 1 x i1> [[MASK]], i64 [[VL]], i64 2, i64 6)
// CHECK-RV64-NEXT:    [[TMP1:%.*]] = extractvalue { target("riscv.vector.tuple", <vscale x 8 x i8>, 7), i64 } [[TMP0]], 0
// CHECK-RV64-NEXT:    [[TMP2:%.*]] = extractvalue { target("riscv.vector.tuple", <vscale x 8 x i8>, 7), i64 } [[TMP0]], 1
// CHECK-RV64-NEXT:    store i64 [[TMP2]], ptr [[NEW_VL]], align 8
// CHECK-RV64-NEXT:    ret target("riscv.vector.tuple", <vscale x 8 x i8>, 7) [[TMP1]]
//
vuint64m1x7_t test_vlseg7e64ff_v_u64m1x7_tum(vbool64_t mask, vuint64m1x7_t maskedoff_tuple, const uint64_t *base, size_t *new_vl, size_t vl) {
  return __riscv_vlseg7e64ff_tum(mask, maskedoff_tuple, base, new_vl, vl);
}

// CHECK-RV64-LABEL: define dso_local target("riscv.vector.tuple", <vscale x 8 x i8>, 7) @test_vlseg7e64ff_v_f64m1x7_tumu
// CHECK-RV64-SAME: (<vscale x 1 x i1> [[MASK:%.*]], target("riscv.vector.tuple", <vscale x 8 x i8>, 7) [[MASKEDOFF_TUPLE:%.*]], ptr noundef [[BASE:%.*]], ptr noundef [[NEW_VL:%.*]], i64 noundef [[VL:%.*]]) #[[ATTR0]] {
// CHECK-RV64-NEXT:  entry:
// CHECK-RV64-NEXT:    [[TMP0:%.*]] = call { target("riscv.vector.tuple", <vscale x 8 x i8>, 7), i64 } @llvm.riscv.vlseg7ff.mask.triscv.vector.tuple_nxv8i8_7t.i64.p0.nxv1i1(target("riscv.vector.tuple", <vscale x 8 x i8>, 7) [[MASKEDOFF_TUPLE]], ptr [[BASE]], <vscale x 1 x i1> [[MASK]], i64 [[VL]], i64 0, i64 6)
// CHECK-RV64-NEXT:    [[TMP1:%.*]] = extractvalue { target("riscv.vector.tuple", <vscale x 8 x i8>, 7), i64 } [[TMP0]], 0
// CHECK-RV64-NEXT:    [[TMP2:%.*]] = extractvalue { target("riscv.vector.tuple", <vscale x 8 x i8>, 7), i64 } [[TMP0]], 1
// CHECK-RV64-NEXT:    store i64 [[TMP2]], ptr [[NEW_VL]], align 8
// CHECK-RV64-NEXT:    ret target("riscv.vector.tuple", <vscale x 8 x i8>, 7) [[TMP1]]
//
vfloat64m1x7_t test_vlseg7e64ff_v_f64m1x7_tumu(vbool64_t mask, vfloat64m1x7_t maskedoff_tuple, const double *base, size_t *new_vl, size_t vl) {
  return __riscv_vlseg7e64ff_tumu(mask, maskedoff_tuple, base, new_vl, vl);
}

// CHECK-RV64-LABEL: define dso_local target("riscv.vector.tuple", <vscale x 8 x i8>, 7) @test_vlseg7e64ff_v_i64m1x7_tumu
// CHECK-RV64-SAME: (<vscale x 1 x i1> [[MASK:%.*]], target("riscv.vector.tuple", <vscale x 8 x i8>, 7) [[MASKEDOFF_TUPLE:%.*]], ptr noundef [[BASE:%.*]], ptr noundef [[NEW_VL:%.*]], i64 noundef [[VL:%.*]]) #[[ATTR0]] {
// CHECK-RV64-NEXT:  entry:
// CHECK-RV64-NEXT:    [[TMP0:%.*]] = call { target("riscv.vector.tuple", <vscale x 8 x i8>, 7), i64 } @llvm.riscv.vlseg7ff.mask.triscv.vector.tuple_nxv8i8_7t.i64.p0.nxv1i1(target("riscv.vector.tuple", <vscale x 8 x i8>, 7) [[MASKEDOFF_TUPLE]], ptr [[BASE]], <vscale x 1 x i1> [[MASK]], i64 [[VL]], i64 0, i64 6)
// CHECK-RV64-NEXT:    [[TMP1:%.*]] = extractvalue { target("riscv.vector.tuple", <vscale x 8 x i8>, 7), i64 } [[TMP0]], 0
// CHECK-RV64-NEXT:    [[TMP2:%.*]] = extractvalue { target("riscv.vector.tuple", <vscale x 8 x i8>, 7), i64 } [[TMP0]], 1
// CHECK-RV64-NEXT:    store i64 [[TMP2]], ptr [[NEW_VL]], align 8
// CHECK-RV64-NEXT:    ret target("riscv.vector.tuple", <vscale x 8 x i8>, 7) [[TMP1]]
//
vint64m1x7_t test_vlseg7e64ff_v_i64m1x7_tumu(vbool64_t mask, vint64m1x7_t maskedoff_tuple, const int64_t *base, size_t *new_vl, size_t vl) {
  return __riscv_vlseg7e64ff_tumu(mask, maskedoff_tuple, base, new_vl, vl);
}

// CHECK-RV64-LABEL: define dso_local target("riscv.vector.tuple", <vscale x 8 x i8>, 7) @test_vlseg7e64ff_v_u64m1x7_tumu
// CHECK-RV64-SAME: (<vscale x 1 x i1> [[MASK:%.*]], target("riscv.vector.tuple", <vscale x 8 x i8>, 7) [[MASKEDOFF_TUPLE:%.*]], ptr noundef [[BASE:%.*]], ptr noundef [[NEW_VL:%.*]], i64 noundef [[VL:%.*]]) #[[ATTR0]] {
// CHECK-RV64-NEXT:  entry:
// CHECK-RV64-NEXT:    [[TMP0:%.*]] = call { target("riscv.vector.tuple", <vscale x 8 x i8>, 7), i64 } @llvm.riscv.vlseg7ff.mask.triscv.vector.tuple_nxv8i8_7t.i64.p0.nxv1i1(target("riscv.vector.tuple", <vscale x 8 x i8>, 7) [[MASKEDOFF_TUPLE]], ptr [[BASE]], <vscale x 1 x i1> [[MASK]], i64 [[VL]], i64 0, i64 6)
// CHECK-RV64-NEXT:    [[TMP1:%.*]] = extractvalue { target("riscv.vector.tuple", <vscale x 8 x i8>, 7), i64 } [[TMP0]], 0
// CHECK-RV64-NEXT:    [[TMP2:%.*]] = extractvalue { target("riscv.vector.tuple", <vscale x 8 x i8>, 7), i64 } [[TMP0]], 1
// CHECK-RV64-NEXT:    store i64 [[TMP2]], ptr [[NEW_VL]], align 8
// CHECK-RV64-NEXT:    ret target("riscv.vector.tuple", <vscale x 8 x i8>, 7) [[TMP1]]
//
vuint64m1x7_t test_vlseg7e64ff_v_u64m1x7_tumu(vbool64_t mask, vuint64m1x7_t maskedoff_tuple, const uint64_t *base, size_t *new_vl, size_t vl) {
  return __riscv_vlseg7e64ff_tumu(mask, maskedoff_tuple, base, new_vl, vl);
}

// CHECK-RV64-LABEL: define dso_local target("riscv.vector.tuple", <vscale x 8 x i8>, 7) @test_vlseg7e64ff_v_f64m1x7_mu
// CHECK-RV64-SAME: (<vscale x 1 x i1> [[MASK:%.*]], target("riscv.vector.tuple", <vscale x 8 x i8>, 7) [[MASKEDOFF_TUPLE:%.*]], ptr noundef [[BASE:%.*]], ptr noundef [[NEW_VL:%.*]], i64 noundef [[VL:%.*]]) #[[ATTR0]] {
// CHECK-RV64-NEXT:  entry:
// CHECK-RV64-NEXT:    [[TMP0:%.*]] = call { target("riscv.vector.tuple", <vscale x 8 x i8>, 7), i64 } @llvm.riscv.vlseg7ff.mask.triscv.vector.tuple_nxv8i8_7t.i64.p0.nxv1i1(target("riscv.vector.tuple", <vscale x 8 x i8>, 7) [[MASKEDOFF_TUPLE]], ptr [[BASE]], <vscale x 1 x i1> [[MASK]], i64 [[VL]], i64 1, i64 6)
// CHECK-RV64-NEXT:    [[TMP1:%.*]] = extractvalue { target("riscv.vector.tuple", <vscale x 8 x i8>, 7), i64 } [[TMP0]], 0
// CHECK-RV64-NEXT:    [[TMP2:%.*]] = extractvalue { target("riscv.vector.tuple", <vscale x 8 x i8>, 7), i64 } [[TMP0]], 1
// CHECK-RV64-NEXT:    store i64 [[TMP2]], ptr [[NEW_VL]], align 8
// CHECK-RV64-NEXT:    ret target("riscv.vector.tuple", <vscale x 8 x i8>, 7) [[TMP1]]
//
vfloat64m1x7_t test_vlseg7e64ff_v_f64m1x7_mu(vbool64_t mask, vfloat64m1x7_t maskedoff_tuple, const double *base, size_t *new_vl, size_t vl) {
  return __riscv_vlseg7e64ff_mu(mask, maskedoff_tuple, base, new_vl, vl);
}

// CHECK-RV64-LABEL: define dso_local target("riscv.vector.tuple", <vscale x 8 x i8>, 7) @test_vlseg7e64ff_v_i64m1x7_mu
// CHECK-RV64-SAME: (<vscale x 1 x i1> [[MASK:%.*]], target("riscv.vector.tuple", <vscale x 8 x i8>, 7) [[MASKEDOFF_TUPLE:%.*]], ptr noundef [[BASE:%.*]], ptr noundef [[NEW_VL:%.*]], i64 noundef [[VL:%.*]]) #[[ATTR0]] {
// CHECK-RV64-NEXT:  entry:
// CHECK-RV64-NEXT:    [[TMP0:%.*]] = call { target("riscv.vector.tuple", <vscale x 8 x i8>, 7), i64 } @llvm.riscv.vlseg7ff.mask.triscv.vector.tuple_nxv8i8_7t.i64.p0.nxv1i1(target("riscv.vector.tuple", <vscale x 8 x i8>, 7) [[MASKEDOFF_TUPLE]], ptr [[BASE]], <vscale x 1 x i1> [[MASK]], i64 [[VL]], i64 1, i64 6)
// CHECK-RV64-NEXT:    [[TMP1:%.*]] = extractvalue { target("riscv.vector.tuple", <vscale x 8 x i8>, 7), i64 } [[TMP0]], 0
// CHECK-RV64-NEXT:    [[TMP2:%.*]] = extractvalue { target("riscv.vector.tuple", <vscale x 8 x i8>, 7), i64 } [[TMP0]], 1
// CHECK-RV64-NEXT:    store i64 [[TMP2]], ptr [[NEW_VL]], align 8
// CHECK-RV64-NEXT:    ret target("riscv.vector.tuple", <vscale x 8 x i8>, 7) [[TMP1]]
//
vint64m1x7_t test_vlseg7e64ff_v_i64m1x7_mu(vbool64_t mask, vint64m1x7_t maskedoff_tuple, const int64_t *base, size_t *new_vl, size_t vl) {
  return __riscv_vlseg7e64ff_mu(mask, maskedoff_tuple, base, new_vl, vl);
}

// CHECK-RV64-LABEL: define dso_local target("riscv.vector.tuple", <vscale x 8 x i8>, 7) @test_vlseg7e64ff_v_u64m1x7_mu
// CHECK-RV64-SAME: (<vscale x 1 x i1> [[MASK:%.*]], target("riscv.vector.tuple", <vscale x 8 x i8>, 7) [[MASKEDOFF_TUPLE:%.*]], ptr noundef [[BASE:%.*]], ptr noundef [[NEW_VL:%.*]], i64 noundef [[VL:%.*]]) #[[ATTR0]] {
// CHECK-RV64-NEXT:  entry:
// CHECK-RV64-NEXT:    [[TMP0:%.*]] = call { target("riscv.vector.tuple", <vscale x 8 x i8>, 7), i64 } @llvm.riscv.vlseg7ff.mask.triscv.vector.tuple_nxv8i8_7t.i64.p0.nxv1i1(target("riscv.vector.tuple", <vscale x 8 x i8>, 7) [[MASKEDOFF_TUPLE]], ptr [[BASE]], <vscale x 1 x i1> [[MASK]], i64 [[VL]], i64 1, i64 6)
// CHECK-RV64-NEXT:    [[TMP1:%.*]] = extractvalue { target("riscv.vector.tuple", <vscale x 8 x i8>, 7), i64 } [[TMP0]], 0
// CHECK-RV64-NEXT:    [[TMP2:%.*]] = extractvalue { target("riscv.vector.tuple", <vscale x 8 x i8>, 7), i64 } [[TMP0]], 1
// CHECK-RV64-NEXT:    store i64 [[TMP2]], ptr [[NEW_VL]], align 8
// CHECK-RV64-NEXT:    ret target("riscv.vector.tuple", <vscale x 8 x i8>, 7) [[TMP1]]
//
vuint64m1x7_t test_vlseg7e64ff_v_u64m1x7_mu(vbool64_t mask, vuint64m1x7_t maskedoff_tuple, const uint64_t *base, size_t *new_vl, size_t vl) {
  return __riscv_vlseg7e64ff_mu(mask, maskedoff_tuple, base, new_vl, vl);
}

