; NOTE: Assertions have been autogenerated by utils/update_llc_test_checks.py UTC_ARGS: --version 5
; NOTE: Fast Isel is not added because it does not support x87 stores.

; RUN: llc < %s -mtriple=x86_64-- -mattr=+x87,-sse,-sse2 -global-isel=0 | FileCheck %s --check-prefixes X64,SDAG-X64
; RUN: llc < %s -mtriple=x86_64-- -mattr=+x87,-sse,-sse2 -global-isel -global-isel-abort=1 | FileCheck %s --check-prefixes X64,GISEL-X64
; RUN: llc < %s -mtriple=i686-- -mattr=+x87,-sse,-sse2 -global-isel=0 | FileCheck %s --check-prefixes X86,SDAG-X86
; TODO: The last RUN line fails GISEL for f64/double cases and will fallback to DAG due to lack of support for
; loads/stores in X86 mode, support is expected soon enough, for this reason the isel-sint-to-fp64-x86.mir test is added.
; RUN: llc < %s -mtriple=i686-- -mattr=+x87,-sse,-sse2 -global-isel -global-isel-abort=2 | FileCheck %s --check-prefixes X86,GISEL-X86

define void @test_int8_to_float(i8 %x, ptr %p) nounwind {
; SDAG-X64-LABEL: test_int8_to_float:
; SDAG-X64:       # %bb.0: # %entry
; SDAG-X64-NEXT:    movsbl %dil, %eax
; SDAG-X64-NEXT:    movw %ax, -{{[0-9]+}}(%rsp)
; SDAG-X64-NEXT:    filds -{{[0-9]+}}(%rsp)
; SDAG-X64-NEXT:    fstps (%rsi)
; SDAG-X64-NEXT:    retq
;
; GISEL-X64-LABEL: test_int8_to_float:
; GISEL-X64:       # %bb.0: # %entry
; GISEL-X64-NEXT:    shlw $8, %di
; GISEL-X64-NEXT:    sarw $8, %di
; GISEL-X64-NEXT:    movw %di, -{{[0-9]+}}(%rsp)
; GISEL-X64-NEXT:    filds -{{[0-9]+}}(%rsp)
; GISEL-X64-NEXT:    fstps (%rsi)
; GISEL-X64-NEXT:    retq
;
; SDAG-X86-LABEL: test_int8_to_float:
; SDAG-X86:       # %bb.0: # %entry
; SDAG-X86-NEXT:    pushl %eax
; SDAG-X86-NEXT:    movsbl {{[0-9]+}}(%esp), %eax
; SDAG-X86-NEXT:    movl {{[0-9]+}}(%esp), %ecx
; SDAG-X86-NEXT:    movw %ax, {{[0-9]+}}(%esp)
; SDAG-X86-NEXT:    filds {{[0-9]+}}(%esp)
; SDAG-X86-NEXT:    fstps (%ecx)
; SDAG-X86-NEXT:    popl %eax
; SDAG-X86-NEXT:    retl
;
; GISEL-X86-LABEL: test_int8_to_float:
; GISEL-X86:       # %bb.0: # %entry
; GISEL-X86-NEXT:    pushl %eax
; GISEL-X86-NEXT:    movzbl {{[0-9]+}}(%esp), %eax
; GISEL-X86-NEXT:    movl {{[0-9]+}}(%esp), %ecx
; GISEL-X86-NEXT:    shlw $8, %ax
; GISEL-X86-NEXT:    sarw $8, %ax
; GISEL-X86-NEXT:    movw %ax, {{[0-9]+}}(%esp)
; GISEL-X86-NEXT:    filds {{[0-9]+}}(%esp)
; GISEL-X86-NEXT:    fstps (%ecx)
; GISEL-X86-NEXT:    popl %eax
; GISEL-X86-NEXT:    retl
entry:
  %conv = sitofp i8 %x to float
  store float %conv, ptr %p, align 4
  ret void
}

