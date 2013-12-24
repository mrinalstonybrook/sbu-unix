
.global _x86_64_asm_switch_first_to
.align 4
_x86_64_asm_switch_first_to:
pushq %rax
pushq %rbx
pushq %rcx
pushq %rdx
pushq %rsi
pushq %rdi
pushq %rbp
pushq %r8
pushq %r9
pushq %r10
pushq %r11
pushq %r12
pushq %r13
pushq %r14
pushq %r15
pushq %rsp
movq    (%rdi),%rsp
popq %rsp
popq %r15
popq %r14
popq %r13
popq %r12
popq %r11
popq %r10
popq %r9
popq %r8
popq %rbp
popq %rdi
popq %rsi
popq %rdx
popq %rcx
popq %rbx
popq %rax
retq



