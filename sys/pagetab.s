.text

.global _cpu_write_cr3
_cpu_write_cr3:
    movq %rdi, %rax
    movq %rax, %cr3
    retq


.global _cpu_read_cr2
_cpu_read_cr2:
    movq %cr2, %rax
    movq %rax, (%rdi)
    retq