define void @test_int16_to_float(i16 %x, ptr %p) nounwind {
; X64-LABEL: test_int16_to_float:
; X64:       # %bb.0: # %entry
; X64-NEXT:    movw %di, -{{[0-9]+}}(%rsp)
; X64-NEXT:    filds -{{[0-9]+}}(%rsp)
; X64-NEXT:    fstps (%rsi)
; X64-NEXT:    retq
;
; SDAG-X86-LABEL: test_int16_to_float:
; SDAG-X86:       # %bb.0: # %entry
; SDAG-X86-NEXT:    pushl %eax
; SDAG-X86-NEXT:    movl {{[0-9]+}}(%esp), %eax
; SDAG-X86-NEXT:    movzwl {{[0-9]+}}(%esp), %ecx
; SDAG-X86-NEXT:    movw %cx, {{[0-9]+}}(%esp)
; SDAG-X86-NEXT:    filds {{[0-9]+}}(%esp)
; SDAG-X86-NEXT:    fstps (%eax)
; SDAG-X86-NEXT:    popl %eax
; SDAG-X86-NEXT:    retl
;
; GISEL-X86-LABEL: test_int16_to_float:
; GISEL-X86:       # %bb.0: # %entry
; GISEL-X86-NEXT:    pushl %eax
; GISEL-X86-NEXT:    movzwl {{[0-9]+}}(%esp), %eax
; GISEL-X86-NEXT:    movl {{[0-9]+}}(%esp), %ecx
; GISEL-X86-NEXT:    movw %ax, {{[0-9]+}}(%esp)
; GISEL-X86-NEXT:    filds {{[0-9]+}}(%esp)
; GISEL-X86-NEXT:    fstps (%ecx)
; GISEL-X86-NEXT:    popl %eax
; GISEL-X86-NEXT:    retl
entry:
  %conv = sitofp i16 %x to float
  store float %conv, ptr %p, align 4
  ret void
}

define void @test_int32_to_float(i32 %x, ptr %p) nounwind {
; X64-LABEL: test_int32_to_float:
; X64:       # %bb.0: # %entry
; X64-NEXT:    movl %edi, -{{[0-9]+}}(%rsp)
; X64-NEXT:    fildl -{{[0-9]+}}(%rsp)
; X64-NEXT:    fstps (%rsi)
; X64-NEXT:    retq
;
; SDAG-X86-LABEL: test_int32_to_float:
; SDAG-X86:       # %bb.0: # %entry
; SDAG-X86-NEXT:    pushl %eax
; SDAG-X86-NEXT:    movl {{[0-9]+}}(%esp), %eax
; SDAG-X86-NEXT:    movl {{[0-9]+}}(%esp), %ecx
; SDAG-X86-NEXT:    movl %ecx, (%esp)
; SDAG-X86-NEXT:    fildl (%esp)
; SDAG-X86-NEXT:    fstps (%eax)
; SDAG-X86-NEXT:    popl %eax
; SDAG-X86-NEXT:    retl
;
; GISEL-X86-LABEL: test_int32_to_float:
; GISEL-X86:       # %bb.0: # %entry
; GISEL-X86-NEXT:    pushl %eax
; GISEL-X86-NEXT:    movl {{[0-9]+}}(%esp), %eax
; GISEL-X86-NEXT:    movl {{[0-9]+}}(%esp), %ecx
; GISEL-X86-NEXT:    movl %eax, (%esp)
; GISEL-X86-NEXT:    fildl (%esp)
; GISEL-X86-NEXT:    fstps (%ecx)
; GISEL-X86-NEXT:    popl %eax
; GISEL-X86-NEXT:    retl
entry:
  %conv = sitofp i32 %x to float
  store float %conv, ptr %p, align 4
  ret void
}

define void @test_int64_to_float(i64 %x, ptr %p) nounwind {
; X64-LABEL: test_int64_to_float:
; X64:       # %bb.0: # %entry
; X64-NEXT:    movq %rdi, -{{[0-9]+}}(%rsp)
; X64-NEXT:    fildll -{{[0-9]+}}(%rsp)
; X64-NEXT:    fstps (%rsi)
; X64-NEXT:    retq
;
; X86-LABEL: test_int64_to_float:
; X86:       # %bb.0: # %entry
; X86-NEXT:    movl {{[0-9]+}}(%esp), %eax
; X86-NEXT:    fildll {{[0-9]+}}(%esp)
; X86-NEXT:    fstps (%eax)
; X86-NEXT:    retl
entry:
  %conv = sitofp i64 %x to float
  store float %conv, ptr %p, align 4
  ret void
}

