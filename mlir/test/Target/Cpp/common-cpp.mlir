// RUN: mlir-translate -mlir-to-cpp %s | FileCheck %s

// CHECK: #include "myheader.h"
emitc.include "myheader.h"
// CHECK: #include <myheader.h>
emitc.include <"myheader.h">

// CHECK: void test_include() {
func.func @test_include() {
  // CHECK: #include "myheader.h"
  emitc.include "myheader.h"
  return
}

// CHECK: void test_foo_print() {
func.func @test_foo_print() {
  // CHECK: [[V1:[^ ]*]] = foo::constant({0, 1});
  %0 = emitc.call_opaque "foo::constant"() {args = [dense<[0, 1]> : tensor<2xi32>]} : () -> (i32)
  // CHECK: [[V2:[^ ]*]] = foo::op_and_attr({0, 1}, [[V1]]);
  %1 = emitc.call_opaque "foo::op_and_attr"(%0) {args = [dense<[0, 1]> : tensor<2xi32>, 0 : index]} : (i32) -> (i32)
  // CHECK: [[V3:[^ ]*]] = foo::op_and_attr([[V2]], {0, 1});
  %2 = emitc.call_opaque "foo::op_and_attr"(%1) {args = [0 : index, dense<[0, 1]> : tensor<2xi32>]} : (i32) -> (i32)
  // CHECK: foo::print([[V3]]);
  emitc.call_opaque "foo::print"(%2): (i32) -> ()
  return
}

// CHECK: int32_t test_single_return(int32_t [[V2:.*]])
func.func @test_single_return(%arg0 : i32) -> i32 {
  // CHECK: return [[V2]]
  return %arg0 : i32
}

// CHECK: std::tuple<int32_t, int32_t> test_multiple_return()
func.func @test_multiple_return() -> (i32, i32) {
  // CHECK: std::tie([[V3:.*]], [[V4:.*]]) = foo::blah();
  %0:2 = emitc.call_opaque "foo::blah"() : () -> (i32, i32)
  // CHECK: [[V5:[^ ]*]] = test_single_return([[V3]]);
  %1 = call @test_single_return(%0#0) : (i32) -> i32
  // CHECK: return std::make_tuple([[V5]], [[V4]]);
  return %1, %0#1 : i32, i32
}

// CHECK: test_float
func.func @test_float() {
  // CHECK: foo::constant({0.0e+00f, 1.000000000e+00f})
  %0 = emitc.call_opaque "foo::constant"() {args = [dense<[0.000000e+00, 1.000000e+00]> : tensor<2xf32>]} : () -> f32
  return
}

// CHECK: test_uint
func.func @test_uint() {
  // CHECK: uint32_t
  %0 = emitc.call_opaque "foo::constant"() {args = [dense<[0, 1]> : tensor<2xui32>]} : () -> ui32
  // CHECK: uint64_t
  %1 = emitc.call_opaque "foo::constant"() {args = [dense<[0, 1]> : tensor<2xui64>]} : () -> ui64
  return
}

// CHECK: int64_t test_plus_int(int64_t [[V1]])
func.func @test_plus_int(%arg0 : i64) -> i64 {
  // CHECK: mhlo::add([[V1]], [[V1]])
  %0 = emitc.call_opaque "mhlo::add"(%arg0, %arg0) {args = [0 : index, 1 : index]} : (i64, i64) -> i64
  return %0 : i64
}

// CHECK: Tensor<float, 2> mixed_types(Tensor<double, 2> [[V1]])
func.func @mixed_types(%arg0: tensor<2xf64>) -> tensor<2xf32> {
  // CHECK: foo::mixed_types([[V1]]);
  %0 = emitc.call_opaque "foo::mixed_types"(%arg0) {args = [0 : index]} : (tensor<2xf64>) -> tensor<2xf32>
  return %0 : tensor<2xf32>
}

// CHECK: Tensor<uint64_t> mhlo_convert(Tensor<uint32_t> [[V1]])
func.func @mhlo_convert(%arg0: tensor<ui32>) -> tensor<ui64> {
  // CHECK: mhlo::convert([[V1]]);
  %0 = emitc.call_opaque "mhlo::convert"(%arg0) {args = [0 : index]} : (tensor<ui32>) -> tensor<ui64>
  return %0 : tensor<ui64>
}

// CHECK: status_t opaque_types(bool [[V1:[^ ]*]], char [[V2:[^ ]*]]) {
func.func @opaque_types(%arg0: !emitc.opaque<"bool">, %arg1: !emitc.opaque<"char">) -> !emitc.opaque<"status_t"> {
  // CHECK: int [[V3:[^ ]*]] = a([[V1]], [[V2]]);
  %0 = emitc.call_opaque "a"(%arg0, %arg1) : (!emitc.opaque<"bool">, !emitc.opaque<"char">) -> (!emitc.opaque<"int">)
  // CHECK: char [[V4:[^ ]*]] = b([[V3]]);
  %1 = emitc.call_opaque "b"(%0): (!emitc.opaque<"int">) -> (!emitc.opaque<"char">)
  // CHECK: status_t [[V5:[^ ]*]] = c([[V3]], [[V4]]);
  %2 = emitc.call_opaque "c"(%0, %1): (!emitc.opaque<"int">, !emitc.opaque<"char">) -> (!emitc.opaque<"status_t">)
  return %2 : !emitc.opaque<"status_t">
}

// CHECK-LABEL: int32_t* apply() {
func.func @apply() -> !emitc.ptr<i32> {
  // CHECK-NEXT: int32_t [[V1:[^ ]*]];
  %0 = "emitc.variable"() <{value = #emitc.opaque<"">}> : () -> !emitc.lvalue<i32>
  // CHECK-NEXT: int32_t* [[V2:[^ ]*]] = &[[V1]];
  %1 = emitc.apply "&"(%0) : (!emitc.lvalue<i32>) -> !emitc.ptr<i32>
  // CHECK-NEXT: int32_t [[V3:[^ ]*]];
  %2 = "emitc.variable"() {value = #emitc.opaque<"">} : () -> !emitc.lvalue<i32>
  // CHECK-NEXT: int32_t [[V4:[^ ]*]] = *[[V2]];
  %3 = emitc.apply "*"(%1) : (!emitc.ptr<i32>) -> i32
  // CHECK-NEXT: [[V3]] = [[V4]];
  emitc.assign %3 : i32 to %2 : !emitc.lvalue<i32>
  // CHECK-NEXT: return [[V2]];
  return %1 : !emitc.ptr<i32>
}

// CHECK: void array_type(int32_t v1[3], float v2[10][20])
func.func @array_type(%arg0: !emitc.array<3xi32>, %arg1: !emitc.array<10x20xf32>) {
  return
}

// CHECK: call_opaque_with_template_arg
func.func @call_opaque_with_template_arg() {
  emitc.call_opaque "init_tile"() {template_args = [512 : index]} : () -> ()
  // CHECK-NEXT: init_tile<512>();
  // CHECK-NEXT: return
  return
}
