.text
.global _x86_64_cr3_load
_x86_64_cr3_load:

movq    %rdi,%rax
movq    %rax,%cr3
retq