define x86_fp80 @test_int8to_longdouble(i8 %a) nounwind {
; SDAG-X64-LABEL: test_int8to_longdouble:
; SDAG-X64:       # %bb.0:
; SDAG-X64-NEXT:    movsbl %dil, %eax
; SDAG-X64-NEXT:    movw %ax, -{{[0-9]+}}(%rsp)
; SDAG-X64-NEXT:    filds -{{[0-9]+}}(%rsp)
; SDAG-X64-NEXT:    retq
;
; GISEL-X64-LABEL: test_int8to_longdouble:
; GISEL-X64:       # %bb.0:
; GISEL-X64-NEXT:    shlw $8, %di
; GISEL-X64-NEXT:    sarw $8, %di
; GISEL-X64-NEXT:    movw %di, -{{[0-9]+}}(%rsp)
; GISEL-X64-NEXT:    filds -{{[0-9]+}}(%rsp)
; GISEL-X64-NEXT:    retq
;
; SDAG-X86-LABEL: test_int8to_longdouble:
; SDAG-X86:       # %bb.0:
; SDAG-X86-NEXT:    pushl %eax
; SDAG-X86-NEXT:    movsbl {{[0-9]+}}(%esp), %eax
; SDAG-X86-NEXT:    movw %ax, {{[0-9]+}}(%esp)
; SDAG-X86-NEXT:    filds {{[0-9]+}}(%esp)
; SDAG-X86-NEXT:    popl %eax
; SDAG-X86-NEXT:    retl
;
; GISEL-X86-LABEL: test_int8to_longdouble:
; GISEL-X86:       # %bb.0:
; GISEL-X86-NEXT:    pushl %eax
; GISEL-X86-NEXT:    movzbl {{[0-9]+}}(%esp), %eax
; GISEL-X86-NEXT:    shlw $8, %ax
; GISEL-X86-NEXT:    sarw $8, %ax
; GISEL-X86-NEXT:    movw %ax, {{[0-9]+}}(%esp)
; GISEL-X86-NEXT:    filds {{[0-9]+}}(%esp)
; GISEL-X86-NEXT:    popl %eax
; GISEL-X86-NEXT:    retl
  %conv = sitofp i8 %a to x86_fp80
  ret x86_fp80 %conv
}

define x86_fp80 @test_int16_to_longdouble(i16 %a) nounwind {
; X64-LABEL: test_int16_to_longdouble:
; X64:       # %bb.0:
; X64-NEXT:    movw %di, -{{[0-9]+}}(%rsp)
; X64-NEXT:    filds -{{[0-9]+}}(%rsp)
; X64-NEXT:    retq
;
; X86-LABEL: test_int16_to_longdouble:
; X86:       # %bb.0:
; X86-NEXT:    pushl %eax
; X86-NEXT:    movzwl {{[0-9]+}}(%esp), %eax
; X86-NEXT:    movw %ax, {{[0-9]+}}(%esp)
; X86-NEXT:    filds {{[0-9]+}}(%esp)
; X86-NEXT:    popl %eax
; X86-NEXT:    retl
  %conv = sitofp i16 %a to x86_fp80
  ret x86_fp80 %conv
}

define x86_fp80 @test_int32_to_longdouble(i32 %a) nounwind {
; X64-LABEL: test_int32_to_longdouble:
; X64:       # %bb.0:
; X64-NEXT:    movl %edi, -{{[0-9]+}}(%rsp)
; X64-NEXT:    fildl -{{[0-9]+}}(%rsp)
; X64-NEXT:    retq
;
; X86-LABEL: test_int32_to_longdouble:
; X86:       # %bb.0:
; X86-NEXT:    pushl %eax
; X86-NEXT:    movl {{[0-9]+}}(%esp), %eax
; X86-NEXT:    movl %eax, (%esp)
; X86-NEXT:    fildl (%esp)
; X86-NEXT:    popl %eax
; X86-NEXT:    retl
  %conv = sitofp i32 %a to x86_fp80
  ret x86_fp80 %conv
}

define x86_fp80 @test_int64_to_longdouble(i64 %a, ptr %p) nounwind {
; X64-LABEL: test_int64_to_longdouble:
; X64:       # %bb.0:
; X64-NEXT:    movq %rdi, -{{[0-9]+}}(%rsp)
; X64-NEXT:    fildll -{{[0-9]+}}(%rsp)
; X64-NEXT:    retq
;
; X86-LABEL: test_int64_to_longdouble:
; X86:       # %bb.0:
; X86-NEXT:    fildll {{[0-9]+}}(%esp)
; X86-NEXT:    retl
  %conv = sitofp i64 %a to x86_fp80
  ret x86_fp80 %conv
}


