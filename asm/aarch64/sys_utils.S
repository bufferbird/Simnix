.section .text
.global get_main_id
.global get_timer_freq
.global get_main_features
.global get_sp
.global get_curennt_el

get_main_id:
    mrs x0, midr_el1
    ret

get_timer_freq:
    mrs x0, cntfrq_el0
    ret

get_main_features:
    mrs x0, id_aa64pfr0_el1
    ret

get_sp:
    mov x0, sp
    ret


get_current_el:
  mrs x0, CurrentEL
  ret