define void @test_int8to_double(i8 %x, ptr %p) nounwind {
; SDAG-X64-LABEL: test_int8to_double:
; SDAG-X64:       # %bb.0: # %entry
; SDAG-X64-NEXT:    movsbl %dil, %eax
; SDAG-X64-NEXT:    movw %ax, -{{[0-9]+}}(%rsp)
; SDAG-X64-NEXT:    filds -{{[0-9]+}}(%rsp)
; SDAG-X64-NEXT:    fstpl (%rsi)
; SDAG-X64-NEXT:    retq
;
; GISEL-X64-LABEL: test_int8to_double:
; GISEL-X64:       # %bb.0: # %entry
; GISEL-X64-NEXT:    shlw $8, %di
; GISEL-X64-NEXT:    sarw $8, %di
; GISEL-X64-NEXT:    movw %di, -{{[0-9]+}}(%rsp)
; GISEL-X64-NEXT:    filds -{{[0-9]+}}(%rsp)
; GISEL-X64-NEXT:    fstpl (%rsi)
; GISEL-X64-NEXT:    retq
;
; X86-LABEL: test_int8to_double:
; X86:       # %bb.0: # %entry
; X86-NEXT:    pushl %eax
; X86-NEXT:    movsbl {{[0-9]+}}(%esp), %eax
; X86-NEXT:    movl {{[0-9]+}}(%esp), %ecx
; X86-NEXT:    movw %ax, {{[0-9]+}}(%esp)
; X86-NEXT:    filds {{[0-9]+}}(%esp)
; X86-NEXT:    fstpl (%ecx)
; X86-NEXT:    popl %eax
; X86-NEXT:    retl
entry:
  %conv = sitofp i8 %x to double
  store double %conv, ptr %p, align 4
  ret void
}

define void @test_int16_to_double(i16 %x, ptr %p) nounwind {
; X64-LABEL: test_int16_to_double:
; X64:       # %bb.0: # %entry
; X64-NEXT:    movw %di, -{{[0-9]+}}(%rsp)
; X64-NEXT:    filds -{{[0-9]+}}(%rsp)
; X64-NEXT:    fstpl (%rsi)
; X64-NEXT:    retq
;
; X86-LABEL: test_int16_to_double:
; X86:       # %bb.0: # %entry
; X86-NEXT:    pushl %eax
; X86-NEXT:    movl {{[0-9]+}}(%esp), %eax
; X86-NEXT:    movzwl {{[0-9]+}}(%esp), %ecx
; X86-NEXT:    movw %cx, {{[0-9]+}}(%esp)
; X86-NEXT:    filds {{[0-9]+}}(%esp)
; X86-NEXT:    fstpl (%eax)
; X86-NEXT:    popl %eax
; X86-NEXT:    retl
entry:
  %conv = sitofp i16 %x to double
  store double %conv, ptr %p, align 4
  ret void
}

define void @test_int32_to_double(i32 %x, ptr %p) nounwind {
; X64-LABEL: test_int32_to_double:
; X64:       # %bb.0: # %entry
; X64-NEXT:    movl %edi, -{{[0-9]+}}(%rsp)
; X64-NEXT:    fildl -{{[0-9]+}}(%rsp)
; X64-NEXT:    fstpl (%rsi)
; X64-NEXT:    retq
;
; X86-LABEL: test_int32_to_double:
; X86:       # %bb.0: # %entry
; X86-NEXT:    pushl %eax
; X86-NEXT:    movl {{[0-9]+}}(%esp), %eax
; X86-NEXT:    movl {{[0-9]+}}(%esp), %ecx
; X86-NEXT:    movl %ecx, (%esp)
; X86-NEXT:    fildl (%esp)
; X86-NEXT:    fstpl (%eax)
; X86-NEXT:    popl %eax
; X86-NEXT:    retl
entry:
  %conv = sitofp i32 %x to double
  store double %conv, ptr %p, align 4
  ret void
}

define void @test_int64_to_double(i64 %x, ptr %p) nounwind {
; X64-LABEL: test_int64_to_double:
; X64:       # %bb.0: # %entry
; X64-NEXT:    movq %rdi, -{{[0-9]+}}(%rsp)
; X64-NEXT:    fildll -{{[0-9]+}}(%rsp)
; X64-NEXT:    fstpl (%rsi)
; X64-NEXT:    retq
;
; X86-LABEL: test_int64_to_double:
; X86:       # %bb.0: # %entry
; X86-NEXT:    movl {{[0-9]+}}(%esp), %eax
; X86-NEXT:    fildll {{[0-9]+}}(%esp)
; X86-NEXT:    fstpl (%eax)
; X86-NEXT:    retl
entry:
  %conv = sitofp i64 %x to double
  store double %conv, ptr %p, align 4
  ret void
}
