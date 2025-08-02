	.file	"kmutex.c"
	.text
.Ltext0:
	.file 1 "proc/kmutex.c"
	.type	equals, @function
equals:
.LFB0:
	.file 2 "include/util/debug.h"
	.loc 2 280 1
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movq	%rdi, -8(%rbp)
	movq	%rsi, -16(%rbp)
	.loc 2 281 14
	movq	-8(%rbp), %rax
	cmpq	-16(%rbp), %rax
	sete	%al
	movzbl	%al, %eax
	.loc 2 282 1
	leave
	.cfi_restore 6
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	equals, .-equals
	.type	notequals, @function
notequals:
.LFB1:
	.loc 2 284 39
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movq	%rdi, -8(%rbp)
	movq	%rsi, -16(%rbp)
	.loc 2 284 50
	movq	-8(%rbp), %rax
	cmpq	-16(%rbp), %rax
	setne	%al
	movzbl	%al, %eax
	.loc 2 284 56
	leave
	.cfi_restore 6
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE1:
	.size	notequals, .-notequals
	.type	lessthan, @function
lessthan:
.LFB2:
	.loc 2 286 38
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movq	%rdi, -8(%rbp)
	movq	%rsi, -16(%rbp)
	.loc 2 286 49
	movq	-8(%rbp), %rax
	cmpq	-16(%rbp), %rax
	setl	%al
	movzbl	%al, %eax
	.loc 2 286 54
	leave
	.cfi_restore 6
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE2:
	.size	lessthan, .-lessthan
	.type	greaterthan, @function
greaterthan:
.LFB3:
	.loc 2 288 41
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movq	%rdi, -8(%rbp)
	movq	%rsi, -16(%rbp)
	.loc 2 288 52
	movq	-8(%rbp), %rax
	cmpq	-16(%rbp), %rax
	setg	%al
	movzbl	%al, %eax
	.loc 2 288 57
	leave
	.cfi_restore 6
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE3:
	.size	greaterthan, .-greaterthan
	.type	lessthaneq, @function
lessthaneq:
.LFB4:
	.loc 2 290 40
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movq	%rdi, -8(%rbp)
	movq	%rsi, -16(%rbp)
	.loc 2 290 51
	movq	-8(%rbp), %rax
	cmpq	-16(%rbp), %rax
	setle	%al
	movzbl	%al, %eax
	.loc 2 290 57
	leave
	.cfi_restore 6
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE4:
	.size	lessthaneq, .-lessthaneq
	.type	greaterthaneq, @function
greaterthaneq:
.LFB5:
	.loc 2 292 43
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movq	%rdi, -8(%rbp)
	movq	%rsi, -16(%rbp)
	.loc 2 292 54
	movq	-8(%rbp), %rax
	cmpq	-16(%rbp), %rax
	setge	%al
	movzbl	%al, %eax
	.loc 2 292 60
	leave
	.cfi_restore 6
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE5:
	.size	greaterthaneq, .-greaterthaneq
	.type	intr_enabled, @function
intr_enabled:
.LFB6:
	.file 3 "include/main/interrupt.h"
	.loc 3 82 1
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	.loc 3 84 5
#APP
# 84 "include/main/interrupt.h" 1
	pushf; pop %rax; and $0x200, %rax;
# 0 "" 2
#NO_APP
	movq	%rax, -8(%rbp)
	.loc 3 86 12
	movq	-8(%rbp), %rax
	.loc 3 87 1
	leave
	.cfi_restore 6
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE6:
	.size	intr_enabled, .-intr_enabled
	.type	intr_enable, @function
intr_enable:
.LFB7:
	.loc 3 89 34
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	.loc 3 89 36
#APP
# 89 "include/main/interrupt.h" 1
	sti
# 0 "" 2
	.loc 3 89 61
#NO_APP
	nop
	popq	%rbp
	.cfi_restore 6
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE7:
	.size	intr_enable, .-intr_enable
	.type	intr_disable, @function
intr_disable:
.LFB8:
	.loc 3 91 35
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	.loc 3 91 37
#APP
# 91 "include/main/interrupt.h" 1
	cli
# 0 "" 2
	.loc 3 91 62
#NO_APP
	nop
	popq	%rbp
	.cfi_restore 6
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE8:
	.size	intr_disable, .-intr_disable
	.type	intr_wait, @function
intr_wait:
.LFB9:
	.loc 3 98 1
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	.loc 3 105 5
#APP
# 105 "include/main/interrupt.h" 1
	sti; hlt
# 0 "" 2
	.loc 3 106 1
#NO_APP
	nop
	popq	%rbp
	.cfi_restore 6
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE9:
	.size	intr_wait, .-intr_wait
	.section	.rodata
	.align 8
.LC0:
	.string	"detected deadlock between P%d and P%d (mutexes 0x%p, 0x%p)\n"
.LC1:
	.string	"proc/kmutex.c"
	.text
	.globl	detect_deadlocks
	.type	detect_deadlocks, @function
detect_deadlocks:
.LFB10:
	.loc 1 20 1
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$48, %rsp
	movq	%rdi, -40(%rbp)
.LBB8:
	.loc 1 22 5
	movabsq	$curthr, %rax
	movq	(%rax), %rax
	movq	136(%rax), %rax
	subq	$32, %rax
	movq	%rax, -8(%rbp)
	movq	-8(%rbp), %rax
	movq	32(%rax), %rax
	subq	$32, %rax
	movq	%rax, -16(%rbp)
	jmp	.L19
.L23:
.LBB9:
	.loc 1 24 9
	movq	-8(%rbp), %rax
	movq	(%rax), %rax
	subq	$120, %rax
	movq	%rax, -24(%rbp)
	movq	-24(%rbp), %rax
	movq	120(%rax), %rax
	subq	$120, %rax
	movq	%rax, -32(%rbp)
	jmp	.L20
.L22:
	.loc 1 26 30
	movq	-40(%rbp), %rax
	movq	24(%rax), %rax
	.loc 1 26 16
	cmpq	%rax, -24(%rbp)
	jne	.L21
	.loc 1 28 17
	movq	-24(%rbp), %rax
	movq	72(%rax), %rax
	movl	(%rax), %edx
	movabsq	$curproc, %rax
	movq	(%rax), %rax
	movl	(%rax), %eax
	pushq	-40(%rbp)
	pushq	-8(%rbp)
	movl	%edx, %r9d
	movl	%eax, %r8d
	movabsq	$.LC0, %rax
	movq	%rax, %rcx
	movabsq	$__func__.2, %rax
	movq	%rax, %rdx
	movl	$28, %esi
	movabsq	$.LC1, %rax
	movq	%rax, %rdi
	movl	$0, %eax
	movabsq	$dbg_panic, %r10
	call	*%r10
.LVL0:
.L21:
	.loc 1 24 9 discriminator 2
	movq	-32(%rbp), %rax
	movq	%rax, -24(%rbp)
	movq	-24(%rbp), %rax
	movq	120(%rax), %rax
	subq	$120, %rax
	movq	%rax, -32(%rbp)
.L20:
	.loc 1 24 9 is_stmt 0 discriminator 1
	movq	-24(%rbp), %rax
	leaq	120(%rax), %rdx
	movq	-8(%rbp), %rax
	cmpq	%rax, %rdx
	jne	.L22
.LBE9:
	.loc 1 22 5 is_stmt 1 discriminator 2
	movq	-16(%rbp), %rax
	movq	%rax, -8(%rbp)
	movq	-8(%rbp), %rax
	movq	32(%rax), %rax
	subq	$32, %rax
	movq	%rax, -16(%rbp)
.L19:
	.loc 1 22 5 is_stmt 0 discriminator 1
	movq	-8(%rbp), %rax
	leaq	32(%rax), %rdx
	movabsq	$curthr, %rax
	movq	(%rax), %rax
	addq	$136, %rax
	cmpq	%rax, %rdx
	jne	.L23
.LBE8:
	.loc 1 36 1 is_stmt 1
	nop
	nop
	leave
	.cfi_restore 6
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE10:
	.size	detect_deadlocks, .-detect_deadlocks
	.globl	kmutex_init
	.type	kmutex_init, @function
kmutex_init:
.LFB11:
	.loc 1 42 1
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movq	%rdi, -8(%rbp)
	.loc 1 44 20
	movq	-8(%rbp), %rax
	movq	$0, 24(%rax)
	.loc 1 45 5
	movq	-8(%rbp), %rax
	movq	%rax, %rdi
	movabsq	$sched_queue_init, %rax
	call	*%rax
.LVL1:
	.loc 1 46 5
	movq	-8(%rbp), %rax
	addq	$32, %rax
	movq	%rax, %rdi
	movabsq	$list_link_init, %rax
	call	*%rax
.LVL2:
	.loc 1 48 1
	nop
	leave
	.cfi_restore 6
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE11:
	.size	kmutex_init, .-kmutex_init
	.section	.rodata
.LC2:
	.string	"%s"
.LC3:
	.string	"C%ld P%ld "
.LC4:
	.string	"%s:%d %s(): "
.LC5:
	.string	"locked mutex: %p\n"
.LC6:
	.string	"\033[0m"
	.align 8
.LC7:
	.string	"curthr && \"need thread context to lock mutex\""
.LC8:
	.string	"assertion failed: %s"
	.align 8
.LC9:
	.string	"!kmutex_owns_mutex(mtx) && \"already owner\""
.LC10:
	.string	"kmutex_owns_mutex(mtx)"
	.text
	.globl	kmutex_lock
	.type	kmutex_lock, @function
kmutex_lock:
.LFB12:
	.loc 1 58 1
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$32, %rsp
	movq	%rdi, -24(%rbp)
	.loc 1 61 5
	movabsq	$dbg_modes, %rax
	movq	(%rax), %rax
	andl	$16384, %eax
	testq	%rax, %rax
	je	.L26
	.loc 1 61 5 is_stmt 0 discriminator 1
	movl	$16384, %edi
	movabsq	$dbg_color, %rax
	call	*%rax
.LVL3:
	movq	%rax, %rsi
	movabsq	$.LC2, %rax
	movq	%rax, %rdi
	movl	$0, %eax
	movabsq	$dbg_print, %rdx
	call	*%rdx
.LVL4:
	movabsq	$curproc, %rax
	movq	(%rax), %rax
	testq	%rax, %rax
	je	.L27
	.loc 1 61 5 discriminator 2
	movabsq	$curproc, %rax
	movq	(%rax), %rax
	movl	(%rax), %eax
	cltq
	jmp	.L28
.L27:
	.loc 1 61 5 discriminator 3
	movq	$-1, %rax
.L28:
	.loc 1 61 5 discriminator 5
	movabsq	$curcore, %rdx
	movq	(%rdx), %rcx
	movq	%rax, %rdx
	movq	%rcx, %rsi
	movabsq	$.LC3, %rax
	movq	%rax, %rdi
	movl	$0, %eax
	movabsq	$dbg_print, %rcx
	call	*%rcx
.LVL5:
	movabsq	$__func__.1, %rax
	movq	%rax, %rcx
	movl	$61, %edx
	movabsq	$.LC1, %rax
	movq	%rax, %rsi
	movabsq	$.LC4, %rax
	movq	%rax, %rdi
	movl	$0, %eax
	movabsq	$dbg_print, %r8
	call	*%r8
.LVL6:
	movq	-24(%rbp), %rax
	movq	%rax, %rsi
	movabsq	$.LC5, %rax
	movq	%rax, %rdi
	movl	$0, %eax
	movabsq	$dbg_print, %rdx
	call	*%rdx
.LVL7:
	movabsq	$.LC6, %rax
	movq	%rax, %rsi
	movabsq	$.LC2, %rax
	movq	%rax, %rdi
	movl	$0, %eax
	movabsq	$dbg_print, %rdx
	call	*%rdx
.LVL8:
.L26:
	.loc 1 62 5 is_stmt 1
	movabsq	$curthr, %rax
	movq	(%rax), %rax
	testq	%rax, %rax
	jne	.L29
	.loc 1 62 5 is_stmt 0 discriminator 1
	movabsq	$.LC7, %r8
	movabsq	$.LC8, %rax
	movq	%rax, %rcx
	movabsq	$__func__.1, %rax
	movq	%rax, %rdx
	movl	$62, %esi
	movabsq	$.LC1, %rax
	movq	%rax, %rdi
	movl	$0, %eax
	movabsq	$dbg_panic, %r9
	call	*%r9
.LVL9:
.L29:
	movq	-24(%rbp), %rax
	movq	%rax, -8(%rbp)
.LBB10:
.LBB11:
	.loc 1 117 12 is_stmt 1
	movabsq	$curthr, %rax
	movq	(%rax), %rax
	.loc 1 117 19
	testq	%rax, %rax
	je	.L30
	.loc 1 117 25
	movq	-8(%rbp), %rax
	movq	24(%rax), %rdx
	.loc 1 117 37
	movabsq	$curthr, %rax
	movq	(%rax), %rax
	.loc 1 117 19
	cmpq	%rax, %rdx
	jne	.L30
	movl	$1, %eax
	jmp	.L31
.L30:
	movl	$0, %eax
.L31:
	cltq
.LBE11:
.LBE10:
	.loc 1 63 5
	testq	%rax, %rax
	je	.L33
	.loc 1 63 5 is_stmt 0 discriminator 3
	movabsq	$.LC9, %r8
	movabsq	$.LC8, %rax
	movq	%rax, %rcx
	movabsq	$__func__.1, %rax
	movq	%rax, %rdx
	movl	$63, %esi
	movabsq	$.LC1, %rax
	movq	%rax, %rdi
	movl	$0, %eax
	movabsq	$dbg_panic, %r9
	call	*%r9
.LVL10:
.L33:
	.loc 1 65 12 is_stmt 1
	movq	-24(%rbp), %rax
	movq	24(%rax), %rax
	.loc 1 65 8
	testq	%rax, %rax
	je	.L34
	.loc 1 67 9
	movq	-24(%rbp), %rax
	movq	%rax, %rdi
	movabsq	$detect_deadlocks, %rax
	call	*%rax
.LVL11:
	.loc 1 68 9
	movq	-24(%rbp), %rax
	movq	%rax, %rdi
	movabsq	$sched_sleep_on, %rax
	call	*%rax
.LVL12:
	movq	-24(%rbp), %rax
	movq	%rax, -16(%rbp)
.LBB12:
.LBB13:
	.loc 1 117 12
	movabsq	$curthr, %rax
	movq	(%rax), %rax
	.loc 1 117 19
	testq	%rax, %rax
	je	.L35
	.loc 1 117 25
	movq	-16(%rbp), %rax
	movq	24(%rax), %rdx
	.loc 1 117 37
	movabsq	$curthr, %rax
	movq	(%rax), %rax
	.loc 1 117 19
	cmpq	%rax, %rdx
	jne	.L35
	movl	$1, %eax
	jmp	.L36
.L35:
	movl	$0, %eax
.L36:
	cltq
.LBE13:
.LBE12:
	.loc 1 69 9
	testq	%rax, %rax
	jne	.L39
	.loc 1 69 9 is_stmt 0 discriminator 1
	movabsq	$.LC10, %r8
	movabsq	$.LC8, %rax
	movq	%rax, %rcx
	movabsq	$__func__.1, %rax
	movq	%rax, %rdx
	movl	$69, %esi
	movabsq	$.LC1, %rax
	movq	%rax, %rdi
	movl	$0, %eax
	movabsq	$dbg_panic, %r9
	call	*%r9
.LVL13:
.L34:
	.loc 1 73 24 is_stmt 1
	movabsq	$curthr, %rax
	movq	(%rax), %rdx
	movq	-24(%rbp), %rax
	movq	%rdx, 24(%rax)
	.loc 1 74 9
	movq	-24(%rbp), %rax
	leaq	32(%rax), %rdx
	.loc 1 74 33
	movabsq	$curthr, %rax
	movq	(%rax), %rax
	.loc 1 74 9
	addq	$136, %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	movabsq	$list_insert_tail, %rax
	call	*%rax
.LVL14:
.L39:
	.loc 1 77 1
	nop
	leave
	.cfi_restore 6
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE12:
	.size	kmutex_lock, .-kmutex_lock
	.section	.rodata
.LC11:
	.string	"unlocked mutex: %p\n"
	.align 8
.LC12:
	.string	"curthr && (curthr == mtx->km_holder) && \"unlocking a mutex we don\\'t own\""
.LC13:
	.string	"!kmutex_owns_mutex(mtx)"
	.text
	.globl	kmutex_unlock
	.type	kmutex_unlock, @function
kmutex_unlock:
.LFB13:
	.loc 1 89 1
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$32, %rsp
	movq	%rdi, -24(%rbp)
	.loc 1 91 5
	movabsq	$dbg_modes, %rax
	movq	(%rax), %rax
	andl	$16384, %eax
	testq	%rax, %rax
	je	.L41
	.loc 1 91 5 is_stmt 0 discriminator 1
	movl	$16384, %edi
	movabsq	$dbg_color, %rax
	call	*%rax
.LVL15:
	movq	%rax, %rsi
	movabsq	$.LC2, %rax
	movq	%rax, %rdi
	movl	$0, %eax
	movabsq	$dbg_print, %rdx
	call	*%rdx
.LVL16:
	movabsq	$curproc, %rax
	movq	(%rax), %rax
	testq	%rax, %rax
	je	.L42
	.loc 1 91 5 discriminator 2
	movabsq	$curproc, %rax
	movq	(%rax), %rax
	movl	(%rax), %eax
	cltq
	jmp	.L43
.L42:
	.loc 1 91 5 discriminator 3
	movq	$-1, %rax
.L43:
	.loc 1 91 5 discriminator 5
	movabsq	$curcore, %rdx
	movq	(%rdx), %rcx
	movq	%rax, %rdx
	movq	%rcx, %rsi
	movabsq	$.LC3, %rax
	movq	%rax, %rdi
	movl	$0, %eax
	movabsq	$dbg_print, %rcx
	call	*%rcx
.LVL17:
	movabsq	$__func__.0, %rax
	movq	%rax, %rcx
	movl	$91, %edx
	movabsq	$.LC1, %rax
	movq	%rax, %rsi
	movabsq	$.LC4, %rax
	movq	%rax, %rdi
	movl	$0, %eax
	movabsq	$dbg_print, %r8
	call	*%r8
.LVL18:
	movq	-24(%rbp), %rax
	movq	%rax, %rsi
	movabsq	$.LC11, %rax
	movq	%rax, %rdi
	movl	$0, %eax
	movabsq	$dbg_print, %rdx
	call	*%rdx
.LVL19:
	movabsq	$.LC6, %rax
	movq	%rax, %rsi
	movabsq	$.LC2, %rax
	movq	%rax, %rdi
	movl	$0, %eax
	movabsq	$dbg_print, %rdx
	call	*%rdx
.LVL20:
.L41:
	.loc 1 92 5 is_stmt 1
	movabsq	$curthr, %rax
	movq	(%rax), %rax
	testq	%rax, %rax
	je	.L44
	.loc 1 92 5 is_stmt 0 discriminator 2
	movq	-24(%rbp), %rax
	movq	24(%rax), %rdx
	movabsq	$curthr, %rax
	movq	(%rax), %rax
	cmpq	%rax, %rdx
	je	.L45
.L44:
	.loc 1 92 5 discriminator 3
	movabsq	$.LC12, %r8
	movabsq	$.LC8, %rax
	movq	%rax, %rcx
	movabsq	$__func__.0, %rax
	movq	%rax, %rdx
	movl	$92, %esi
	movabsq	$.LC1, %rax
	movq	%rax, %rdi
	movl	$0, %eax
	movabsq	$dbg_panic, %r9
	call	*%r9
.LVL21:
.L45:
	.loc 1 94 5 is_stmt 1
	movq	-24(%rbp), %rax
	leaq	24(%rax), %rdx
	movq	-24(%rbp), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	movabsq	$sched_wakeup_on, %rax
	call	*%rax
.LVL22:
	movq	-24(%rbp), %rax
	movq	%rax, -8(%rbp)
.LBB14:
.LBB15:
	.loc 1 117 12
	movabsq	$curthr, %rax
	movq	(%rax), %rax
	.loc 1 117 19
	testq	%rax, %rax
	je	.L46
	.loc 1 117 25
	movq	-8(%rbp), %rax
	movq	24(%rax), %rdx
	.loc 1 117 37
	movabsq	$curthr, %rax
	movq	(%rax), %rax
	.loc 1 117 19
	cmpq	%rax, %rdx
	jne	.L46
	movl	$1, %eax
	jmp	.L47
.L46:
	movl	$0, %eax
.L47:
	cltq
.LBE15:
.LBE14:
	.loc 1 95 5
	testq	%rax, %rax
	je	.L49
	.loc 1 95 5 is_stmt 0 discriminator 1
	movabsq	$.LC13, %r8
	movabsq	$.LC8, %rax
	movq	%rax, %rcx
	movabsq	$__func__.0, %rax
	movq	%rax, %rdx
	movl	$95, %esi
	movabsq	$.LC1, %rax
	movq	%rax, %rdi
	movl	$0, %eax
	movabsq	$dbg_panic, %r9
	call	*%r9
.LVL23:
.L49:
	.loc 1 96 5 is_stmt 1
	movq	-24(%rbp), %rax
	addq	$32, %rax
	movq	%rax, %rdi
	movabsq	$list_remove, %rax
	call	*%rax
.LVL24:
	.loc 1 97 12
	movq	-24(%rbp), %rax
	movq	24(%rax), %rax
	.loc 1 97 8
	testq	%rax, %rax
	je	.L51
	.loc 1 98 9
	movq	-24(%rbp), %rax
	leaq	32(%rax), %rdx
	.loc 1 98 30
	movq	-24(%rbp), %rax
	movq	24(%rax), %rax
	.loc 1 98 9
	addq	$136, %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	movabsq	$list_insert_tail, %rax
	call	*%rax
.LVL25:
.L51:
	.loc 1 101 1
	nop
	leave
	.cfi_restore 6
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE13:
	.size	kmutex_unlock, .-kmutex_unlock
	.globl	kmutex_has_waiters
	.type	kmutex_has_waiters, @function
kmutex_has_waiters:
.LFB14:
	.loc 1 107 1
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movq	%rdi, -8(%rbp)
	.loc 1 108 13
	movq	-8(%rbp), %rax
	movq	%rax, %rdi
	movabsq	$sched_queue_empty, %rax
	call	*%rax
.LVL26:
	.loc 1 108 12
	testq	%rax, %rax
	sete	%al
	movzbl	%al, %eax
	.loc 1 110 1
	leave
	.cfi_restore 6
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE14:
	.size	kmutex_has_waiters, .-kmutex_has_waiters
	.globl	kmutex_owns_mutex
	.type	kmutex_owns_mutex, @function
kmutex_owns_mutex:
.LFB15:
	.loc 1 116 1
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$8, %rsp
	movq	%rdi, -8(%rbp)
	.loc 1 117 12
	movabsq	$curthr, %rax
	movq	(%rax), %rax
	.loc 1 117 19
	testq	%rax, %rax
	je	.L55
	.loc 1 117 25 discriminator 1
	movq	-8(%rbp), %rax
	movq	24(%rax), %rdx
	.loc 1 117 37 discriminator 1
	movabsq	$curthr, %rax
	movq	(%rax), %rax
	.loc 1 117 19 discriminator 1
	cmpq	%rax, %rdx
	jne	.L55
	.loc 1 117 19 is_stmt 0 discriminator 3
	movl	$1, %eax
	jmp	.L56
.L55:
	.loc 1 117 19 discriminator 4
	movl	$0, %eax
.L56:
	.loc 1 117 19 discriminator 6
	cltq
	.loc 1 118 1 is_stmt 1 discriminator 6
	leave
	.cfi_restore 6
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE15:
	.size	kmutex_owns_mutex, .-kmutex_owns_mutex
	.section	.rodata
	.align 16
	.type	__func__.2, @object
	.size	__func__.2, 17
__func__.2:
	.string	"detect_deadlocks"
	.align 8
	.type	__func__.1, @object
	.size	__func__.1, 12
__func__.1:
	.string	"kmutex_lock"
	.align 8
	.type	__func__.0, @object
	.size	__func__.0, 14
__func__.0:
	.string	"kmutex_unlock"
	.text
.Letext0:
	.file 4 "include/types.h"
	.file 5 "include/util/list.h"
	.file 6 "include/proc/sched.h"
	.file 7 "include/proc/kmutex.h"
	.file 8 "include/proc/kthread.h"
	.file 9 "include/vm/vmmap.h"
	.file 10 "include/proc/proc.h"
	.file 11 "include/mm/pagetable.h"
	.file 12 "include/proc/context.h"
	.file 13 "include/proc/core.h"
	.file 14 "include/globals.h"
	.section	.debug_info,"",@progbits
.Ldebug_info0:
	.long	0xc3d
	.value	0x3
	.long	.Ldebug_abbrev0
	.byte	0x8
	.uleb128 0x1
	.long	.LASF838
	.byte	0xc
	.long	.LASF839
	.long	.LASF840
	.quad	.Ltext0
	.quad	.Letext0
	.long	.Ldebug_line0
	.long	.Ldebug_macro0
	.uleb128 0x2
	.byte	0x1
	.byte	0x6
	.long	.LASF714
	.uleb128 0x3
	.long	0x31
	.uleb128 0x2
	.byte	0x1
	.byte	0x6
	.long	.LASF715
	.uleb128 0x2
	.byte	0x1
	.byte	0x8
	.long	.LASF716
	.uleb128 0x2
	.byte	0x2
	.byte	0x5
	.long	.LASF717
	.uleb128 0x2
	.byte	0x2
	.byte	0x7
	.long	.LASF718
	.uleb128 0x4
	.long	.LASF721
	.byte	0x4
	.byte	0xd
	.byte	0x14
	.long	0x65
	.uleb128 0x5
	.byte	0x4
	.byte	0x5
	.string	"int"
	.uleb128 0x2
	.byte	0x4
	.byte	0x7
	.long	.LASF719
	.uleb128 0x2
	.byte	0x8
	.byte	0x5
	.long	.LASF720
	.uleb128 0x4
	.long	.LASF722
	.byte	0x4
	.byte	0x11
	.byte	0x17
	.long	0x86
	.uleb128 0x2
	.byte	0x8
	.byte	0x7
	.long	.LASF723
	.uleb128 0x4
	.long	.LASF724
	.byte	0x4
	.byte	0x13
	.byte	0x17
	.long	0x86
	.uleb128 0x4
	.long	.LASF725
	.byte	0x4
	.byte	0x14
	.byte	0x12
	.long	0x7a
	.uleb128 0x4
	.long	.LASF726
	.byte	0x4
	.byte	0x18
	.byte	0x11
	.long	0x59
	.uleb128 0x6
	.byte	0x8
	.uleb128 0x7
	.long	.LASF731
	.byte	0x10
	.byte	0x5
	.byte	0x5e
	.byte	0x10
	.long	0xdb
	.uleb128 0x8
	.long	.LASF727
	.byte	0x5
	.byte	0x60
	.byte	0x12
	.long	0xdb
	.byte	0
	.uleb128 0x8
	.long	.LASF728
	.byte	0x5
	.byte	0x61
	.byte	0x12
	.long	0xdb
	.byte	0x8
	.byte	0
	.uleb128 0x9
	.byte	0x8
	.long	0xb3
	.uleb128 0x4
	.long	.LASF729
	.byte	0x5
	.byte	0x62
	.byte	0x3
	.long	0xb3
	.uleb128 0x4
	.long	.LASF730
	.byte	0x5
	.byte	0x62
	.byte	0xb
	.long	0xb3
	.uleb128 0x7
	.long	.LASF732
	.byte	0x18
	.byte	0x6
	.byte	0xf
	.byte	0x10
	.long	0x121
	.uleb128 0x8
	.long	.LASF733
	.byte	0x6
	.byte	0x11
	.byte	0xc
	.long	0xe1
	.byte	0
	.uleb128 0x8
	.long	.LASF734
	.byte	0x6
	.byte	0x12
	.byte	0xc
	.long	0x99
	.byte	0x10
	.byte	0
	.uleb128 0x4
	.long	.LASF735
	.byte	0x6
	.byte	0x13
	.byte	0x3
	.long	0xf9
	.uleb128 0x7
	.long	.LASF736
	.byte	0x30
	.byte	0x7
	.byte	0xa
	.byte	0x10
	.long	0x162
	.uleb128 0x8
	.long	.LASF737
	.byte	0x7
	.byte	0xc
	.byte	0xf
	.long	0x121
	.byte	0
	.uleb128 0x8
	.long	.LASF738
	.byte	0x7
	.byte	0xd
	.byte	0x15
	.long	0x20c
	.byte	0x18
	.uleb128 0x8
	.long	.LASF739
	.byte	0x7
	.byte	0xe
	.byte	0x11
	.long	0xed
	.byte	0x20
	.byte	0
	.uleb128 0x7
	.long	.LASF740
	.byte	0xa0
	.byte	0x8
	.byte	0x21
	.byte	0x10
	.long	0x20c
	.uleb128 0x8
	.long	.LASF741
	.byte	0x8
	.byte	0x23
	.byte	0xf
	.long	0x3d9
	.byte	0
	.uleb128 0x8
	.long	.LASF742
	.byte	0x8
	.byte	0x24
	.byte	0xb
	.long	0x478
	.byte	0x30
	.uleb128 0x8
	.long	.LASF743
	.byte	0x8
	.byte	0x25
	.byte	0xb
	.long	0xb1
	.byte	0x38
	.uleb128 0x8
	.long	.LASF744
	.byte	0x8
	.byte	0x26
	.byte	0xa
	.long	0x73
	.byte	0x40
	.uleb128 0x8
	.long	.LASF745
	.byte	0x8
	.byte	0x27
	.byte	0x12
	.long	0x333
	.byte	0x48
	.uleb128 0x8
	.long	.LASF746
	.byte	0x8
	.byte	0x29
	.byte	0xa
	.long	0x73
	.byte	0x50
	.uleb128 0x8
	.long	.LASF747
	.byte	0x8
	.byte	0x2a
	.byte	0x10
	.long	0x427
	.byte	0x58
	.uleb128 0x8
	.long	.LASF748
	.byte	0x8
	.byte	0x2b
	.byte	0x15
	.long	0x46c
	.byte	0x60
	.uleb128 0x8
	.long	.LASF749
	.byte	0x8
	.byte	0x2d
	.byte	0x11
	.long	0xed
	.byte	0x68
	.uleb128 0x8
	.long	.LASF750
	.byte	0x8
	.byte	0x2f
	.byte	0x9
	.long	0xed
	.byte	0x78
	.uleb128 0x8
	.long	.LASF751
	.byte	0x8
	.byte	0x31
	.byte	0xc
	.long	0xe1
	.byte	0x88
	.uleb128 0x8
	.long	.LASF752
	.byte	0x8
	.byte	0x34
	.byte	0xe
	.long	0x7a
	.byte	0x98
	.byte	0
	.uleb128 0x9
	.byte	0x8
	.long	0x162
	.uleb128 0x4
	.long	.LASF753
	.byte	0x7
	.byte	0xf
	.byte	0x3
	.long	0x12d
	.uleb128 0x7
	.long	.LASF754
	.byte	0x18
	.byte	0x9
	.byte	0xe
	.byte	0x10
	.long	0x246
	.uleb128 0x8
	.long	.LASF755
	.byte	0x9
	.byte	0x10
	.byte	0xc
	.long	0xe1
	.byte	0
	.uleb128 0x8
	.long	.LASF756
	.byte	0x9
	.byte	0x11
	.byte	0x12
	.long	0x333
	.byte	0x10
	.byte	0
	.uleb128 0xa
	.long	.LASF757
	.value	0x2a0
	.byte	0xa
	.byte	0x1b
	.byte	0x10
	.long	0x333
	.uleb128 0x8
	.long	.LASF758
	.byte	0xa
	.byte	0x1d
	.byte	0xb
	.long	0xa5
	.byte	0
	.uleb128 0x8
	.long	.LASF759
	.byte	0xa
	.byte	0x1e
	.byte	0xa
	.long	0x4b1
	.byte	0x4
	.uleb128 0xb
	.long	.LASF760
	.byte	0xa
	.byte	0x20
	.byte	0xc
	.long	0xe1
	.value	0x108
	.uleb128 0xb
	.long	.LASF761
	.byte	0xa
	.byte	0x21
	.byte	0xc
	.long	0xe1
	.value	0x118
	.uleb128 0xb
	.long	.LASF762
	.byte	0xa
	.byte	0x22
	.byte	0x12
	.long	0x333
	.value	0x128
	.uleb128 0xb
	.long	.LASF763
	.byte	0xa
	.byte	0x24
	.byte	0x11
	.long	0xed
	.value	0x130
	.uleb128 0xb
	.long	.LASF764
	.byte	0xa
	.byte	0x25
	.byte	0x11
	.long	0xed
	.value	0x140
	.uleb128 0xb
	.long	.LASF765
	.byte	0xa
	.byte	0x27
	.byte	0xa
	.long	0x73
	.value	0x150
	.uleb128 0xb
	.long	.LASF766
	.byte	0xa
	.byte	0x28
	.byte	0x12
	.long	0x4a5
	.value	0x158
	.uleb128 0xb
	.long	.LASF767
	.byte	0xa
	.byte	0x2a
	.byte	0xd
	.long	0x3d3
	.value	0x160
	.uleb128 0xb
	.long	.LASF768
	.byte	0xa
	.byte	0x31
	.byte	0xf
	.long	0x121
	.value	0x168
	.uleb128 0xb
	.long	.LASF769
	.byte	0xa
	.byte	0x34
	.byte	0x12
	.long	0x4c1
	.value	0x180
	.uleb128 0xb
	.long	.LASF770
	.byte	0xa
	.byte	0x35
	.byte	0x13
	.long	0x4e3
	.value	0x280
	.uleb128 0xb
	.long	.LASF771
	.byte	0xa
	.byte	0x45
	.byte	0xb
	.long	0xb1
	.value	0x288
	.uleb128 0xb
	.long	.LASF772
	.byte	0xa
	.byte	0x46
	.byte	0xb
	.long	0xb1
	.value	0x290
	.uleb128 0xb
	.long	.LASF773
	.byte	0xa
	.byte	0x47
	.byte	0x13
	.long	0x339
	.value	0x298
	.byte	0
	.uleb128 0x9
	.byte	0x8
	.long	0x246
	.uleb128 0x9
	.byte	0x8
	.long	0x21e
	.uleb128 0xc
	.string	"pt"
	.value	0x1000
	.byte	0xb
	.byte	0x18
	.byte	0x10
	.long	0x35a
	.uleb128 0x8
	.long	.LASF774
	.byte	0xb
	.byte	0x1a
	.byte	0xf
	.long	0x35a
	.byte	0
	.byte	0
	.uleb128 0xd
	.long	0x8d
	.long	0x36b
	.uleb128 0xe
	.long	0x86
	.value	0x1ff
	.byte	0
	.uleb128 0x4
	.long	.LASF775
	.byte	0xb
	.byte	0x1b
	.byte	0x16
	.long	0x33f
	.uleb128 0x7
	.long	.LASF776
	.byte	0x30
	.byte	0xc
	.byte	0xd
	.byte	0x10
	.long	0x3d3
	.uleb128 0x8
	.long	.LASF777
	.byte	0xc
	.byte	0xf
	.byte	0xf
	.long	0x8d
	.byte	0
	.uleb128 0x8
	.long	.LASF778
	.byte	0xc
	.byte	0x10
	.byte	0xf
	.long	0x8d
	.byte	0x8
	.uleb128 0x8
	.long	.LASF779
	.byte	0xc
	.byte	0x11
	.byte	0xf
	.long	0x8d
	.byte	0x10
	.uleb128 0x8
	.long	.LASF780
	.byte	0xc
	.byte	0x14
	.byte	0xa
	.long	0x3d3
	.byte	0x18
	.uleb128 0x8
	.long	.LASF781
	.byte	0xc
	.byte	0x18
	.byte	0xf
	.long	0x8d
	.byte	0x20
	.uleb128 0x8
	.long	.LASF782
	.byte	0xc
	.byte	0x19
	.byte	0xc
	.long	0x99
	.byte	0x28
	.byte	0
	.uleb128 0x9
	.byte	0x8
	.long	0x36b
	.uleb128 0x4
	.long	.LASF783
	.byte	0xc
	.byte	0x1a
	.byte	0x3
	.long	0x377
	.uleb128 0x7
	.long	.LASF784
	.byte	0x48
	.byte	0xd
	.byte	0x7
	.byte	0x10
	.long	0x427
	.uleb128 0x8
	.long	.LASF785
	.byte	0xd
	.byte	0x9
	.byte	0xa
	.long	0x73
	.byte	0
	.uleb128 0x8
	.long	.LASF786
	.byte	0xd
	.byte	0xa
	.byte	0xf
	.long	0x3d9
	.byte	0x8
	.uleb128 0x8
	.long	.LASF787
	.byte	0xd
	.byte	0xc
	.byte	0x10
	.long	0x427
	.byte	0x38
	.uleb128 0x8
	.long	.LASF788
	.byte	0xd
	.byte	0xe
	.byte	0xf
	.long	0x8d
	.byte	0x40
	.byte	0
	.uleb128 0x9
	.byte	0x8
	.long	0x121
	.uleb128 0x4
	.long	.LASF789
	.byte	0xd
	.byte	0xf
	.byte	0x3
	.long	0x3e5
	.uleb128 0xf
	.byte	0x7
	.byte	0x4
	.long	0x6c
	.byte	0x8
	.byte	0x15
	.byte	0x1
	.long	0x46c
	.uleb128 0x10
	.long	.LASF790
	.byte	0
	.uleb128 0x10
	.long	.LASF791
	.byte	0x1
	.uleb128 0x10
	.long	.LASF792
	.byte	0x2
	.uleb128 0x10
	.long	.LASF793
	.byte	0x3
	.uleb128 0x10
	.long	.LASF794
	.byte	0x4
	.uleb128 0x10
	.long	.LASF795
	.byte	0x5
	.byte	0
	.uleb128 0x4
	.long	.LASF796
	.byte	0x8
	.byte	0x1c
	.byte	0x3
	.long	0x439
	.uleb128 0x9
	.byte	0x8
	.long	0x31
	.uleb128 0x4
	.long	.LASF797
	.byte	0x8
	.byte	0x35
	.byte	0x3
	.long	0x162
	.uleb128 0xf
	.byte	0x7
	.byte	0x4
	.long	0x6c
	.byte	0xa
	.byte	0x15
	.byte	0x1
	.long	0x4a5
	.uleb128 0x10
	.long	.LASF798
	.byte	0
	.uleb128 0x10
	.long	.LASF799
	.byte	0x1
	.byte	0
	.uleb128 0x4
	.long	.LASF800
	.byte	0xa
	.byte	0x18
	.byte	0x3
	.long	0x48a
	.uleb128 0xd
	.long	0x31
	.long	0x4c1
	.uleb128 0x11
	.long	0x86
	.byte	0xff
	.byte	0
	.uleb128 0xd
	.long	0x4d1
	.long	0x4d1
	.uleb128 0x11
	.long	0x86
	.byte	0x1f
	.byte	0
	.uleb128 0x9
	.byte	0x8
	.long	0x4d7
	.uleb128 0x12
	.long	.LASF801
	.byte	0x1
	.uleb128 0x12
	.long	.LASF802
	.byte	0x1
	.uleb128 0x9
	.byte	0x8
	.long	0x4dd
	.uleb128 0x4
	.long	.LASF803
	.byte	0xa
	.byte	0x49
	.byte	0x3
	.long	0x246
	.uleb128 0x13
	.long	.LASF804
	.byte	0xe
	.byte	0x9
	.byte	0xf
	.long	0x42d
	.byte	0x1
	.byte	0x1
	.uleb128 0x13
	.long	.LASF805
	.byte	0xe
	.byte	0xa
	.byte	0x10
	.long	0x511
	.byte	0x1
	.byte	0x1
	.uleb128 0x9
	.byte	0x8
	.long	0x4e9
	.uleb128 0x13
	.long	.LASF806
	.byte	0xe
	.byte	0xb
	.byte	0x13
	.long	0x525
	.byte	0x1
	.byte	0x1
	.uleb128 0x9
	.byte	0x8
	.long	0x47e
	.uleb128 0x13
	.long	.LASF807
	.byte	0x2
	.byte	0x8c
	.byte	0x11
	.long	0x7a
	.byte	0x1
	.byte	0x1
	.uleb128 0x14
	.byte	0x1
	.long	.LASF813
	.byte	0x6
	.byte	0x76
	.byte	0x6
	.byte	0x1
	.long	0x73
	.byte	0x1
	.long	0x552
	.uleb128 0x15
	.long	0x427
	.byte	0
	.uleb128 0x16
	.byte	0x1
	.long	.LASF808
	.byte	0x5
	.byte	0xa3
	.byte	0x6
	.byte	0x1
	.byte	0x1
	.long	0x567
	.uleb128 0x15
	.long	0x567
	.byte	0
	.uleb128 0x9
	.byte	0x8
	.long	0xed
	.uleb128 0x16
	.byte	0x1
	.long	.LASF809
	.byte	0x6
	.byte	0x59
	.byte	0x6
	.byte	0x1
	.byte	0x1
	.long	0x587
	.uleb128 0x15
	.long	0x427
	.uleb128 0x15
	.long	0x587
	.byte	0
	.uleb128 0x9
	.byte	0x8
	.long	0x20c
	.uleb128 0x16
	.byte	0x1
	.long	.LASF810
	.byte	0x5
	.byte	0x9c
	.byte	0x6
	.byte	0x1
	.byte	0x1
	.long	0x5a7
	.uleb128 0x15
	.long	0x5a7
	.uleb128 0x15
	.long	0x567
	.byte	0
	.uleb128 0x9
	.byte	0x8
	.long	0xe1
	.uleb128 0x16
	.byte	0x1
	.long	.LASF811
	.byte	0x6
	.byte	0x46
	.byte	0x6
	.byte	0x1
	.byte	0x1
	.long	0x5c2
	.uleb128 0x15
	.long	0x427
	.byte	0
	.uleb128 0x16
	.byte	0x1
	.long	.LASF812
	.byte	0x2
	.byte	0x9b
	.byte	0x6
	.byte	0x1
	.byte	0x1
	.long	0x5d8
	.uleb128 0x15
	.long	0x478
	.uleb128 0x17
	.byte	0
	.uleb128 0x14
	.byte	0x1
	.long	.LASF814
	.byte	0x2
	.byte	0x9f
	.byte	0xd
	.byte	0x1
	.long	0x5f1
	.byte	0x1
	.long	0x5f1
	.uleb128 0x15
	.long	0x7a
	.byte	0
	.uleb128 0x9
	.byte	0x8
	.long	0x38
	.uleb128 0x16
	.byte	0x1
	.long	.LASF815
	.byte	0x5
	.byte	0x67
	.byte	0x6
	.byte	0x1
	.byte	0x1
	.long	0x60c
	.uleb128 0x15
	.long	0x567
	.byte	0
	.uleb128 0x16
	.byte	0x1
	.long	.LASF816
	.byte	0x6
	.byte	0x6e
	.byte	0x6
	.byte	0x1
	.byte	0x1
	.long	0x621
	.uleb128 0x15
	.long	0x427
	.byte	0
	.uleb128 0x18
	.byte	0x1
	.long	.LASF817
	.byte	0x2
	.byte	0xfd
	.byte	0xf
	.byte	0x1
	.byte	0x1
	.byte	0x1
	.long	0x647
	.uleb128 0x15
	.long	0x5f1
	.uleb128 0x15
	.long	0x65
	.uleb128 0x15
	.long	0x5f1
	.uleb128 0x15
	.long	0x5f1
	.uleb128 0x17
	.byte	0
	.uleb128 0x19
	.byte	0x1
	.long	.LASF818
	.byte	0x1
	.byte	0x73
	.byte	0xd
	.byte	0x1
	.long	0x73
	.byte	0x1
	.long	0x667
	.uleb128 0x1a
	.string	"mtx"
	.byte	0x1
	.byte	0x73
	.byte	0x29
	.long	0x667
	.byte	0
	.uleb128 0x9
	.byte	0x8
	.long	0x212
	.uleb128 0x1b
	.byte	0x1
	.long	.LASF841
	.byte	0x1
	.byte	0x6a
	.byte	0x6
	.byte	0x1
	.long	0x73
	.quad	.LFB14
	.quad	.LFE14
	.byte	0x1
	.byte	0x9c
	.byte	0x1
	.long	0x6af
	.uleb128 0x1c
	.string	"mtx"
	.byte	0x1
	.byte	0x6a
	.byte	0x23
	.long	0x667
	.byte	0x2
	.byte	0x91
	.sleb128 -24
	.uleb128 0x1d
	.quad	.LVL26
	.long	0x539
	.byte	0
	.uleb128 0x1e
	.byte	0x1
	.long	.LASF819
	.byte	0x1
	.byte	0x58
	.byte	0x6
	.byte	0x1
	.quad	.LFB13
	.quad	.LFE13
	.byte	0x1
	.byte	0x9c
	.byte	0x1
	.long	0x7a8
	.uleb128 0x1c
	.string	"mtx"
	.byte	0x1
	.byte	0x58
	.byte	0x1e
	.long	0x667
	.byte	0x2
	.byte	0x91
	.sleb128 -40
	.uleb128 0x1f
	.long	.LASF821
	.long	0x7b8
	.byte	0x1
	.byte	0x9
	.byte	0x3
	.quad	__func__.0
	.uleb128 0x20
	.long	0x647
	.quad	.LBB14
	.quad	.LBE14
	.byte	0x1
	.byte	0x5f
	.byte	0x5
	.long	0x718
	.uleb128 0x21
	.long	0x65a
	.byte	0x2
	.byte	0x91
	.sleb128 -24
	.byte	0
	.uleb128 0x1d
	.quad	.LVL15
	.long	0x5d8
	.uleb128 0x1d
	.quad	.LVL16
	.long	0x5c2
	.uleb128 0x1d
	.quad	.LVL17
	.long	0x5c2
	.uleb128 0x1d
	.quad	.LVL18
	.long	0x5c2
	.uleb128 0x1d
	.quad	.LVL19
	.long	0x5c2
	.uleb128 0x1d
	.quad	.LVL20
	.long	0x5c2
	.uleb128 0x1d
	.quad	.LVL21
	.long	0x621
	.uleb128 0x1d
	.quad	.LVL22
	.long	0x56d
	.uleb128 0x1d
	.quad	.LVL23
	.long	0x621
	.uleb128 0x1d
	.quad	.LVL24
	.long	0x552
	.uleb128 0x1d
	.quad	.LVL25
	.long	0x58d
	.byte	0
	.uleb128 0xd
	.long	0x38
	.long	0x7b8
	.uleb128 0x11
	.long	0x86
	.byte	0xd
	.byte	0
	.uleb128 0x3
	.long	0x7a8
	.uleb128 0x1e
	.byte	0x1
	.long	.LASF820
	.byte	0x1
	.byte	0x39
	.byte	0x6
	.byte	0x1
	.quad	.LFB12
	.quad	.LFE12
	.byte	0x1
	.byte	0x9c
	.byte	0x1
	.long	0x8e8
	.uleb128 0x1c
	.string	"mtx"
	.byte	0x1
	.byte	0x39
	.byte	0x1c
	.long	0x667
	.byte	0x2
	.byte	0x91
	.sleb128 -40
	.uleb128 0x1f
	.long	.LASF821
	.long	0x8f8
	.byte	0x1
	.byte	0x9
	.byte	0x3
	.quad	__func__.1
	.uleb128 0x20
	.long	0x647
	.quad	.LBB10
	.quad	.LBE10
	.byte	0x1
	.byte	0x3f
	.byte	0x5
	.long	0x826
	.uleb128 0x21
	.long	0x65a
	.byte	0x2
	.byte	0x91
	.sleb128 -24
	.byte	0
	.uleb128 0x20
	.long	0x647
	.quad	.LBB12
	.quad	.LBE12
	.byte	0x1
	.byte	0x45
	.byte	0x9
	.long	0x84b
	.uleb128 0x21
	.long	0x65a
	.byte	0x2
	.byte	0x91
	.sleb128 -32
	.byte	0
	.uleb128 0x1d
	.quad	.LVL3
	.long	0x5d8
	.uleb128 0x1d
	.quad	.LVL4
	.long	0x5c2
	.uleb128 0x1d
	.quad	.LVL5
	.long	0x5c2
	.uleb128 0x1d
	.quad	.LVL6
	.long	0x5c2
	.uleb128 0x1d
	.quad	.LVL7
	.long	0x5c2
	.uleb128 0x1d
	.quad	.LVL8
	.long	0x5c2
	.uleb128 0x1d
	.quad	.LVL9
	.long	0x621
	.uleb128 0x1d
	.quad	.LVL10
	.long	0x621
	.uleb128 0x1d
	.quad	.LVL11
	.long	0x948
	.uleb128 0x1d
	.quad	.LVL12
	.long	0x5ad
	.uleb128 0x1d
	.quad	.LVL13
	.long	0x621
	.uleb128 0x1d
	.quad	.LVL14
	.long	0x58d
	.byte	0
	.uleb128 0xd
	.long	0x38
	.long	0x8f8
	.uleb128 0x11
	.long	0x86
	.byte	0xb
	.byte	0
	.uleb128 0x3
	.long	0x8e8
	.uleb128 0x1e
	.byte	0x1
	.long	.LASF822
	.byte	0x1
	.byte	0x29
	.byte	0x6
	.byte	0x1
	.quad	.LFB11
	.quad	.LFE11
	.byte	0x1
	.byte	0x9c
	.byte	0x1
	.long	0x948
	.uleb128 0x1c
	.string	"mtx"
	.byte	0x1
	.byte	0x29
	.byte	0x1c
	.long	0x667
	.byte	0x2
	.byte	0x91
	.sleb128 -24
	.uleb128 0x1d
	.quad	.LVL1
	.long	0x60c
	.uleb128 0x1d
	.quad	.LVL2
	.long	0x5f7
	.byte	0
	.uleb128 0x1e
	.byte	0x1
	.long	.LASF823
	.byte	0x1
	.byte	0x13
	.byte	0x6
	.byte	0x1
	.quad	.LFB10
	.quad	.LFE10
	.byte	0x1
	.byte	0x9c
	.byte	0x1
	.long	0x9fa
	.uleb128 0x1c
	.string	"mtx"
	.byte	0x1
	.byte	0x13
	.byte	0x21
	.long	0x667
	.byte	0x2
	.byte	0x91
	.sleb128 -56
	.uleb128 0x1f
	.long	.LASF821
	.long	0xa0a
	.byte	0x1
	.byte	0x9
	.byte	0x3
	.quad	__func__.2
	.uleb128 0x22
	.quad	.LBB8
	.quad	.LBE8
	.uleb128 0x23
	.long	.LASF824
	.byte	0x1
	.byte	0x16
	.byte	0x5
	.long	0x667
	.byte	0x2
	.byte	0x91
	.sleb128 -24
	.uleb128 0x23
	.long	.LASF825
	.byte	0x1
	.byte	0x16
	.byte	0x5
	.long	0x667
	.byte	0x2
	.byte	0x91
	.sleb128 -32
	.uleb128 0x22
	.quad	.LBB9
	.quad	.LBE9
	.uleb128 0x23
	.long	.LASF826
	.byte	0x1
	.byte	0x18
	.byte	0x9
	.long	0x525
	.byte	0x2
	.byte	0x91
	.sleb128 -40
	.uleb128 0x23
	.long	.LASF827
	.byte	0x1
	.byte	0x18
	.byte	0x9
	.long	0x525
	.byte	0x2
	.byte	0x91
	.sleb128 -48
	.uleb128 0x1d
	.quad	.LVL0
	.long	0x621
	.byte	0
	.byte	0
	.byte	0
	.uleb128 0xd
	.long	0x38
	.long	0xa0a
	.uleb128 0x11
	.long	0x86
	.byte	0x10
	.byte	0
	.uleb128 0x3
	.long	0x9fa
	.uleb128 0x24
	.long	.LASF828
	.byte	0x3
	.byte	0x61
	.byte	0x14
	.quad	.LFB9
	.quad	.LFE9
	.byte	0x1
	.byte	0x9c
	.byte	0x1
	.uleb128 0x24
	.long	.LASF829
	.byte	0x3
	.byte	0x5b
	.byte	0x14
	.quad	.LFB8
	.quad	.LFE8
	.byte	0x1
	.byte	0x9c
	.byte	0x1
	.uleb128 0x24
	.long	.LASF830
	.byte	0x3
	.byte	0x59
	.byte	0x14
	.quad	.LFB7
	.quad	.LFE7
	.byte	0x1
	.byte	0x9c
	.byte	0x1
	.uleb128 0x25
	.long	.LASF842
	.byte	0x3
	.byte	0x51
	.byte	0x18
	.long	0x7a
	.quad	.LFB6
	.quad	.LFE6
	.byte	0x1
	.byte	0x9c
	.byte	0x1
	.long	0xa93
	.uleb128 0x23
	.long	.LASF831
	.byte	0x3
	.byte	0x53
	.byte	0xe
	.long	0x7a
	.byte	0x2
	.byte	0x91
	.sleb128 -24
	.byte	0
	.uleb128 0x26
	.long	.LASF832
	.byte	0x2
	.value	0x124
	.byte	0xd
	.byte	0x1
	.long	0x73
	.quad	.LFB5
	.quad	.LFE5
	.byte	0x1
	.byte	0x9c
	.byte	0x1
	.long	0xad5
	.uleb128 0x27
	.string	"l"
	.byte	0x2
	.value	0x124
	.byte	0x20
	.long	0x73
	.byte	0x2
	.byte	0x91
	.sleb128 -24
	.uleb128 0x27
	.string	"r"
	.byte	0x2
	.value	0x124
	.byte	0x28
	.long	0x73
	.byte	0x2
	.byte	0x91
	.sleb128 -32
	.byte	0
	.uleb128 0x26
	.long	.LASF833
	.byte	0x2
	.value	0x122
	.byte	0xd
	.byte	0x1
	.long	0x73
	.quad	.LFB4
	.quad	.LFE4
	.byte	0x1
	.byte	0x9c
	.byte	0x1
	.long	0xb17
	.uleb128 0x27
	.string	"l"
	.byte	0x2
	.value	0x122
	.byte	0x1d
	.long	0x73
	.byte	0x2
	.byte	0x91
	.sleb128 -24
	.uleb128 0x27
	.string	"r"
	.byte	0x2
	.value	0x122
	.byte	0x25
	.long	0x73
	.byte	0x2
	.byte	0x91
	.sleb128 -32
	.byte	0
	.uleb128 0x26
	.long	.LASF834
	.byte	0x2
	.value	0x120
	.byte	0xd
	.byte	0x1
	.long	0x73
	.quad	.LFB3
	.quad	.LFE3
	.byte	0x1
	.byte	0x9c
	.byte	0x1
	.long	0xb59
	.uleb128 0x27
	.string	"l"
	.byte	0x2
	.value	0x120
	.byte	0x1e
	.long	0x73
	.byte	0x2
	.byte	0x91
	.sleb128 -24
	.uleb128 0x27
	.string	"r"
	.byte	0x2
	.value	0x120
	.byte	0x26
	.long	0x73
	.byte	0x2
	.byte	0x91
	.sleb128 -32
	.byte	0
	.uleb128 0x26
	.long	.LASF835
	.byte	0x2
	.value	0x11e
	.byte	0xd
	.byte	0x1
	.long	0x73
	.quad	.LFB2
	.quad	.LFE2
	.byte	0x1
	.byte	0x9c
	.byte	0x1
	.long	0xb9b
	.uleb128 0x27
	.string	"l"
	.byte	0x2
	.value	0x11e
	.byte	0x1b
	.long	0x73
	.byte	0x2
	.byte	0x91
	.sleb128 -24
	.uleb128 0x27
	.string	"r"
	.byte	0x2
	.value	0x11e
	.byte	0x23
	.long	0x73
	.byte	0x2
	.byte	0x91
	.sleb128 -32
	.byte	0
	.uleb128 0x26
	.long	.LASF836
	.byte	0x2
	.value	0x11c
	.byte	0xd
	.byte	0x1
	.long	0x73
	.quad	.LFB1
	.quad	.LFE1
	.byte	0x1
	.byte	0x9c
	.byte	0x1
	.long	0xbdd
	.uleb128 0x27
	.string	"l"
	.byte	0x2
	.value	0x11c
	.byte	0x1c
	.long	0x73
	.byte	0x2
	.byte	0x91
	.sleb128 -24
	.uleb128 0x27
	.string	"r"
	.byte	0x2
	.value	0x11c
	.byte	0x24
	.long	0x73
	.byte	0x2
	.byte	0x91
	.sleb128 -32
	.byte	0
	.uleb128 0x26
	.long	.LASF837
	.byte	0x2
	.value	0x117
	.byte	0xd
	.byte	0x1
	.long	0x73
	.quad	.LFB0
	.quad	.LFE0
	.byte	0x1
	.byte	0x9c
	.byte	0x1
	.long	0xc1f
	.uleb128 0x27
	.string	"l"
	.byte	0x2
	.value	0x117
	.byte	0x19
	.long	0x73
	.byte	0x2
	.byte	0x91
	.sleb128 -24
	.uleb128 0x27
	.string	"r"
	.byte	0x2
	.value	0x117
	.byte	0x21
	.long	0x73
	.byte	0x2
	.byte	0x91
	.sleb128 -32
	.byte	0
	.uleb128 0x28
	.long	0x647
	.quad	.LFB15
	.quad	.LFE15
	.byte	0x1
	.byte	0x9c
	.byte	0x1
	.uleb128 0x21
	.long	0x65a
	.byte	0x2
	.byte	0x91
	.sleb128 -24
	.byte	0
	.byte	0
	.section	.debug_abbrev,"",@progbits
.Ldebug_abbrev0:
	.uleb128 0x1
	.uleb128 0x11
	.byte	0x1
	.uleb128 0x25
	.uleb128 0xe
	.uleb128 0x13
	.uleb128 0xb
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x1b
	.uleb128 0xe
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x1
	.uleb128 0x10
	.uleb128 0x6
	.uleb128 0x2119
	.uleb128 0x6
	.byte	0
	.byte	0
	.uleb128 0x2
	.uleb128 0x24
	.byte	0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3e
	.uleb128 0xb
	.uleb128 0x3
	.uleb128 0xe
	.byte	0
	.byte	0
	.uleb128 0x3
	.uleb128 0x26
	.byte	0
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x4
	.uleb128 0x16
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x5
	.uleb128 0x24
	.byte	0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3e
	.uleb128 0xb
	.uleb128 0x3
	.uleb128 0x8
	.byte	0
	.byte	0
	.uleb128 0x6
	.uleb128 0xf
	.byte	0
	.uleb128 0xb
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0x7
	.uleb128 0x13
	.byte	0x1
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x8
	.uleb128 0xd
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x38
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0x9
	.uleb128 0xf
	.byte	0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0xa
	.uleb128 0x13
	.byte	0x1
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0xb
	.uleb128 0x5
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0xb
	.uleb128 0xd
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x38
	.uleb128 0x5
	.byte	0
	.byte	0
	.uleb128 0xc
	.uleb128 0x13
	.byte	0x1
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0xb
	.uleb128 0x5
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0xd
	.uleb128 0x1
	.byte	0x1
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0xe
	.uleb128 0x21
	.byte	0
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2f
	.uleb128 0x5
	.byte	0
	.byte	0
	.uleb128 0xf
	.uleb128 0x4
	.byte	0x1
	.uleb128 0x3e
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x10
	.uleb128 0x28
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x1c
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0x11
	.uleb128 0x21
	.byte	0
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2f
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0x12
	.uleb128 0x13
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3c
	.uleb128 0xc
	.byte	0
	.byte	0
	.uleb128 0x13
	.uleb128 0x34
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x3f
	.uleb128 0xc
	.uleb128 0x3c
	.uleb128 0xc
	.byte	0
	.byte	0
	.uleb128 0x14
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3f
	.uleb128 0xc
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x27
	.uleb128 0xc
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x3c
	.uleb128 0xc
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x15
	.uleb128 0x5
	.byte	0
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x16
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3f
	.uleb128 0xc
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x27
	.uleb128 0xc
	.uleb128 0x3c
	.uleb128 0xc
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x17
	.uleb128 0x18
	.byte	0
	.byte	0
	.byte	0
	.uleb128 0x18
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3f
	.uleb128 0xc
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x27
	.uleb128 0xc
	.uleb128 0x87
	.uleb128 0xc
	.uleb128 0x3c
	.uleb128 0xc
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x19
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3f
	.uleb128 0xc
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x27
	.uleb128 0xc
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x20
	.uleb128 0xb
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x1a
	.uleb128 0x5
	.byte	0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x1b
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3f
	.uleb128 0xc
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x27
	.uleb128 0xc
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x1
	.uleb128 0x40
	.uleb128 0xa
	.uleb128 0x2117
	.uleb128 0xc
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x1c
	.uleb128 0x5
	.byte	0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0xa
	.byte	0
	.byte	0
	.uleb128 0x1d
	.uleb128 0x4109
	.byte	0
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x31
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x1e
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3f
	.uleb128 0xc
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x27
	.uleb128 0xc
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x1
	.uleb128 0x40
	.uleb128 0xa
	.uleb128 0x2117
	.uleb128 0xc
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x1f
	.uleb128 0x34
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x34
	.uleb128 0xc
	.uleb128 0x2
	.uleb128 0xa
	.byte	0
	.byte	0
	.uleb128 0x20
	.uleb128 0x1d
	.byte	0x1
	.uleb128 0x31
	.uleb128 0x13
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x1
	.uleb128 0x58
	.uleb128 0xb
	.uleb128 0x59
	.uleb128 0xb
	.uleb128 0x57
	.uleb128 0xb
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x21
	.uleb128 0x5
	.byte	0
	.uleb128 0x31
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0xa
	.byte	0
	.byte	0
	.uleb128 0x22
	.uleb128 0xb
	.byte	0x1
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x1
	.byte	0
	.byte	0
	.uleb128 0x23
	.uleb128 0x34
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0xa
	.byte	0
	.byte	0
	.uleb128 0x24
	.uleb128 0x2e
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x1
	.uleb128 0x40
	.uleb128 0xa
	.uleb128 0x2117
	.uleb128 0xc
	.byte	0
	.byte	0
	.uleb128 0x25
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x1
	.uleb128 0x40
	.uleb128 0xa
	.uleb128 0x2117
	.uleb128 0xc
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x26
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0x5
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x27
	.uleb128 0xc
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x1
	.uleb128 0x40
	.uleb128 0xa
	.uleb128 0x2117
	.uleb128 0xc
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x27
	.uleb128 0x5
	.byte	0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0x5
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0xa
	.byte	0
	.byte	0
	.uleb128 0x28
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x31
	.uleb128 0x13
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x1
	.uleb128 0x40
	.uleb128 0xa
	.uleb128 0x2117
	.uleb128 0xc
	.byte	0
	.byte	0
	.byte	0
	.section	.debug_aranges,"",@progbits
	.long	0x2c
	.value	0x2
	.long	.Ldebug_info0
	.byte	0x8
	.byte	0
	.value	0
	.value	0
	.quad	.Ltext0
	.quad	.Letext0-.Ltext0
	.quad	0
	.quad	0
	.section	.debug_macro,"",@progbits
.Ldebug_macro0:
	.value	0x4
	.byte	0x2
	.long	.Ldebug_line0
	.byte	0x7
	.long	.Ldebug_macro2
	.byte	0x3
	.uleb128 0
	.uleb128 0x1
	.byte	0x3
	.uleb128 0x2
	.uleb128 0x7
	.byte	0x3
	.uleb128 0x3
	.uleb128 0x6
	.file 15 "include/proc/spinlock.h"
	.byte	0x3
	.uleb128 0x3
	.uleb128 0xf
	.byte	0x5
	.uleb128 0x8
	.long	.LASF379
	.byte	0x4
	.byte	0x3
	.uleb128 0x4
	.uleb128 0x5
	.file 16 "include/kernel.h"
	.byte	0x3
	.uleb128 0x3
	.uleb128 0x10
	.byte	0x3
	.uleb128 0x3
	.uleb128 0x4
	.byte	0x7
	.long	.Ldebug_macro3
	.byte	0x4
	.byte	0x7
	.long	.Ldebug_macro4
	.byte	0x4
	.byte	0x7
	.long	.Ldebug_macro5
	.byte	0x4
	.byte	0x5
	.uleb128 0x19
	.long	.LASF401
	.byte	0x4
	.byte	0x5
	.uleb128 0x11
	.long	.LASF402
	.byte	0x4
	.byte	0x3
	.uleb128 0x3
	.uleb128 0xe
	.file 17 "include/main/smp.h"
	.byte	0x3
	.uleb128 0x3
	.uleb128 0x11
	.file 18 "include/boot/config.h"
	.byte	0x3
	.uleb128 0x1
	.uleb128 0x12
	.byte	0x7
	.long	.Ldebug_macro6
	.byte	0x4
	.file 19 "include/mm/page.h"
	.byte	0x3
	.uleb128 0x2
	.uleb128 0x13
	.byte	0x7
	.long	.Ldebug_macro7
	.byte	0x4
	.byte	0x3
	.uleb128 0x3
	.uleb128 0xd
	.byte	0x3
	.uleb128 0x3
	.uleb128 0xc
	.byte	0x3
	.uleb128 0x5
	.uleb128 0xb
	.byte	0x3
	.uleb128 0x4
	.uleb128 0x9
	.byte	0x7
	.long	.Ldebug_macro8
	.byte	0x4
	.byte	0x7
	.long	.Ldebug_macro9
	.byte	0x4
	.byte	0x4
	.byte	0x4
	.byte	0x5
	.uleb128 0xb
	.long	.LASF475
	.byte	0x4
	.byte	0x3
	.uleb128 0x4
	.uleb128 0x8
	.byte	0x4
	.byte	0x3
	.uleb128 0x5
	.uleb128 0xa
	.file 20 "include/config.h"
	.byte	0x3
	.uleb128 0x3
	.uleb128 0x14
	.byte	0x7
	.long	.Ldebug_macro10
	.byte	0x4
	.byte	0x7
	.long	.Ldebug_macro11
	.byte	0x4
	.byte	0x5
	.uleb128 0x7
	.long	.LASF493
	.byte	0x4
	.byte	0x3
	.uleb128 0x4
	.uleb128 0x3
	.byte	0x3
	.uleb128 0x5
	.uleb128 0x2
	.file 21 "include/main/apic.h"
	.byte	0x3
	.uleb128 0x8
	.uleb128 0x15
	.byte	0x5
	.uleb128 0x10
	.long	.LASF494
	.byte	0x4
	.byte	0x7
	.long	.Ldebug_macro12
	.byte	0x4
	.byte	0x7
	.long	.Ldebug_macro13
	.byte	0x4
	.file 22 "include/errno.h"
	.byte	0x3
	.uleb128 0x5
	.uleb128 0x16
	.byte	0x7
	.long	.Ldebug_macro14
	.byte	0x4
	.byte	0x5
	.uleb128 0x12
	.long	.LASF713
	.byte	0x4
	.byte	0
	.section	.debug_macro,"G",@progbits,wm4.0.2884a3d6fb35dc0140d9187cc097dcf8,comdat
.Ldebug_macro2:
	.value	0x4
	.byte	0
	.byte	0x5
	.uleb128 0
	.long	.LASF0
	.byte	0x5
	.uleb128 0
	.long	.LASF1
	.byte	0x5
	.uleb128 0
	.long	.LASF2
	.byte	0x5
	.uleb128 0
	.long	.LASF3
	.byte	0x5
	.uleb128 0
	.long	.LASF4
	.byte	0x5
	.uleb128 0
	.long	.LASF5
	.byte	0x5
	.uleb128 0
	.long	.LASF6
	.byte	0x5
	.uleb128 0
	.long	.LASF7
	.byte	0x5
	.uleb128 0
	.long	.LASF8
	.byte	0x5
	.uleb128 0
	.long	.LASF9
	.byte	0x5
	.uleb128 0
	.long	.LASF10
	.byte	0x5
	.uleb128 0
	.long	.LASF11
	.byte	0x5
	.uleb128 0
	.long	.LASF12
	.byte	0x5
	.uleb128 0
	.long	.LASF13
	.byte	0x5
	.uleb128 0
	.long	.LASF14
	.byte	0x5
	.uleb128 0
	.long	.LASF15
	.byte	0x5
	.uleb128 0
	.long	.LASF16
	.byte	0x5
	.uleb128 0
	.long	.LASF17
	.byte	0x5
	.uleb128 0
	.long	.LASF18
	.byte	0x5
	.uleb128 0
	.long	.LASF19
	.byte	0x5
	.uleb128 0
	.long	.LASF20
	.byte	0x5
	.uleb128 0
	.long	.LASF21
	.byte	0x5
	.uleb128 0
	.long	.LASF22
	.byte	0x5
	.uleb128 0
	.long	.LASF23
	.byte	0x5
	.uleb128 0
	.long	.LASF24
	.byte	0x5
	.uleb128 0
	.long	.LASF25
	.byte	0x5
	.uleb128 0
	.long	.LASF26
	.byte	0x5
	.uleb128 0
	.long	.LASF27
	.byte	0x5
	.uleb128 0
	.long	.LASF28
	.byte	0x5
	.uleb128 0
	.long	.LASF29
	.byte	0x5
	.uleb128 0
	.long	.LASF30
	.byte	0x5
	.uleb128 0
	.long	.LASF31
	.byte	0x5
	.uleb128 0
	.long	.LASF32
	.byte	0x5
	.uleb128 0
	.long	.LASF33
	.byte	0x5
	.uleb128 0
	.long	.LASF34
	.byte	0x5
	.uleb128 0
	.long	.LASF35
	.byte	0x5
	.uleb128 0
	.long	.LASF36
	.byte	0x5
	.uleb128 0
	.long	.LASF37
	.byte	0x5
	.uleb128 0
	.long	.LASF38
	.byte	0x5
	.uleb128 0
	.long	.LASF39
	.byte	0x5
	.uleb128 0
	.long	.LASF40
	.byte	0x5
	.uleb128 0
	.long	.LASF41
	.byte	0x5
	.uleb128 0
	.long	.LASF42
	.byte	0x5
	.uleb128 0
	.long	.LASF43
	.byte	0x5
	.uleb128 0
	.long	.LASF44
	.byte	0x5
	.uleb128 0
	.long	.LASF45
	.byte	0x5
	.uleb128 0
	.long	.LASF46
	.byte	0x5
	.uleb128 0
	.long	.LASF47
	.byte	0x5
	.uleb128 0
	.long	.LASF48
	.byte	0x5
	.uleb128 0
	.long	.LASF49
	.byte	0x5
	.uleb128 0
	.long	.LASF50
	.byte	0x5
	.uleb128 0
	.long	.LASF51
	.byte	0x5
	.uleb128 0
	.long	.LASF52
	.byte	0x5
	.uleb128 0
	.long	.LASF53
	.byte	0x5
	.uleb128 0
	.long	.LASF54
	.byte	0x5
	.uleb128 0
	.long	.LASF55
	.byte	0x5
	.uleb128 0
	.long	.LASF56
	.byte	0x5
	.uleb128 0
	.long	.LASF57
	.byte	0x5
	.uleb128 0
	.long	.LASF58
	.byte	0x5
	.uleb128 0
	.long	.LASF59
	.byte	0x5
	.uleb128 0
	.long	.LASF60
	.byte	0x5
	.uleb128 0
	.long	.LASF61
	.byte	0x5
	.uleb128 0
	.long	.LASF62
	.byte	0x5
	.uleb128 0
	.long	.LASF63
	.byte	0x5
	.uleb128 0
	.long	.LASF64
	.byte	0x5
	.uleb128 0
	.long	.LASF65
	.byte	0x5
	.uleb128 0
	.long	.LASF66
	.byte	0x5
	.uleb128 0
	.long	.LASF67
	.byte	0x5
	.uleb128 0
	.long	.LASF68
	.byte	0x5
	.uleb128 0
	.long	.LASF69
	.byte	0x5
	.uleb128 0
	.long	.LASF70
	.byte	0x5
	.uleb128 0
	.long	.LASF71
	.byte	0x5
	.uleb128 0
	.long	.LASF72
	.byte	0x5
	.uleb128 0
	.long	.LASF73
	.byte	0x5
	.uleb128 0
	.long	.LASF74
	.byte	0x5
	.uleb128 0
	.long	.LASF75
	.byte	0x5
	.uleb128 0
	.long	.LASF76
	.byte	0x5
	.uleb128 0
	.long	.LASF77
	.byte	0x5
	.uleb128 0
	.long	.LASF78
	.byte	0x5
	.uleb128 0
	.long	.LASF79
	.byte	0x5
	.uleb128 0
	.long	.LASF80
	.byte	0x5
	.uleb128 0
	.long	.LASF81
	.byte	0x5
	.uleb128 0
	.long	.LASF82
	.byte	0x5
	.uleb128 0
	.long	.LASF83
	.byte	0x5
	.uleb128 0
	.long	.LASF84
	.byte	0x5
	.uleb128 0
	.long	.LASF85
	.byte	0x5
	.uleb128 0
	.long	.LASF86
	.byte	0x5
	.uleb128 0
	.long	.LASF87
	.byte	0x5
	.uleb128 0
	.long	.LASF88
	.byte	0x5
	.uleb128 0
	.long	.LASF89
	.byte	0x5
	.uleb128 0
	.long	.LASF90
	.byte	0x5
	.uleb128 0
	.long	.LASF91
	.byte	0x5
	.uleb128 0
	.long	.LASF92
	.byte	0x5
	.uleb128 0
	.long	.LASF93
	.byte	0x5
	.uleb128 0
	.long	.LASF94
	.byte	0x5
	.uleb128 0
	.long	.LASF95
	.byte	0x5
	.uleb128 0
	.long	.LASF96
	.byte	0x5
	.uleb128 0
	.long	.LASF97
	.byte	0x5
	.uleb128 0
	.long	.LASF98
	.byte	0x5
	.uleb128 0
	.long	.LASF99
	.byte	0x5
	.uleb128 0
	.long	.LASF100
	.byte	0x5
	.uleb128 0
	.long	.LASF101
	.byte	0x5
	.uleb128 0
	.long	.LASF102
	.byte	0x5
	.uleb128 0
	.long	.LASF103
	.byte	0x5
	.uleb128 0
	.long	.LASF104
	.byte	0x5
	.uleb128 0
	.long	.LASF105
	.byte	0x5
	.uleb128 0
	.long	.LASF106
	.byte	0x5
	.uleb128 0
	.long	.LASF107
	.byte	0x5
	.uleb128 0
	.long	.LASF108
	.byte	0x5
	.uleb128 0
	.long	.LASF109
	.byte	0x5
	.uleb128 0
	.long	.LASF110
	.byte	0x5
	.uleb128 0
	.long	.LASF111
	.byte	0x5
	.uleb128 0
	.long	.LASF112
	.byte	0x5
	.uleb128 0
	.long	.LASF113
	.byte	0x5
	.uleb128 0
	.long	.LASF114
	.byte	0x5
	.uleb128 0
	.long	.LASF115
	.byte	0x5
	.uleb128 0
	.long	.LASF116
	.byte	0x5
	.uleb128 0
	.long	.LASF117
	.byte	0x5
	.uleb128 0
	.long	.LASF118
	.byte	0x5
	.uleb128 0
	.long	.LASF119
	.byte	0x5
	.uleb128 0
	.long	.LASF120
	.byte	0x5
	.uleb128 0
	.long	.LASF121
	.byte	0x5
	.uleb128 0
	.long	.LASF122
	.byte	0x5
	.uleb128 0
	.long	.LASF123
	.byte	0x5
	.uleb128 0
	.long	.LASF124
	.byte	0x5
	.uleb128 0
	.long	.LASF125
	.byte	0x5
	.uleb128 0
	.long	.LASF126
	.byte	0x5
	.uleb128 0
	.long	.LASF127
	.byte	0x5
	.uleb128 0
	.long	.LASF128
	.byte	0x5
	.uleb128 0
	.long	.LASF129
	.byte	0x5
	.uleb128 0
	.long	.LASF130
	.byte	0x5
	.uleb128 0
	.long	.LASF131
	.byte	0x5
	.uleb128 0
	.long	.LASF132
	.byte	0x5
	.uleb128 0
	.long	.LASF133
	.byte	0x5
	.uleb128 0
	.long	.LASF134
	.byte	0x5
	.uleb128 0
	.long	.LASF135
	.byte	0x5
	.uleb128 0
	.long	.LASF136
	.byte	0x5
	.uleb128 0
	.long	.LASF137
	.byte	0x5
	.uleb128 0
	.long	.LASF138
	.byte	0x5
	.uleb128 0
	.long	.LASF139
	.byte	0x5
	.uleb128 0
	.long	.LASF140
	.byte	0x5
	.uleb128 0
	.long	.LASF141
	.byte	0x5
	.uleb128 0
	.long	.LASF142
	.byte	0x5
	.uleb128 0
	.long	.LASF143
	.byte	0x5
	.uleb128 0
	.long	.LASF144
	.byte	0x5
	.uleb128 0
	.long	.LASF145
	.byte	0x5
	.uleb128 0
	.long	.LASF146
	.byte	0x5
	.uleb128 0
	.long	.LASF147
	.byte	0x5
	.uleb128 0
	.long	.LASF148
	.byte	0x5
	.uleb128 0
	.long	.LASF149
	.byte	0x5
	.uleb128 0
	.long	.LASF150
	.byte	0x5
	.uleb128 0
	.long	.LASF151
	.byte	0x5
	.uleb128 0
	.long	.LASF152
	.byte	0x5
	.uleb128 0
	.long	.LASF153
	.byte	0x5
	.uleb128 0
	.long	.LASF154
	.byte	0x5
	.uleb128 0
	.long	.LASF155
	.byte	0x5
	.uleb128 0
	.long	.LASF156
	.byte	0x5
	.uleb128 0
	.long	.LASF157
	.byte	0x5
	.uleb128 0
	.long	.LASF158
	.byte	0x5
	.uleb128 0
	.long	.LASF159
	.byte	0x5
	.uleb128 0
	.long	.LASF160
	.byte	0x5
	.uleb128 0
	.long	.LASF161
	.byte	0x5
	.uleb128 0
	.long	.LASF162
	.byte	0x5
	.uleb128 0
	.long	.LASF163
	.byte	0x5
	.uleb128 0
	.long	.LASF164
	.byte	0x5
	.uleb128 0
	.long	.LASF165
	.byte	0x5
	.uleb128 0
	.long	.LASF166
	.byte	0x5
	.uleb128 0
	.long	.LASF167
	.byte	0x5
	.uleb128 0
	.long	.LASF168
	.byte	0x5
	.uleb128 0
	.long	.LASF169
	.byte	0x5
	.uleb128 0
	.long	.LASF170
	.byte	0x5
	.uleb128 0
	.long	.LASF171
	.byte	0x5
	.uleb128 0
	.long	.LASF172
	.byte	0x5
	.uleb128 0
	.long	.LASF173
	.byte	0x5
	.uleb128 0
	.long	.LASF174
	.byte	0x5
	.uleb128 0
	.long	.LASF175
	.byte	0x5
	.uleb128 0
	.long	.LASF176
	.byte	0x5
	.uleb128 0
	.long	.LASF177
	.byte	0x5
	.uleb128 0
	.long	.LASF178
	.byte	0x5
	.uleb128 0
	.long	.LASF179
	.byte	0x5
	.uleb128 0
	.long	.LASF180
	.byte	0x5
	.uleb128 0
	.long	.LASF181
	.byte	0x5
	.uleb128 0
	.long	.LASF182
	.byte	0x5
	.uleb128 0
	.long	.LASF183
	.byte	0x5
	.uleb128 0
	.long	.LASF184
	.byte	0x5
	.uleb128 0
	.long	.LASF185
	.byte	0x5
	.uleb128 0
	.long	.LASF186
	.byte	0x5
	.uleb128 0
	.long	.LASF187
	.byte	0x5
	.uleb128 0
	.long	.LASF188
	.byte	0x5
	.uleb128 0
	.long	.LASF189
	.byte	0x5
	.uleb128 0
	.long	.LASF190
	.byte	0x5
	.uleb128 0
	.long	.LASF191
	.byte	0x5
	.uleb128 0
	.long	.LASF192
	.byte	0x5
	.uleb128 0
	.long	.LASF193
	.byte	0x5
	.uleb128 0
	.long	.LASF194
	.byte	0x5
	.uleb128 0
	.long	.LASF195
	.byte	0x5
	.uleb128 0
	.long	.LASF196
	.byte	0x5
	.uleb128 0
	.long	.LASF197
	.byte	0x5
	.uleb128 0
	.long	.LASF198
	.byte	0x5
	.uleb128 0
	.long	.LASF199
	.byte	0x5
	.uleb128 0
	.long	.LASF200
	.byte	0x5
	.uleb128 0
	.long	.LASF201
	.byte	0x5
	.uleb128 0
	.long	.LASF202
	.byte	0x5
	.uleb128 0
	.long	.LASF203
	.byte	0x5
	.uleb128 0
	.long	.LASF204
	.byte	0x5
	.uleb128 0
	.long	.LASF205
	.byte	0x5
	.uleb128 0
	.long	.LASF206
	.byte	0x5
	.uleb128 0
	.long	.LASF207
	.byte	0x5
	.uleb128 0
	.long	.LASF208
	.byte	0x5
	.uleb128 0
	.long	.LASF209
	.byte	0x5
	.uleb128 0
	.long	.LASF210
	.byte	0x5
	.uleb128 0
	.long	.LASF211
	.byte	0x5
	.uleb128 0
	.long	.LASF212
	.byte	0x5
	.uleb128 0
	.long	.LASF213
	.byte	0x5
	.uleb128 0
	.long	.LASF214
	.byte	0x5
	.uleb128 0
	.long	.LASF215
	.byte	0x5
	.uleb128 0
	.long	.LASF216
	.byte	0x5
	.uleb128 0
	.long	.LASF217
	.byte	0x5
	.uleb128 0
	.long	.LASF218
	.byte	0x5
	.uleb128 0
	.long	.LASF219
	.byte	0x5
	.uleb128 0
	.long	.LASF220
	.byte	0x5
	.uleb128 0
	.long	.LASF221
	.byte	0x5
	.uleb128 0
	.long	.LASF222
	.byte	0x5
	.uleb128 0
	.long	.LASF223
	.byte	0x5
	.uleb128 0
	.long	.LASF224
	.byte	0x5
	.uleb128 0
	.long	.LASF225
	.byte	0x5
	.uleb128 0
	.long	.LASF226
	.byte	0x5
	.uleb128 0
	.long	.LASF227
	.byte	0x5
	.uleb128 0
	.long	.LASF228
	.byte	0x5
	.uleb128 0
	.long	.LASF229
	.byte	0x5
	.uleb128 0
	.long	.LASF230
	.byte	0x5
	.uleb128 0
	.long	.LASF231
	.byte	0x5
	.uleb128 0
	.long	.LASF232
	.byte	0x5
	.uleb128 0
	.long	.LASF233
	.byte	0x5
	.uleb128 0
	.long	.LASF234
	.byte	0x5
	.uleb128 0
	.long	.LASF235
	.byte	0x5
	.uleb128 0
	.long	.LASF236
	.byte	0x5
	.uleb128 0
	.long	.LASF237
	.byte	0x5
	.uleb128 0
	.long	.LASF238
	.byte	0x5
	.uleb128 0
	.long	.LASF239
	.byte	0x5
	.uleb128 0
	.long	.LASF240
	.byte	0x5
	.uleb128 0
	.long	.LASF241
	.byte	0x5
	.uleb128 0
	.long	.LASF242
	.byte	0x5
	.uleb128 0
	.long	.LASF243
	.byte	0x5
	.uleb128 0
	.long	.LASF244
	.byte	0x5
	.uleb128 0
	.long	.LASF245
	.byte	0x5
	.uleb128 0
	.long	.LASF246
	.byte	0x5
	.uleb128 0
	.long	.LASF247
	.byte	0x5
	.uleb128 0
	.long	.LASF248
	.byte	0x5
	.uleb128 0
	.long	.LASF249
	.byte	0x5
	.uleb128 0
	.long	.LASF250
	.byte	0x5
	.uleb128 0
	.long	.LASF251
	.byte	0x5
	.uleb128 0
	.long	.LASF252
	.byte	0x5
	.uleb128 0
	.long	.LASF253
	.byte	0x5
	.uleb128 0
	.long	.LASF254
	.byte	0x5
	.uleb128 0
	.long	.LASF255
	.byte	0x5
	.uleb128 0
	.long	.LASF256
	.byte	0x5
	.uleb128 0
	.long	.LASF257
	.byte	0x5
	.uleb128 0
	.long	.LASF258
	.byte	0x5
	.uleb128 0
	.long	.LASF259
	.byte	0x5
	.uleb128 0
	.long	.LASF260
	.byte	0x5
	.uleb128 0
	.long	.LASF261
	.byte	0x5
	.uleb128 0
	.long	.LASF262
	.byte	0x5
	.uleb128 0
	.long	.LASF263
	.byte	0x5
	.uleb128 0
	.long	.LASF264
	.byte	0x5
	.uleb128 0
	.long	.LASF265
	.byte	0x5
	.uleb128 0
	.long	.LASF266
	.byte	0x5
	.uleb128 0
	.long	.LASF267
	.byte	0x5
	.uleb128 0
	.long	.LASF268
	.byte	0x5
	.uleb128 0
	.long	.LASF269
	.byte	0x5
	.uleb128 0
	.long	.LASF270
	.byte	0x5
	.uleb128 0
	.long	.LASF271
	.byte	0x5
	.uleb128 0
	.long	.LASF272
	.byte	0x5
	.uleb128 0
	.long	.LASF273
	.byte	0x5
	.uleb128 0
	.long	.LASF274
	.byte	0x5
	.uleb128 0
	.long	.LASF275
	.byte	0x5
	.uleb128 0
	.long	.LASF276
	.byte	0x5
	.uleb128 0
	.long	.LASF277
	.byte	0x5
	.uleb128 0
	.long	.LASF278
	.byte	0x5
	.uleb128 0
	.long	.LASF279
	.byte	0x5
	.uleb128 0
	.long	.LASF280
	.byte	0x5
	.uleb128 0
	.long	.LASF281
	.byte	0x5
	.uleb128 0
	.long	.LASF282
	.byte	0x5
	.uleb128 0
	.long	.LASF283
	.byte	0x5
	.uleb128 0
	.long	.LASF284
	.byte	0x5
	.uleb128 0
	.long	.LASF285
	.byte	0x5
	.uleb128 0
	.long	.LASF286
	.byte	0x5
	.uleb128 0
	.long	.LASF287
	.byte	0x5
	.uleb128 0
	.long	.LASF288
	.byte	0x5
	.uleb128 0
	.long	.LASF289
	.byte	0x5
	.uleb128 0
	.long	.LASF290
	.byte	0x5
	.uleb128 0
	.long	.LASF291
	.byte	0x5
	.uleb128 0
	.long	.LASF292
	.byte	0x5
	.uleb128 0
	.long	.LASF293
	.byte	0x5
	.uleb128 0
	.long	.LASF294
	.byte	0x5
	.uleb128 0
	.long	.LASF295
	.byte	0x5
	.uleb128 0
	.long	.LASF296
	.byte	0x5
	.uleb128 0
	.long	.LASF297
	.byte	0x5
	.uleb128 0
	.long	.LASF298
	.byte	0x5
	.uleb128 0
	.long	.LASF299
	.byte	0x5
	.uleb128 0
	.long	.LASF300
	.byte	0x5
	.uleb128 0
	.long	.LASF301
	.byte	0x5
	.uleb128 0
	.long	.LASF302
	.byte	0x5
	.uleb128 0
	.long	.LASF303
	.byte	0x5
	.uleb128 0
	.long	.LASF304
	.byte	0x5
	.uleb128 0
	.long	.LASF305
	.byte	0x5
	.uleb128 0
	.long	.LASF306
	.byte	0x5
	.uleb128 0
	.long	.LASF307
	.byte	0x5
	.uleb128 0
	.long	.LASF308
	.byte	0x5
	.uleb128 0
	.long	.LASF309
	.byte	0x5
	.uleb128 0
	.long	.LASF310
	.byte	0x5
	.uleb128 0
	.long	.LASF311
	.byte	0x5
	.uleb128 0
	.long	.LASF312
	.byte	0x5
	.uleb128 0
	.long	.LASF313
	.byte	0x5
	.uleb128 0
	.long	.LASF314
	.byte	0x5
	.uleb128 0
	.long	.LASF315
	.byte	0x5
	.uleb128 0
	.long	.LASF316
	.byte	0x5
	.uleb128 0
	.long	.LASF317
	.byte	0x5
	.uleb128 0
	.long	.LASF318
	.byte	0x5
	.uleb128 0
	.long	.LASF319
	.byte	0x5
	.uleb128 0
	.long	.LASF320
	.byte	0x5
	.uleb128 0
	.long	.LASF321
	.byte	0x5
	.uleb128 0
	.long	.LASF322
	.byte	0x5
	.uleb128 0
	.long	.LASF323
	.byte	0x5
	.uleb128 0
	.long	.LASF324
	.byte	0x5
	.uleb128 0
	.long	.LASF325
	.byte	0x5
	.uleb128 0
	.long	.LASF326
	.byte	0x5
	.uleb128 0
	.long	.LASF327
	.byte	0x5
	.uleb128 0
	.long	.LASF328
	.byte	0x5
	.uleb128 0
	.long	.LASF329
	.byte	0x5
	.uleb128 0
	.long	.LASF330
	.byte	0x5
	.uleb128 0
	.long	.LASF331
	.byte	0x5
	.uleb128 0
	.long	.LASF332
	.byte	0x5
	.uleb128 0
	.long	.LASF333
	.byte	0x5
	.uleb128 0
	.long	.LASF334
	.byte	0x5
	.uleb128 0
	.long	.LASF335
	.byte	0x5
	.uleb128 0
	.long	.LASF336
	.byte	0x5
	.uleb128 0
	.long	.LASF337
	.byte	0x5
	.uleb128 0
	.long	.LASF338
	.byte	0x5
	.uleb128 0
	.long	.LASF339
	.byte	0x5
	.uleb128 0
	.long	.LASF340
	.byte	0x5
	.uleb128 0
	.long	.LASF341
	.byte	0x5
	.uleb128 0
	.long	.LASF342
	.byte	0x5
	.uleb128 0
	.long	.LASF343
	.byte	0x5
	.uleb128 0
	.long	.LASF344
	.byte	0x5
	.uleb128 0
	.long	.LASF345
	.byte	0x5
	.uleb128 0
	.long	.LASF346
	.byte	0x5
	.uleb128 0
	.long	.LASF347
	.byte	0x5
	.uleb128 0
	.long	.LASF348
	.byte	0x5
	.uleb128 0
	.long	.LASF349
	.byte	0x5
	.uleb128 0
	.long	.LASF350
	.byte	0x5
	.uleb128 0
	.long	.LASF351
	.byte	0x5
	.uleb128 0
	.long	.LASF352
	.byte	0x5
	.uleb128 0
	.long	.LASF353
	.byte	0x5
	.uleb128 0
	.long	.LASF354
	.byte	0x5
	.uleb128 0
	.long	.LASF355
	.byte	0x5
	.uleb128 0
	.long	.LASF356
	.byte	0x5
	.uleb128 0
	.long	.LASF357
	.byte	0x5
	.uleb128 0
	.long	.LASF358
	.byte	0x5
	.uleb128 0
	.long	.LASF359
	.byte	0x5
	.uleb128 0
	.long	.LASF360
	.byte	0x5
	.uleb128 0
	.long	.LASF361
	.byte	0x5
	.uleb128 0
	.long	.LASF362
	.byte	0x5
	.uleb128 0
	.long	.LASF363
	.byte	0x5
	.uleb128 0
	.long	.LASF364
	.byte	0x5
	.uleb128 0
	.long	.LASF365
	.byte	0x5
	.uleb128 0
	.long	.LASF366
	.byte	0x5
	.uleb128 0
	.long	.LASF367
	.byte	0x5
	.uleb128 0
	.long	.LASF368
	.byte	0x5
	.uleb128 0
	.long	.LASF369
	.byte	0x5
	.uleb128 0
	.long	.LASF370
	.byte	0x5
	.uleb128 0
	.long	.LASF371
	.byte	0x5
	.uleb128 0
	.long	.LASF372
	.byte	0x5
	.uleb128 0
	.long	.LASF373
	.byte	0x5
	.uleb128 0
	.long	.LASF374
	.byte	0x5
	.uleb128 0
	.long	.LASF375
	.byte	0x5
	.uleb128 0
	.long	.LASF376
	.byte	0x5
	.uleb128 0
	.long	.LASF377
	.byte	0x5
	.uleb128 0
	.long	.LASF378
	.byte	0
	.section	.debug_macro,"G",@progbits,wm4.types.h.5.ce5cf6e0f5e04c88197477365e766aac,comdat
.Ldebug_macro3:
	.value	0x4
	.byte	0
	.byte	0x5
	.uleb128 0x5
	.long	.LASF380
	.byte	0x5
	.uleb128 0x7
	.long	.LASF381
	.byte	0
	.section	.debug_macro,"G",@progbits,wm4.kernel.h.27.55537b03585a581058170c4d492e9fbd,comdat
.Ldebug_macro4:
	.value	0x4
	.byte	0
	.byte	0x5
	.uleb128 0x1b
	.long	.LASF382
	.byte	0x5
	.uleb128 0x1c
	.long	.LASF383
	.byte	0x5
	.uleb128 0x1e
	.long	.LASF384
	.byte	0x5
	.uleb128 0x21
	.long	.LASF385
	.byte	0x5
	.uleb128 0x26
	.long	.LASF386
	.byte	0x5
	.uleb128 0x29
	.long	.LASF387
	.byte	0x5
	.uleb128 0x2c
	.long	.LASF388
	.byte	0x5
	.uleb128 0x37
	.long	.LASF389
	.byte	0x5
	.uleb128 0x38
	.long	.LASF390
	.byte	0x5
	.uleb128 0x3a
	.long	.LASF391
	.byte	0
	.section	.debug_macro,"G",@progbits,wm4.list.h.81.e8b2f05650366a7d54625f3c709387c4,comdat
.Ldebug_macro5:
	.value	0x4
	.byte	0
	.byte	0x5
	.uleb128 0x51
	.long	.LASF392
	.byte	0x5
	.uleb128 0x59
	.long	.LASF393
	.byte	0x5
	.uleb128 0xb0
	.long	.LASF394
	.byte	0x5
	.uleb128 0xb7
	.long	.LASF395
	.byte	0x5
	.uleb128 0xbd
	.long	.LASF396
	.byte	0x5
	.uleb128 0xc5
	.long	.LASF397
	.byte	0x5
	.uleb128 0xcb
	.long	.LASF398
	.byte	0x5
	.uleb128 0xd2
	.long	.LASF399
	.byte	0x5
	.uleb128 0xdc
	.long	.LASF400
	.byte	0
	.section	.debug_macro,"G",@progbits,wm4.config.h.3.5098c18d6f25962e3a31ca1ba399ffa3,comdat
.Ldebug_macro6:
	.value	0x4
	.byte	0
	.byte	0x5
	.uleb128 0x3
	.long	.LASF403
	.byte	0x5
	.uleb128 0x5
	.long	.LASF404
	.byte	0x5
	.uleb128 0x6
	.long	.LASF405
	.byte	0x5
	.uleb128 0x7
	.long	.LASF406
	.byte	0x5
	.uleb128 0xa
	.long	.LASF407
	.byte	0x5
	.uleb128 0xc
	.long	.LASF408
	.byte	0
	.section	.debug_macro,"G",@progbits,wm4.page.h.16.ce77fc105ad3d118d91491723596dd47,comdat
.Ldebug_macro7:
	.value	0x4
	.byte	0
	.byte	0x5
	.uleb128 0x10
	.long	.LASF409
	.byte	0x5
	.uleb128 0x11
	.long	.LASF410
	.byte	0x5
	.uleb128 0x12
	.long	.LASF411
	.byte	0x5
	.uleb128 0x14
	.long	.LASF412
	.byte	0x5
	.uleb128 0x15
	.long	.LASF413
	.byte	0x5
	.uleb128 0x18
	.long	.LASF414
	.byte	0x5
	.uleb128 0x19
	.long	.LASF415
	.byte	0x5
	.uleb128 0x1b
	.long	.LASF416
	.byte	0x5
	.uleb128 0x1c
	.long	.LASF417
	.byte	0x5
	.uleb128 0x1e
	.long	.LASF418
	.byte	0x5
	.uleb128 0x20
	.long	.LASF419
	.byte	0x5
	.uleb128 0x22
	.long	.LASF420
	.byte	0x5
	.uleb128 0x23
	.long	.LASF421
	.byte	0x5
	.uleb128 0x25
	.long	.LASF422
	.byte	0x5
	.uleb128 0x26
	.long	.LASF423
	.byte	0x5
	.uleb128 0x27
	.long	.LASF424
	.byte	0x5
	.uleb128 0x28
	.long	.LASF425
	.byte	0x5
	.uleb128 0x29
	.long	.LASF426
	.byte	0x5
	.uleb128 0x2a
	.long	.LASF427
	.byte	0x5
	.uleb128 0x2b
	.long	.LASF428
	.byte	0x5
	.uleb128 0x2c
	.long	.LASF429
	.byte	0x5
	.uleb128 0x2e
	.long	.LASF430
	.byte	0x5
	.uleb128 0x2f
	.long	.LASF431
	.byte	0x5
	.uleb128 0x30
	.long	.LASF432
	.byte	0x5
	.uleb128 0x31
	.long	.LASF433
	.byte	0x5
	.uleb128 0x32
	.long	.LASF434
	.byte	0x5
	.uleb128 0x33
	.long	.LASF435
	.byte	0x5
	.uleb128 0x34
	.long	.LASF436
	.byte	0x5
	.uleb128 0x35
	.long	.LASF437
	.byte	0x5
	.uleb128 0x37
	.long	.LASF438
	.byte	0x5
	.uleb128 0x38
	.long	.LASF439
	.byte	0x5
	.uleb128 0x39
	.long	.LASF440
	.byte	0x5
	.uleb128 0x3a
	.long	.LASF441
	.byte	0x5
	.uleb128 0x3b
	.long	.LASF442
	.byte	0x5
	.uleb128 0x3d
	.long	.LASF443
	.byte	0x5
	.uleb128 0x40
	.long	.LASF444
	.byte	0
	.section	.debug_macro,"G",@progbits,wm4.vmmap.h.7.19545d496afbad2f9659dd15270e0d7a,comdat
.Ldebug_macro8:
	.value	0x4
	.byte	0
	.byte	0x5
	.uleb128 0x7
	.long	.LASF445
	.byte	0x5
	.uleb128 0x8
	.long	.LASF446
	.byte	0
	.section	.debug_macro,"G",@progbits,wm4.pagetable.h.6.22608c150ad8426775e8969d2a213369,comdat
.Ldebug_macro9:
	.value	0x4
	.byte	0
	.byte	0x5
	.uleb128 0x6
	.long	.LASF447
	.byte	0x5
	.uleb128 0x7
	.long	.LASF448
	.byte	0x5
	.uleb128 0x8
	.long	.LASF449
	.byte	0x5
	.uleb128 0x9
	.long	.LASF450
	.byte	0x5
	.uleb128 0xa
	.long	.LASF451
	.byte	0x5
	.uleb128 0xb
	.long	.LASF452
	.byte	0x5
	.uleb128 0xc
	.long	.LASF453
	.byte	0x5
	.uleb128 0xd
	.long	.LASF454
	.byte	0x5
	.uleb128 0xe
	.long	.LASF455
	.byte	0x5
	.uleb128 0x10
	.long	.LASF456
	.byte	0x5
	.uleb128 0x1d
	.long	.LASF457
	.byte	0x5
	.uleb128 0x1e
	.long	.LASF458
	.byte	0x5
	.uleb128 0x1f
	.long	.LASF459
	.byte	0x5
	.uleb128 0x20
	.long	.LASF460
	.byte	0x5
	.uleb128 0x21
	.long	.LASF461
	.byte	0x5
	.uleb128 0x23
	.long	.LASF456
	.byte	0x5
	.uleb128 0x24
	.long	.LASF462
	.byte	0x5
	.uleb128 0x25
	.long	.LASF463
	.byte	0x5
	.uleb128 0x26
	.long	.LASF464
	.byte	0x5
	.uleb128 0x28
	.long	.LASF465
	.byte	0x5
	.uleb128 0x2c
	.long	.LASF466
	.byte	0x5
	.uleb128 0x2d
	.long	.LASF467
	.byte	0x5
	.uleb128 0x2e
	.long	.LASF468
	.byte	0x5
	.uleb128 0x30
	.long	.LASF469
	.byte	0x5
	.uleb128 0x31
	.long	.LASF470
	.byte	0x5
	.uleb128 0x32
	.long	.LASF471
	.byte	0x5
	.uleb128 0x33
	.long	.LASF472
	.byte	0x5
	.uleb128 0x35
	.long	.LASF473
	.byte	0x5
	.uleb128 0x3f
	.long	.LASF474
	.byte	0
	.section	.debug_macro,"G",@progbits,wm4.config.h.14.e2badae53c883e2d30ba918a4d0452c1,comdat
.Ldebug_macro10:
	.value	0x4
	.byte	0
	.byte	0x5
	.uleb128 0xe
	.long	.LASF476
	.byte	0x5
	.uleb128 0xf
	.long	.LASF477
	.byte	0x5
	.uleb128 0x10
	.long	.LASF478
	.byte	0x5
	.uleb128 0x1a
	.long	.LASF479
	.byte	0x5
	.uleb128 0x1d
	.long	.LASF480
	.byte	0x5
	.uleb128 0x23
	.long	.LASF481
	.byte	0x5
	.uleb128 0x24
	.long	.LASF482
	.byte	0x5
	.uleb128 0x25
	.long	.LASF483
	.byte	0x5
	.uleb128 0x26
	.long	.LASF484
	.byte	0x5
	.uleb128 0x27
	.long	.LASF485
	.byte	0x5
	.uleb128 0x28
	.long	.LASF486
	.byte	0x5
	.uleb128 0x2b
	.long	.LASF487
	.byte	0x5
	.uleb128 0x2f
	.long	.LASF488
	.byte	0
	.section	.debug_macro,"G",@progbits,wm4.proc.h.16.d5cbe4ff5cf52613d45c388ad8a49d2b,comdat
.Ldebug_macro11:
	.value	0x4
	.byte	0
	.byte	0x5
	.uleb128 0x10
	.long	.LASF489
	.byte	0x5
	.uleb128 0x11
	.long	.LASF490
	.byte	0x5
	.uleb128 0xaa
	.long	.LASF491
	.byte	0x5
	.uleb128 0xab
	.long	.LASF492
	.byte	0
	.section	.debug_macro,"G",@progbits,wm4.debug.h.29.320d85be495b514ff3c326a420b3cb7f,comdat
.Ldebug_macro12:
	.value	0x4
	.byte	0
	.byte	0x5
	.uleb128 0x1d
	.long	.LASF495
	.byte	0x5
	.uleb128 0x1e
	.long	.LASF496
	.byte	0x5
	.uleb128 0x1f
	.long	.LASF497
	.byte	0x5
	.uleb128 0x20
	.long	.LASF498
	.byte	0x5
	.uleb128 0x21
	.long	.LASF499
	.byte	0x5
	.uleb128 0x22
	.long	.LASF500
	.byte	0x5
	.uleb128 0x23
	.long	.LASF501
	.byte	0x5
	.uleb128 0x24
	.long	.LASF502
	.byte	0x5
	.uleb128 0x25
	.long	.LASF503
	.byte	0x5
	.uleb128 0x27
	.long	.LASF504
	.byte	0x5
	.uleb128 0x28
	.long	.LASF505
	.byte	0x5
	.uleb128 0x29
	.long	.LASF506
	.byte	0x5
	.uleb128 0x2a
	.long	.LASF507
	.byte	0x5
	.uleb128 0x2b
	.long	.LASF508
	.byte	0x5
	.uleb128 0x2c
	.long	.LASF509
	.byte	0x5
	.uleb128 0x2d
	.long	.LASF510
	.byte	0x5
	.uleb128 0x2f
	.long	.LASF511
	.byte	0x5
	.uleb128 0x34
	.long	.LASF512
	.byte	0x5
	.uleb128 0x35
	.long	.LASF513
	.byte	0x5
	.uleb128 0x36
	.long	.LASF514
	.byte	0x5
	.uleb128 0x37
	.long	.LASF515
	.byte	0x5
	.uleb128 0x38
	.long	.LASF516
	.byte	0x5
	.uleb128 0x39
	.long	.LASF517
	.byte	0x5
	.uleb128 0x3a
	.long	.LASF518
	.byte	0x5
	.uleb128 0x3b
	.long	.LASF519
	.byte	0x5
	.uleb128 0x3c
	.long	.LASF520
	.byte	0x5
	.uleb128 0x3d
	.long	.LASF521
	.byte	0x5
	.uleb128 0x3e
	.long	.LASF522
	.byte	0x5
	.uleb128 0x3f
	.long	.LASF523
	.byte	0x5
	.uleb128 0x40
	.long	.LASF524
	.byte	0x5
	.uleb128 0x41
	.long	.LASF525
	.byte	0x5
	.uleb128 0x42
	.long	.LASF526
	.byte	0x5
	.uleb128 0x43
	.long	.LASF527
	.byte	0x5
	.uleb128 0x44
	.long	.LASF528
	.byte	0x5
	.uleb128 0x45
	.long	.LASF529
	.byte	0x5
	.uleb128 0x46
	.long	.LASF530
	.byte	0x5
	.uleb128 0x47
	.long	.LASF531
	.byte	0x5
	.uleb128 0x48
	.long	.LASF532
	.byte	0x5
	.uleb128 0x49
	.long	.LASF533
	.byte	0x5
	.uleb128 0x4a
	.long	.LASF534
	.byte	0x5
	.uleb128 0x4b
	.long	.LASF535
	.byte	0x5
	.uleb128 0x4c
	.long	.LASF536
	.byte	0x5
	.uleb128 0x4d
	.long	.LASF537
	.byte	0x5
	.uleb128 0x4e
	.long	.LASF538
	.byte	0x5
	.uleb128 0x4f
	.long	.LASF539
	.byte	0x5
	.uleb128 0x50
	.long	.LASF540
	.byte	0x5
	.uleb128 0x51
	.long	.LASF541
	.byte	0x5
	.uleb128 0x52
	.long	.LASF542
	.byte	0x5
	.uleb128 0x54
	.long	.LASF543
	.byte	0x5
	.uleb128 0x55
	.long	.LASF544
	.byte	0x5
	.uleb128 0x56
	.long	.LASF545
	.byte	0x5
	.uleb128 0x57
	.long	.LASF546
	.byte	0x5
	.uleb128 0x58
	.long	.LASF547
	.byte	0x5
	.uleb128 0x59
	.long	.LASF548
	.byte	0x5
	.uleb128 0x5a
	.long	.LASF549
	.byte	0x5
	.uleb128 0x64
	.long	.LASF550
	.byte	0x5
	.uleb128 0x97
	.long	.LASF551
	.byte	0x5
	.uleb128 0xa8
	.long	.LASF552
	.byte	0x5
	.uleb128 0xac
	.long	.LASF553
	.byte	0x5
	.uleb128 0xb3
	.long	.LASF554
	.byte	0x5
	.uleb128 0xc3
	.long	.LASF555
	.byte	0x5
	.uleb128 0xd0
	.long	.LASF556
	.byte	0x5
	.uleb128 0xdf
	.long	.LASF557
	.byte	0x5
	.uleb128 0xee
	.long	.LASF558
	.byte	0x5
	.uleb128 0x101
	.long	.LASF559
	.byte	0x5
	.uleb128 0x104
	.long	.LASF560
	.byte	0x5
	.uleb128 0x10b
	.long	.LASF561
	.byte	0x5
	.uleb128 0x126
	.long	.LASF562
	.byte	0x5
	.uleb128 0x127
	.long	.LASF563
	.byte	0x5
	.uleb128 0x128
	.long	.LASF564
	.byte	0x5
	.uleb128 0x129
	.long	.LASF565
	.byte	0x5
	.uleb128 0x12a
	.long	.LASF566
	.byte	0x5
	.uleb128 0x12b
	.long	.LASF567
	.byte	0
	.section	.debug_macro,"G",@progbits,wm4.interrupt.h.8.e18d17fef3e23eba1335c3f7cd8b9b26,comdat
.Ldebug_macro13:
	.value	0x4
	.byte	0
	.byte	0x5
	.uleb128 0x8
	.long	.LASF568
	.byte	0x5
	.uleb128 0x9
	.long	.LASF569
	.byte	0x5
	.uleb128 0xa
	.long	.LASF570
	.byte	0x5
	.uleb128 0xb
	.long	.LASF571
	.byte	0x5
	.uleb128 0xd
	.long	.LASF572
	.byte	0x5
	.uleb128 0xe
	.long	.LASF573
	.byte	0x5
	.uleb128 0x10
	.long	.LASF574
	.byte	0x5
	.uleb128 0x11
	.long	.LASF575
	.byte	0x5
	.uleb128 0x12
	.long	.LASF576
	.byte	0x5
	.uleb128 0x13
	.long	.LASF577
	.byte	0x5
	.uleb128 0x19
	.long	.LASF578
	.byte	0x5
	.uleb128 0x1c
	.long	.LASF579
	.byte	0x5
	.uleb128 0x1d
	.long	.LASF580
	.byte	0
	.section	.debug_macro,"G",@progbits,wm4.errno.h.12.8e602d82239fc14b356bf7ba5b1f7039,comdat
.Ldebug_macro14:
	.value	0x4
	.byte	0
	.byte	0x5
	.uleb128 0xc
	.long	.LASF581
	.byte	0x5
	.uleb128 0xd
	.long	.LASF582
	.byte	0x5
	.uleb128 0xe
	.long	.LASF583
	.byte	0x5
	.uleb128 0xf
	.long	.LASF584
	.byte	0x5
	.uleb128 0x10
	.long	.LASF585
	.byte	0x5
	.uleb128 0x11
	.long	.LASF586
	.byte	0x5
	.uleb128 0x12
	.long	.LASF587
	.byte	0x5
	.uleb128 0x13
	.long	.LASF588
	.byte	0x5
	.uleb128 0x14
	.long	.LASF589
	.byte	0x5
	.uleb128 0x15
	.long	.LASF590
	.byte	0x5
	.uleb128 0x16
	.long	.LASF591
	.byte	0x5
	.uleb128 0x17
	.long	.LASF592
	.byte	0x5
	.uleb128 0x18
	.long	.LASF593
	.byte	0x5
	.uleb128 0x19
	.long	.LASF594
	.byte	0x5
	.uleb128 0x1a
	.long	.LASF595
	.byte	0x5
	.uleb128 0x1b
	.long	.LASF596
	.byte	0x5
	.uleb128 0x1c
	.long	.LASF597
	.byte	0x5
	.uleb128 0x1d
	.long	.LASF598
	.byte	0x5
	.uleb128 0x1e
	.long	.LASF599
	.byte	0x5
	.uleb128 0x1f
	.long	.LASF600
	.byte	0x5
	.uleb128 0x20
	.long	.LASF601
	.byte	0x5
	.uleb128 0x21
	.long	.LASF602
	.byte	0x5
	.uleb128 0x22
	.long	.LASF603
	.byte	0x5
	.uleb128 0x23
	.long	.LASF604
	.byte	0x5
	.uleb128 0x24
	.long	.LASF605
	.byte	0x5
	.uleb128 0x25
	.long	.LASF606
	.byte	0x5
	.uleb128 0x26
	.long	.LASF607
	.byte	0x5
	.uleb128 0x27
	.long	.LASF608
	.byte	0x5
	.uleb128 0x28
	.long	.LASF609
	.byte	0x5
	.uleb128 0x29
	.long	.LASF610
	.byte	0x5
	.uleb128 0x2a
	.long	.LASF611
	.byte	0x5
	.uleb128 0x2b
	.long	.LASF612
	.byte	0x5
	.uleb128 0x2c
	.long	.LASF613
	.byte	0x5
	.uleb128 0x2d
	.long	.LASF614
	.byte	0x5
	.uleb128 0x2f
	.long	.LASF615
	.byte	0x5
	.uleb128 0x30
	.long	.LASF616
	.byte	0x5
	.uleb128 0x31
	.long	.LASF617
	.byte	0x5
	.uleb128 0x32
	.long	.LASF618
	.byte	0x5
	.uleb128 0x33
	.long	.LASF619
	.byte	0x5
	.uleb128 0x34
	.long	.LASF620
	.byte	0x5
	.uleb128 0x35
	.long	.LASF621
	.byte	0x5
	.uleb128 0x36
	.long	.LASF622
	.byte	0x5
	.uleb128 0x37
	.long	.LASF623
	.byte	0x5
	.uleb128 0x38
	.long	.LASF624
	.byte	0x5
	.uleb128 0x39
	.long	.LASF625
	.byte	0x5
	.uleb128 0x3a
	.long	.LASF626
	.byte	0x5
	.uleb128 0x3b
	.long	.LASF627
	.byte	0x5
	.uleb128 0x3c
	.long	.LASF628
	.byte	0x5
	.uleb128 0x3d
	.long	.LASF629
	.byte	0x5
	.uleb128 0x3e
	.long	.LASF630
	.byte	0x5
	.uleb128 0x3f
	.long	.LASF631
	.byte	0x5
	.uleb128 0x40
	.long	.LASF632
	.byte	0x5
	.uleb128 0x41
	.long	.LASF633
	.byte	0x5
	.uleb128 0x42
	.long	.LASF634
	.byte	0x5
	.uleb128 0x43
	.long	.LASF635
	.byte	0x5
	.uleb128 0x44
	.long	.LASF636
	.byte	0x5
	.uleb128 0x45
	.long	.LASF637
	.byte	0x5
	.uleb128 0x47
	.long	.LASF638
	.byte	0x5
	.uleb128 0x49
	.long	.LASF639
	.byte	0x5
	.uleb128 0x4a
	.long	.LASF640
	.byte	0x5
	.uleb128 0x4b
	.long	.LASF641
	.byte	0x5
	.uleb128 0x4c
	.long	.LASF642
	.byte	0x5
	.uleb128 0x4d
	.long	.LASF643
	.byte	0x5
	.uleb128 0x4e
	.long	.LASF644
	.byte	0x5
	.uleb128 0x4f
	.long	.LASF645
	.byte	0x5
	.uleb128 0x50
	.long	.LASF646
	.byte	0x5
	.uleb128 0x51
	.long	.LASF647
	.byte	0x5
	.uleb128 0x52
	.long	.LASF648
	.byte	0x5
	.uleb128 0x53
	.long	.LASF649
	.byte	0x5
	.uleb128 0x54
	.long	.LASF650
	.byte	0x5
	.uleb128 0x55
	.long	.LASF651
	.byte	0x5
	.uleb128 0x56
	.long	.LASF652
	.byte	0x5
	.uleb128 0x57
	.long	.LASF653
	.byte	0x5
	.uleb128 0x58
	.long	.LASF654
	.byte	0x5
	.uleb128 0x59
	.long	.LASF655
	.byte	0x5
	.uleb128 0x5a
	.long	.LASF656
	.byte	0x5
	.uleb128 0x5b
	.long	.LASF657
	.byte	0x5
	.uleb128 0x5c
	.long	.LASF658
	.byte	0x5
	.uleb128 0x5d
	.long	.LASF659
	.byte	0x5
	.uleb128 0x5e
	.long	.LASF660
	.byte	0x5
	.uleb128 0x5f
	.long	.LASF661
	.byte	0x5
	.uleb128 0x60
	.long	.LASF662
	.byte	0x5
	.uleb128 0x61
	.long	.LASF663
	.byte	0x5
	.uleb128 0x62
	.long	.LASF664
	.byte	0x5
	.uleb128 0x63
	.long	.LASF665
	.byte	0x5
	.uleb128 0x64
	.long	.LASF666
	.byte	0x5
	.uleb128 0x65
	.long	.LASF667
	.byte	0x5
	.uleb128 0x66
	.long	.LASF668
	.byte	0x5
	.uleb128 0x67
	.long	.LASF669
	.byte	0x5
	.uleb128 0x68
	.long	.LASF670
	.byte	0x5
	.uleb128 0x69
	.long	.LASF671
	.byte	0x5
	.uleb128 0x6a
	.long	.LASF672
	.byte	0x5
	.uleb128 0x6b
	.long	.LASF673
	.byte	0x5
	.uleb128 0x6c
	.long	.LASF674
	.byte	0x5
	.uleb128 0x6d
	.long	.LASF675
	.byte	0x5
	.uleb128 0x6e
	.long	.LASF676
	.byte	0x5
	.uleb128 0x6f
	.long	.LASF677
	.byte	0x5
	.uleb128 0x70
	.long	.LASF678
	.byte	0x5
	.uleb128 0x71
	.long	.LASF679
	.byte	0x5
	.uleb128 0x72
	.long	.LASF680
	.byte	0x5
	.uleb128 0x73
	.long	.LASF681
	.byte	0x5
	.uleb128 0x74
	.long	.LASF682
	.byte	0x5
	.uleb128 0x75
	.long	.LASF683
	.byte	0x5
	.uleb128 0x76
	.long	.LASF684
	.byte	0x5
	.uleb128 0x77
	.long	.LASF685
	.byte	0x5
	.uleb128 0x78
	.long	.LASF686
	.byte	0x5
	.uleb128 0x79
	.long	.LASF687
	.byte	0x5
	.uleb128 0x7a
	.long	.LASF688
	.byte	0x5
	.uleb128 0x7b
	.long	.LASF689
	.byte	0x5
	.uleb128 0x7c
	.long	.LASF690
	.byte	0x5
	.uleb128 0x7d
	.long	.LASF691
	.byte	0x5
	.uleb128 0x7e
	.long	.LASF692
	.byte	0x5
	.uleb128 0x7f
	.long	.LASF693
	.byte	0x5
	.uleb128 0x80
	.long	.LASF694
	.byte	0x5
	.uleb128 0x81
	.long	.LASF695
	.byte	0x5
	.uleb128 0x82
	.long	.LASF696
	.byte	0x5
	.uleb128 0x83
	.long	.LASF697
	.byte	0x5
	.uleb128 0x84
	.long	.LASF698
	.byte	0x5
	.uleb128 0x85
	.long	.LASF699
	.byte	0x5
	.uleb128 0x86
	.long	.LASF700
	.byte	0x5
	.uleb128 0x87
	.long	.LASF701
	.byte	0x5
	.uleb128 0x88
	.long	.LASF702
	.byte	0x5
	.uleb128 0x8a
	.long	.LASF703
	.byte	0x5
	.uleb128 0x8b
	.long	.LASF704
	.byte	0x5
	.uleb128 0x8c
	.long	.LASF705
	.byte	0x5
	.uleb128 0x8d
	.long	.LASF706
	.byte	0x5
	.uleb128 0x8e
	.long	.LASF707
	.byte	0x5
	.uleb128 0x8f
	.long	.LASF708
	.byte	0x5
	.uleb128 0x90
	.long	.LASF709
	.byte	0x5
	.uleb128 0x93
	.long	.LASF710
	.byte	0x5
	.uleb128 0x94
	.long	.LASF711
	.byte	0x5
	.uleb128 0x97
	.long	.LASF712
	.byte	0
	.section	.debug_line,"",@progbits
.Ldebug_line0:
	.section	.debug_str,"MS",@progbits,1
.LASF675:
	.string	"EOPNOTSUPP 95"
.LASF365:
	.string	"__linux__ 1"
.LASF247:
	.string	"__FLT128_MIN_10_EXP__ (-4931)"
.LASF311:
	.string	"__DEC128_EPSILON__ 1E-33DL"
.LASF539:
	.string	"DBG_VM DBG_MODE(28)"
.LASF49:
	.string	"__UINT32_TYPE__ unsigned int"
.LASF326:
	.string	"__GCC_ATOMIC_WCHAR_T_LOCK_FREE 2"
.LASF104:
	.string	"__UINT32_MAX__ 0xffffffffU"
.LASF725:
	.string	"size_t"
.LASF410:
	.string	"PAGE_SIZE ((uintptr_t)(1UL << PAGE_SHIFT))"
.LASF686:
	.string	"EISCONN 106"
.LASF600:
	.string	"ENOTDIR 20"
.LASF816:
	.string	"sched_queue_init"
.LASF241:
	.string	"__FLT64_HAS_INFINITY__ 1"
.LASF786:
	.string	"kc_ctx"
.LASF545:
	.string	"DBG_VMMAP DBG_MODE(35)"
.LASF332:
	.string	"__GCC_DESTRUCTIVE_SIZE 64"
.LASF514:
	.string	"DBG_MM DBG_MODE(1)"
.LASF412:
	.string	"PAGE_ALIGN_DOWN(x) ((void *)(((uintptr_t)(x)&PAGE_MASK)))"
.LASF310:
	.string	"__DEC128_MAX__ 9.999999999999999999999999999999999E6144DL"
.LASF15:
	.string	"__LP64__ 1"
.LASF794:
	.string	"KT_SLEEP_CANCELLABLE"
.LASF527:
	.string	"DBG_ERROR DBG_MODE(14)"
.LASF778:
	.string	"c_rsp"
.LASF451:
	.string	"PT_CACHE_DISABLED 0x010"
.LASF298:
	.string	"__DEC32_SUBNORMAL_MIN__ 0.000001E-95DF"
.LASF262:
	.string	"__FLT32X_MIN_EXP__ (-1021)"
.LASF770:
	.string	"p_cwd"
.LASF699:
	.string	"ENAVAIL 119"
.LASF12:
	.string	"__ATOMIC_CONSUME 1"
.LASF399:
	.string	"list_iterate(list,var,type,member) for (type *var = list_head(list, type, member), *__next_ ##var = list_next(var, type, member); &var->member != (list); var = __next_ ##var, __next_ ##var = list_next(var, type, member))"
.LASF16:
	.string	"__SIZEOF_INT__ 4"
.LASF259:
	.string	"__FLT128_IS_IEC_60559__ 2"
.LASF460:
	.string	"PDE(x) ((((uintptr_t)(x)) >> 21) & INDEX_MASK)"
.LASF607:
	.string	"EFBIG 27"
.LASF409:
	.string	"PAGE_SHIFT 12"
.LASF721:
	.string	"int32_t"
.LASF18:
	.string	"__SIZEOF_LONG_LONG__ 8"
.LASF127:
	.string	"__INT_FAST8_WIDTH__ 8"
.LASF168:
	.string	"__DBL_MAX_10_EXP__ 308"
.LASF812:
	.string	"dbg_print"
.LASF228:
	.string	"__FLT64_MANT_DIG__ 53"
.LASF295:
	.string	"__DEC32_MIN__ 1E-95DF"
.LASF132:
	.string	"__INT_FAST64_MAX__ 0x7fffffffffffffffL"
.LASF28:
	.string	"__ORDER_PDP_ENDIAN__ 3412"
.LASF301:
	.string	"__DEC64_MAX_EXP__ 385"
.LASF780:
	.string	"c_pml4"
.LASF43:
	.string	"__INT8_TYPE__ signed char"
.LASF703:
	.string	"ENOMEDIUM 123"
.LASF218:
	.string	"__FLT32_DECIMAL_DIG__ 9"
.LASF821:
	.string	"__func__"
.LASF34:
	.string	"__SIZE_TYPE__ long unsigned int"
.LASF834:
	.string	"greaterthan"
.LASF540:
	.string	"DBG_TEST DBG_MODE(30)"
.LASF44:
	.string	"__INT16_TYPE__ short int"
.LASF748:
	.string	"kt_state"
.LASF572:
	.string	"INTR_APICTIMER 0xf0"
.LASF406:
	.string	"KERNEL_VMA 0xffff800000000000"
.LASF695:
	.string	"EINPROGRESS 115"
.LASF380:
	.string	"NULL 0"
.LASF318:
	.string	"__GCC_HAVE_SYNC_COMPARE_AND_SWAP_1 1"
.LASF676:
	.string	"EPFNOSUPPORT 96"
.LASF590:
	.string	"ECHILD 10"
.LASF2:
	.string	"__STDC_HOSTED__ 0"
.LASF444:
	.string	"PAGE_FLAGS(x) ((x) & (~PAGE_MASK))"
.LASF708:
	.string	"EKEYREVOKED 128"
.LASF344:
	.string	"__x86_64 1"
.LASF69:
	.string	"__GXX_ABI_VERSION 1017"
.LASF594:
	.string	"EFAULT 14"
.LASF221:
	.string	"__FLT32_MIN__ 1.17549435082228750796873653722224568e-38F32"
.LASF217:
	.string	"__FLT32_MAX_10_EXP__ 38"
.LASF101:
	.string	"__INT64_MAX__ 0x7fffffffffffffffL"
.LASF155:
	.string	"__FLT_NORM_MAX__ 3.40282346638528859811704183484516925e+38F"
.LASF777:
	.string	"c_rip"
.LASF214:
	.string	"__FLT32_MIN_EXP__ (-125)"
.LASF179:
	.string	"__LDBL_MANT_DIG__ 64"
.LASF558:
	.string	"dbg_active(mode) (dbg_modes & (mode))"
.LASF614:
	.string	"ERANGE 34"
.LASF601:
	.string	"EISDIR 21"
.LASF209:
	.string	"__FLT16_HAS_INFINITY__ 1"
.LASF450:
	.string	"PT_WRITE_THROUGH 0x008"
.LASF791:
	.string	"KT_ON_CPU"
.LASF231:
	.string	"__FLT64_MIN_10_EXP__ (-307)"
.LASF32:
	.string	"__GNUC_EXECUTION_CHARSET_NAME \"UTF-8\""
.LASF133:
	.string	"__INT_FAST64_WIDTH__ 64"
.LASF1:
	.string	"__STDC_VERSION__ 199901L"
.LASF348:
	.string	"__ATOMIC_HLE_ACQUIRE 65536"
.LASF413:
	.string	"PAGE_ALIGN_UP(x) ((void *)((((uintptr_t)(x) + (PAGE_SIZE - 1)) & PAGE_MASK)))"
.LASF588:
	.string	"ENOEXEC 8"
.LASF436:
	.string	"PAGE_ALIGNED_1GB(x) ((x) == PAGE_ALIGN_DOWN_1GB(x))"
.LASF839:
	.string	"proc/kmutex.c"
.LASF465:
	.string	"PML4_VADDR_SIZE (PAGE_SIZE * PT_ENTRY_COUNT * PT_ENTRY_COUNT * PT_ENTRY_COUNT * PT_ENTRY_COUNT)"
.LASF608:
	.string	"ENOSPC 28"
.LASF27:
	.string	"__ORDER_BIG_ENDIAN__ 4321"
.LASF833:
	.string	"lessthaneq"
.LASF216:
	.string	"__FLT32_MAX_EXP__ 128"
.LASF309:
	.string	"__DEC128_MIN__ 1E-6143DL"
.LASF553:
	.string	"DEBUG_EXIT do { } while (0);"
.LASF737:
	.string	"km_waitq"
.LASF428:
	.string	"PAGE_ALIGNED_2MB(x) ((x) == PAGE_ALIGN_DOWN_2MB(x))"
.LASF185:
	.string	"__DECIMAL_DIG__ 21"
.LASF258:
	.string	"__FLT128_HAS_QUIET_NAN__ 1"
.LASF461:
	.string	"PTE(x) ((((uintptr_t)(x)) >> 12) & INDEX_MASK)"
.LASF548:
	.string	"DBG_BTREE DBG_MODE(39)"
.LASF623:
	.string	"EIDRM 43"
.LASF25:
	.string	"__BIGGEST_ALIGNMENT__ 16"
.LASF297:
	.string	"__DEC32_EPSILON__ 1E-6DF"
.LASF492:
	.string	"PID_INIT 1"
.LASF281:
	.string	"__FLT64X_MAX_10_EXP__ 4932"
.LASF423:
	.string	"PAGE_SIZE_2MB ((uintptr_t)(1UL << PAGE_SHIFT_2MB))"
.LASF597:
	.string	"EEXIST 17"
.LASF397:
	.string	"list_next(current,type,member) list_head(&(current)->member, type, member)"
.LASF531:
	.string	"DBG_BRK DBG_MODE(18)"
.LASF334:
	.string	"__GCC_ATOMIC_POINTER_LOCK_FREE 2"
.LASF752:
	.string	"kt_preemption_count"
.LASF668:
	.string	"ENOTSOCK 88"
.LASF38:
	.string	"__INTMAX_TYPE__ long int"
.LASF208:
	.string	"__FLT16_HAS_DENORM__ 1"
.LASF706:
	.string	"ENOKEY 126"
.LASF252:
	.string	"__FLT128_NORM_MAX__ 1.18973149535723176508575932662800702e+4932F128"
.LASF417:
	.string	"ADDR_TO_PN(x) (((uintptr_t)(x)) >> PAGE_SHIFT)"
.LASF187:
	.string	"__LDBL_MAX__ 1.18973149535723176502126385303097021e+4932L"
.LASF296:
	.string	"__DEC32_MAX__ 9.999999E96DF"
.LASF134:
	.string	"__UINT_FAST8_MAX__ 0xff"
.LASF670:
	.string	"EMSGSIZE 90"
.LASF785:
	.string	"kc_id"
.LASF273:
	.string	"__FLT32X_HAS_INFINITY__ 1"
.LASF520:
	.string	"DBG_PAGEALLOC DBG_MODE(7)"
.LASF739:
	.string	"km_link"
.LASF559:
	.string	"panic(...) dbg_panic(__FILE__, __LINE__, __func__, __VA_ARGS__)"
.LASF55:
	.string	"__UINT_LEAST8_TYPE__ unsigned char"
.LASF162:
	.string	"__FLT_IS_IEC_60559__ 2"
.LASF811:
	.string	"sched_sleep_on"
.LASF606:
	.string	"ETXTBSY 26"
.LASF720:
	.string	"long int"
.LASF467:
	.string	"IS_2MB_PAGE(n) ((n)&PT_SIZE)"
.LASF266:
	.string	"__FLT32X_DECIMAL_DIG__ 17"
.LASF494:
	.string	"MAX_LAPICS 8"
.LASF274:
	.string	"__FLT32X_HAS_QUIET_NAN__ 1"
.LASF371:
	.string	"__VFS__ 1"
.LASF307:
	.string	"__DEC128_MIN_EXP__ (-6142)"
.LASF246:
	.string	"__FLT128_MIN_EXP__ (-16381)"
.LASF66:
	.string	"__UINT_FAST64_TYPE__ long unsigned int"
.LASF366:
	.string	"__unix 1"
.LASF464:
	.string	"PDP_VADDR_SIZE (PAGE_SIZE * PT_ENTRY_COUNT * PT_ENTRY_COUNT * PT_ENTRY_COUNT)"
.LASF51:
	.string	"__INT_LEAST8_TYPE__ signed char"
.LASF804:
	.string	"curcore"
.LASF836:
	.string	"notequals"
.LASF31:
	.string	"__SIZEOF_POINTER__ 8"
.LASF420:
	.string	"USE_2MB_PAGES 1"
.LASF288:
	.string	"__FLT64X_HAS_DENORM__ 1"
.LASF581:
	.string	"EPERM 1"
.LASF661:
	.string	"ELIBSCN 81"
.LASF65:
	.string	"__UINT_FAST32_TYPE__ long unsigned int"
.LASF570:
	.string	"INTR_GPF 0x0d"
.LASF524:
	.string	"DBG_PROC DBG_MODE(11)"
.LASF568:
	.string	"INTR_DIVIDE_BY_ZERO 0x00"
.LASF33:
	.string	"__GNUC_WIDE_EXECUTION_CHARSET_NAME \"UTF-32LE\""
.LASF438:
	.string	"PAGE_SHIFT_512GB 39"
.LASF814:
	.string	"dbg_color"
.LASF422:
	.string	"PAGE_SHIFT_2MB 21"
.LASF137:
	.string	"__UINT_FAST64_MAX__ 0xffffffffffffffffUL"
.LASF178:
	.string	"__DBL_IS_IEC_60559__ 2"
.LASF827:
	.string	"__next_waiter"
.LASF616:
	.string	"ENAMETOOLONG 36"
.LASF637:
	.string	"EBADSLT 57"
.LASF17:
	.string	"__SIZEOF_LONG__ 8"
.LASF21:
	.string	"__SIZEOF_DOUBLE__ 8"
.LASF114:
	.string	"__INT_LEAST32_WIDTH__ 32"
.LASF769:
	.string	"p_files"
.LASF176:
	.string	"__DBL_HAS_INFINITY__ 1"
.LASF184:
	.string	"__LDBL_MAX_10_EXP__ 4932"
.LASF481:
	.string	"MAXPATHLEN 1024"
.LASF116:
	.string	"__INT64_C(c) c ## L"
.LASF691:
	.string	"ECONNREFUSED 111"
.LASF239:
	.string	"__FLT64_DENORM_MIN__ 4.94065645841246544176568792868221372e-324F64"
.LASF303:
	.string	"__DEC64_MAX__ 9.999999999999999E384DD"
.LASF502:
	.string	"_CYAN_ \"\\x1b[36;40m\""
.LASF150:
	.string	"__FLT_MIN_10_EXP__ (-37)"
.LASF384:
	.string	"offsetof(type,member) ((uintptr_t)((char *)&((type *)(0))->member - (char *)0))"
.LASF712:
	.string	"ENOTSUP EOPNOTSUPP"
.LASF153:
	.string	"__FLT_DECIMAL_DIG__ 9"
.LASF790:
	.string	"KT_NO_STATE"
.LASF636:
	.string	"EBADRQC 56"
.LASF715:
	.string	"signed char"
.LASF425:
	.string	"PAGE_ALIGN_DOWN_2MB(x) (((uintptr_t)(x)) & PAGE_MASK_2MB)"
.LASF223:
	.string	"__FLT32_DENORM_MIN__ 1.40129846432481707092372958328991613e-45F32"
.LASF315:
	.string	"__GNUC_STDC_INLINE__ 1"
.LASF497:
	.string	"_RED_ \"\\x1b[31;40m\""
.LASF161:
	.string	"__FLT_HAS_QUIET_NAN__ 1"
.LASF5:
	.string	"__GNUC_PATCHLEVEL__ 0"
.LASF605:
	.string	"ENOTTY 25"
.LASF325:
	.string	"__GCC_ATOMIC_CHAR32_T_LOCK_FREE 2"
.LASF655:
	.string	"EOVERFLOW 75"
.LASF546:
	.string	"DBG_ELF DBG_MODE(37)"
.LASF88:
	.string	"__PTRDIFF_WIDTH__ 64"
.LASF120:
	.string	"__UINT_LEAST16_MAX__ 0xffff"
.LASF776:
	.string	"context"
.LASF685:
	.string	"ENOBUFS 105"
.LASF552:
	.string	"DEBUG_ENTER do { } while (0);"
.LASF609:
	.string	"ESPIPE 29"
.LASF271:
	.string	"__FLT32X_DENORM_MIN__ 4.94065645841246544176568792868221372e-324F32x"
.LASF233:
	.string	"__FLT64_MAX_10_EXP__ 308"
.LASF129:
	.string	"__INT_FAST16_WIDTH__ 64"
.LASF716:
	.string	"unsigned char"
.LASF222:
	.string	"__FLT32_EPSILON__ 1.19209289550781250000000000000000000e-7F32"
.LASF152:
	.string	"__FLT_MAX_10_EXP__ 38"
.LASF784:
	.string	"core"
.LASF308:
	.string	"__DEC128_MAX_EXP__ 6145"
.LASF822:
	.string	"kmutex_init"
.LASF665:
	.string	"ERESTART 85"
.LASF446:
	.string	"VMMAP_DIR_HILO 2"
.LASF278:
	.string	"__FLT64X_MIN_EXP__ (-16381)"
.LASF8:
	.string	"__ATOMIC_SEQ_CST 5"
.LASF536:
	.string	"DBG_THR DBG_MODE(23)"
.LASF454:
	.string	"PT_SIZE 0x080"
.LASF160:
	.string	"__FLT_HAS_INFINITY__ 1"
.LASF628:
	.string	"ELNRNG 48"
.LASF457:
	.string	"INDEX_MASK 0b111111111"
.LASF735:
	.string	"ktqueue_t"
.LASF234:
	.string	"__FLT64_DECIMAL_DIG__ 17"
.LASF103:
	.string	"__UINT16_MAX__ 0xffff"
.LASF593:
	.string	"EACCES 13"
.LASF579:
	.string	"IPL_HIGH 0xe0"
.LASF123:
	.string	"__UINT32_C(c) c ## U"
.LASF611:
	.string	"EMLINK 31"
.LASF359:
	.string	"__SSE2_MATH__ 1"
.LASF91:
	.string	"__INTMAX_C(c) c ## L"
.LASF183:
	.string	"__LDBL_MAX_EXP__ 16384"
.LASF19:
	.string	"__SIZEOF_SHORT__ 2"
.LASF211:
	.string	"__FLT16_IS_IEC_60559__ 2"
.LASF511:
	.string	"DBG_MODE(x) (1ULL << (x))"
.LASF654:
	.string	"EBADMSG 74"
.LASF681:
	.string	"ENETUNREACH 101"
.LASF62:
	.string	"__INT_FAST64_TYPE__ long int"
.LASF405:
	.string	"KERNEL_PHYS_END ((uintptr_t)(&kernel_phys_end))"
.LASF317:
	.string	"__STRICT_ANSI__ 1"
.LASF639:
	.string	"EBFONT 59"
.LASF93:
	.string	"__UINTMAX_C(c) c ## UL"
.LASF253:
	.string	"__FLT128_MIN__ 3.36210314311209350626267781732175260e-4932F128"
.LASF782:
	.string	"c_kstacksz"
.LASF421:
	.string	"USE_1GB_PAGES 1"
.LASF714:
	.string	"char"
.LASF151:
	.string	"__FLT_MAX_EXP__ 128"
.LASF393:
	.string	"LIST_LINK_INITIALIZER(list_link) { .l_next = NULL, .l_prev = NULL }"
.LASF319:
	.string	"__GCC_HAVE_SYNC_COMPARE_AND_SWAP_2 1"
.LASF758:
	.string	"p_pid"
.LASF775:
	.string	"pml4_t"
.LASF135:
	.string	"__UINT_FAST16_MAX__ 0xffffffffffffffffUL"
.LASF75:
	.string	"__WCHAR_MAX__ 0x7fffffff"
.LASF394:
	.string	"list_item(link,type,member) (type *)((char *)(link)-offsetof(type, member))"
.LASF732:
	.string	"ktqueue"
.LASF702:
	.string	"EDQUOT 122"
.LASF227:
	.string	"__FLT32_IS_IEC_60559__ 2"
.LASF554:
	.ascii	"dbg(mode,...) do { if (dbg_active(mod"
	.string	"e)) { DEBUG_ENTER dbg_print(\"%s\", dbg_color(mode)); dbg_print(\"C%ld P%ld \", curcore.kc_id, curproc ? curproc->p_pid : -1L); dbg_print(\"%s:%d %s(): \", __FILE__, __LINE__, __func__); dbg_print(__VA_ARGS__); dbg_print(\"%s\", _NORMAL_); DEBUG_EXIT } } while (0)"
.LASF580:
	.string	"IPL_HIGHEST 0xff"
.LASF201:
	.string	"__FLT16_MAX_10_EXP__ 4"
.LASF456:
	.string	"PT_ENTRY_COUNT (PAGE_SIZE / sizeof(uintptr_t))"
.LASF345:
	.string	"__x86_64__ 1"
.LASF817:
	.string	"dbg_panic"
.LASF381:
	.string	"packed __attribute__((packed))"
.LASF327:
	.string	"__GCC_ATOMIC_SHORT_LOCK_FREE 2"
.LASF743:
	.string	"kt_retval"
.LASF102:
	.string	"__UINT8_MAX__ 0xff"
.LASF157:
	.string	"__FLT_EPSILON__ 1.19209289550781250000000000000000000e-7F"
.LASF279:
	.string	"__FLT64X_MIN_10_EXP__ (-4931)"
.LASF630:
	.string	"ENOCSI 50"
.LASF452:
	.string	"PT_ACCESSED 0x020"
.LASF175:
	.string	"__DBL_HAS_DENORM__ 1"
.LASF374:
	.string	"__DYNAMIC__ 1"
.LASF771:
	.string	"p_brk"
.LASF361:
	.string	"__SEG_FS 1"
.LASF757:
	.string	"proc"
.LASF831:
	.string	"flags"
.LASF373:
	.string	"__VM__ 1"
.LASF109:
	.string	"__INT_LEAST16_MAX__ 0x7fff"
.LASF683:
	.string	"ECONNABORTED 103"
.LASF248:
	.string	"__FLT128_MAX_EXP__ 16384"
.LASF407:
	.string	"PHYS_OFFSET 0xffff880000000000"
.LASF620:
	.string	"ELOOP 40"
.LASF505:
	.string	"_BGREEN_ \"\\x1b[1;32;40m\""
.LASF761:
	.string	"p_children"
.LASF488:
	.string	"VFS_ROOTFS_TYPE \"s5fs\""
.LASF547:
	.string	"DBG_USER DBG_MODE(38)"
.LASF797:
	.string	"kthread_t"
.LASF441:
	.string	"PAGE_ALIGN_DOWN_512GB(x) (((uintptr_t)(x)) & PAGE_MASK_512GB)"
.LASF378:
	.string	"__KERNEL__ 1"
.LASF385:
	.string	"NOT_YET_IMPLEMENTED(f) dbg(DBG_PRINT, \"Not yet implemented: %s, file %s, line %d\\n\", f, __FILE__, __LINE__)"
.LASF830:
	.string	"intr_enable"
.LASF523:
	.string	"DBG_FORK DBG_MODE(10)"
.LASF202:
	.string	"__FLT16_DECIMAL_DIG__ 5"
.LASF633:
	.string	"EBADR 53"
.LASF59:
	.string	"__INT_FAST8_TYPE__ signed char"
.LASF612:
	.string	"EPIPE 32"
.LASF810:
	.string	"list_insert_tail"
.LASF805:
	.string	"curproc"
.LASF117:
	.string	"__INT_LEAST64_WIDTH__ 64"
.LASF46:
	.string	"__INT64_TYPE__ long int"
.LASF391:
	.string	"QUOTE(...) QUOTE_BY_NAME(__VA_ARGS__)"
.LASF331:
	.string	"__GCC_ATOMIC_TEST_AND_SET_TRUEVAL 1"
.LASF468:
	.string	"IS_1GB_PAGE IS_2MB_PAGE"
.LASF649:
	.string	"ESRMNT 69"
.LASF368:
	.string	"__ELF__ 1"
.LASF801:
	.string	"file"
.LASF660:
	.string	"ELIBBAD 80"
.LASF76:
	.string	"__WCHAR_MIN__ (-__WCHAR_MAX__ - 1)"
.LASF226:
	.string	"__FLT32_HAS_QUIET_NAN__ 1"
.LASF651:
	.string	"EPROTO 71"
.LASF193:
	.string	"__LDBL_HAS_INFINITY__ 1"
.LASF404:
	.string	"KERNEL_PHYS_BASE ((uintptr_t)(&kernel_phys_base))"
.LASF767:
	.string	"p_pml4"
.LASF796:
	.string	"kthread_state_t"
.LASF265:
	.string	"__FLT32X_MAX_10_EXP__ 308"
.LASF485:
	.string	"NAME_LEN 28"
.LASF89:
	.string	"__SIZE_WIDTH__ 64"
.LASF832:
	.string	"greaterthaneq"
.LASF96:
	.string	"__SIG_ATOMIC_MIN__ (-__SIG_ATOMIC_MAX__ - 1)"
.LASF640:
	.string	"ENOSTR 60"
.LASF118:
	.string	"__UINT_LEAST8_MAX__ 0xff"
.LASF300:
	.string	"__DEC64_MIN_EXP__ (-382)"
.LASF329:
	.string	"__GCC_ATOMIC_LONG_LOCK_FREE 2"
.LASF838:
	.ascii	"GNU C99 12.2.0 -march=x86-"
	.string	"64 -m64 -mno-red-zone -mcmodel=large -mno-sse3 -mno-ssse3 -mno-sse4.1 -mno-sse4.2 -mno-sse4 -mno-sse4a -mno-3dnow -mno-avx -mno-avx2 -g3 -gdwarf-3 -std=c99 -fno-pie -ffreestanding -fno-builtin -fno-stack-protector -fsigned-char -fasynchronous-unwind-tables"
.LASF795:
	.string	"KT_EXITED"
.LASF692:
	.string	"EHOSTDOWN 112"
.LASF289:
	.string	"__FLT64X_HAS_INFINITY__ 1"
.LASF727:
	.string	"l_next"
.LASF337:
	.string	"__PRAGMA_REDEFINE_EXTNAME 1"
.LASF483:
	.string	"MAX_VFS 8"
.LASF170:
	.string	"__DBL_MAX__ ((double)1.79769313486231570814527423731704357e+308L)"
.LASF509:
	.string	"_BCYAN_ \"\\x1b[1;36;40m\""
.LASF453:
	.string	"PT_DIRTY 0x040"
.LASF591:
	.string	"EAGAIN 11"
.LASF728:
	.string	"l_prev"
.LASF111:
	.string	"__INT_LEAST16_WIDTH__ 16"
.LASF693:
	.string	"EHOSTUNREACH 113"
.LASF145:
	.string	"__DEC_EVAL_METHOD__ 2"
.LASF533:
	.string	"DBG_VFS DBG_MODE(20)"
.LASF557:
	.string	"dbginfo(mode,func,data) do { if (dbg_active(mode)) { DEBUG_ENTER dbg_print(\"%s\", dbg_color(mode)); dbg_print(\"C%ld P%ld \", curcore.kc_id, curproc ? curproc->p_pid : -1L); dbg_printinfo(func, data); dbg_print(\"%s\", _NORMAL_); DEBUG_EXIT } } while (0)"
.LASF305:
	.string	"__DEC64_SUBNORMAL_MIN__ 0.000000000000001E-383DD"
.LASF741:
	.string	"kt_ctx"
.LASF622:
	.string	"ENOMSG 42"
.LASF506:
	.string	"_BYELLOW_ \"\\x1b[1;33;40m\""
.LASF119:
	.string	"__UINT8_C(c) c"
.LASF363:
	.string	"__gnu_linux__ 1"
.LASF186:
	.string	"__LDBL_DECIMAL_DIG__ 21"
.LASF54:
	.string	"__INT_LEAST64_TYPE__ long int"
.LASF815:
	.string	"list_link_init"
.LASF403:
	.string	"IDENTITY_MAPPED_RAM_SIZE (1 << 16)"
.LASF798:
	.string	"PROC_RUNNING"
.LASF106:
	.string	"__INT_LEAST8_MAX__ 0x7f"
.LASF589:
	.string	"EBADF 9"
.LASF565:
	.string	"KASSERT_LESS(l,r) KASSERT_GENERIC(l, r, lessthan, \"<\")"
.LASF549:
	.string	"DBG_DEFAULT DBG_ERROR"
.LASF9:
	.string	"__ATOMIC_ACQUIRE 2"
.LASF768:
	.string	"p_wait"
.LASF781:
	.string	"c_kstack"
.LASF264:
	.string	"__FLT32X_MAX_EXP__ 1024"
.LASF541:
	.string	"DBG_TESTPASS DBG_MODE(31)"
.LASF282:
	.string	"__FLT64X_DECIMAL_DIG__ 21"
.LASF360:
	.string	"__MMX_WITH_SSE__ 1"
.LASF462:
	.string	"PT_VADDR_SIZE (PAGE_SIZE * PT_ENTRY_COUNT)"
.LASF682:
	.string	"ENETRESET 102"
.LASF98:
	.string	"__INT8_MAX__ 0x7f"
.LASF646:
	.string	"EREMOTE 66"
.LASF336:
	.string	"__GCC_HAVE_DWARF2_CFI_ASM 1"
.LASF723:
	.string	"long unsigned int"
.LASF503:
	.string	"_WHITE_ \"\\x1b[37;40m\""
.LASF478:
	.string	"TICK_MSECS 10"
.LASF48:
	.string	"__UINT16_TYPE__ short unsigned int"
.LASF534:
	.string	"DBG_S5FS DBG_MODE(21)"
.LASF60:
	.string	"__INT_FAST16_TYPE__ long int"
.LASF370:
	.string	"__DRIVERS__ 1"
.LASF448:
	.string	"PT_WRITE 0x002"
.LASF664:
	.string	"EILSEQ 84"
.LASF190:
	.string	"__LDBL_EPSILON__ 1.08420217248550443400745280086994171e-19L"
.LASF294:
	.string	"__DEC32_MAX_EXP__ 97"
.LASF144:
	.string	"__FLT_EVAL_METHOD_TS_18661_3__ 0"
.LASF81:
	.string	"__SCHAR_WIDTH__ 8"
.LASF595:
	.string	"ENOTBLK 15"
.LASF414:
	.string	"PAGE_OFFSET(x) (((uintptr_t)(x)) & ~PAGE_MASK)"
.LASF206:
	.string	"__FLT16_EPSILON__ 9.76562500000000000000000000000000000e-4F16"
.LASF435:
	.string	"PAGE_OFFSET_1GB(x) (((uintptr_t)(x)) & ~PAGE_MASK_1GB)"
.LASF641:
	.string	"ENODATA 61"
.LASF352:
	.string	"__k8__ 1"
.LASF14:
	.string	"_LP64 1"
.LASF471:
	.string	"GDB_PDP_PHYSADDR(pdp,v) (pdp->phys[PDPE(v)] & PAGE_MASK)"
.LASF650:
	.string	"ECOMM 70"
.LASF476:
	.string	"DEFAULT_STACK_SIZE_PAGES 16"
.LASF764:
	.string	"p_child_link"
.LASF544:
	.string	"DBG_ANON DBG_MODE(34)"
.LASF212:
	.string	"__FLT32_MANT_DIG__ 24"
.LASF139:
	.string	"__INTPTR_WIDTH__ 64"
.LASF470:
	.string	"GDB_PD_PHYSADDR(pd,v) (pd->phys[PDE(v)] & PAGE_MASK)"
.LASF671:
	.string	"EPROTOTYPE 91"
.LASF760:
	.string	"p_threads"
.LASF324:
	.string	"__GCC_ATOMIC_CHAR16_T_LOCK_FREE 2"
.LASF744:
	.string	"kt_errno"
.LASF167:
	.string	"__DBL_MAX_EXP__ 1024"
.LASF10:
	.string	"__ATOMIC_RELEASE 3"
.LASF272:
	.string	"__FLT32X_HAS_DENORM__ 1"
.LASF147:
	.string	"__FLT_MANT_DIG__ 24"
.LASF463:
	.string	"PD_VADDR_SIZE (PAGE_SIZE * PT_ENTRY_COUNT * PT_ENTRY_COUNT)"
.LASF512:
	.string	"DBG_ALL (~0ULL)"
.LASF705:
	.string	"ECANCELED 125"
.LASF90:
	.string	"__INTMAX_MAX__ 0x7fffffffffffffffL"
.LASF779:
	.string	"c_rbp"
.LASF237:
	.string	"__FLT64_MIN__ 2.22507385850720138309023271733240406e-308F64"
.LASF395:
	.string	"list_head(list,type,member) list_item((list)->l_next, type, member)"
.LASF575:
	.string	"INTR_SPURIOUS 0xfe"
.LASF92:
	.string	"__UINTMAX_MAX__ 0xffffffffffffffffUL"
.LASF199:
	.string	"__FLT16_MIN_10_EXP__ (-4)"
.LASF490:
	.string	"PROC_NAME_LEN 256"
.LASF491:
	.string	"PID_IDLE 0"
.LASF263:
	.string	"__FLT32X_MIN_10_EXP__ (-307)"
.LASF355:
	.string	"__SSE__ 1"
.LASF401:
	.string	"KTQUEUE_INITIALIZER(ktqueue) { .tq_list = LIST_INITIALIZER((ktqueue).tq_list), }"
.LASF678:
	.string	"EADDRINUSE 98"
.LASF3:
	.string	"__GNUC__ 12"
.LASF321:
	.string	"__GCC_HAVE_SYNC_COMPARE_AND_SWAP_8 1"
.LASF501:
	.string	"_MAGENTA_ \"\\x1b[35;40m\""
.LASF592:
	.string	"ENOMEM 12"
.LASF112:
	.string	"__INT_LEAST32_MAX__ 0x7fffffff"
.LASF803:
	.string	"proc_t"
.LASF159:
	.string	"__FLT_HAS_DENORM__ 1"
.LASF657:
	.string	"EBADFD 77"
.LASF610:
	.string	"EROFS 30"
.LASF386:
	.string	"MIN(a,b) ((a) < (b) ? (a) : (b))"
.LASF275:
	.string	"__FLT32X_IS_IEC_60559__ 2"
.LASF517:
	.string	"DBG_DISK DBG_MODE(4)"
.LASF711:
	.string	"ENOTRECOVERABLE 131"
.LASF538:
	.string	"DBG_OSYSCALL DBG_MODE(25)"
.LASF146:
	.string	"__FLT_RADIX__ 2"
.LASF555:
	.ascii	"dbg_force(mode,.."
	.string	".) do { DEBUG_ENTER dbg_print(\"%s\", dbg_color(mode)); dbg_print(\"C%ld P%ld \", curcore.kc_id, curproc ? curproc->p_pid : -1L); dbg_print(\"%s:%d %s(): \", __FILE__, __LINE__, __func__); dbg_print(__VA_ARGS__); dbg_print(\"%s\", _NORMAL_); DEBUG_EXIT } while (0)"
.LASF740:
	.string	"kthread"
.LASF746:
	.string	"kt_cancelled"
.LASF684:
	.string	"ECONNRESET 104"
.LASF194:
	.string	"__LDBL_HAS_QUIET_NAN__ 1"
.LASF191:
	.string	"__LDBL_DENORM_MIN__ 3.64519953188247460252840593361941982e-4951L"
.LASF343:
	.string	"__amd64__ 1"
.LASF85:
	.string	"__LONG_LONG_WIDTH__ 64"
.LASF510:
	.string	"_BWHITE_ \"\\x1b[1;37;40m\""
.LASF508:
	.string	"_BMAGENTA_ \"\\x1b[1;35;40m\""
.LASF638:
	.string	"EDEADLOCK EDEADLK"
.LASF128:
	.string	"__INT_FAST16_MAX__ 0x7fffffffffffffffL"
.LASF551:
	.string	"DBG_BUFFER_SIZE (PAGE_SIZE)"
.LASF293:
	.string	"__DEC32_MIN_EXP__ (-94)"
.LASF330:
	.string	"__GCC_ATOMIC_LLONG_LOCK_FREE 2"
.LASF642:
	.string	"ETIME 62"
.LASF736:
	.string	"kmutex"
.LASF232:
	.string	"__FLT64_MAX_EXP__ 1024"
.LASF130:
	.string	"__INT_FAST32_MAX__ 0x7fffffffffffffffL"
.LASF687:
	.string	"ENOTCONN 107"
.LASF39:
	.string	"__UINTMAX_TYPE__ long unsigned int"
.LASF87:
	.string	"__WINT_WIDTH__ 32"
.LASF599:
	.string	"ENODEV 19"
.LASF24:
	.string	"__CHAR_BIT__ 8"
.LASF339:
	.string	"__SIZEOF_WCHAR_T__ 4"
.LASF126:
	.string	"__INT_FAST8_MAX__ 0x7f"
.LASF499:
	.string	"_YELLOW_ \"\\x1b[33;40m\""
.LASF807:
	.string	"dbg_modes"
.LASF529:
	.string	"DBG_FREF DBG_MODE(16)"
.LASF818:
	.string	"kmutex_owns_mutex"
.LASF700:
	.string	"EISNAM 120"
.LASF701:
	.string	"EREMOTEIO 121"
.LASF200:
	.string	"__FLT16_MAX_EXP__ 16"
.LASF136:
	.string	"__UINT_FAST32_MAX__ 0xffffffffffffffffUL"
.LASF447:
	.string	"PT_PRESENT 0x001"
.LASF742:
	.string	"kt_kstack"
.LASF813:
	.string	"sched_queue_empty"
.LASF500:
	.string	"_BLUE_ \"\\x1b[34;40m\""
.LASF729:
	.string	"list_t"
.LASF29:
	.string	"__BYTE_ORDER__ __ORDER_LITTLE_ENDIAN__"
.LASF787:
	.string	"kc_queue"
.LASF113:
	.string	"__INT32_C(c) c"
.LASF268:
	.string	"__FLT32X_NORM_MAX__ 1.79769313486231570814527423731704357e+308F32x"
.LASF36:
	.string	"__WCHAR_TYPE__ int"
.LASF68:
	.string	"__UINTPTR_TYPE__ long unsigned int"
.LASF825:
	.string	"__next_held"
.LASF504:
	.string	"_BRED_ \"\\x1b[1;31;40m\""
.LASF37:
	.string	"__WINT_TYPE__ unsigned int"
.LASF635:
	.string	"ENOANO 55"
.LASF287:
	.string	"__FLT64X_DENORM_MIN__ 3.64519953188247460252840593361941982e-4951F64x"
.LASF396:
	.string	"list_tail(list,type,member) list_item((list)->l_prev, type, member)"
.LASF730:
	.string	"list_link_t"
.LASF351:
	.string	"__k8 1"
.LASF57:
	.string	"__UINT_LEAST32_TYPE__ unsigned int"
.LASF719:
	.string	"unsigned int"
.LASF61:
	.string	"__INT_FAST32_TYPE__ long int"
.LASF808:
	.string	"list_remove"
.LASF149:
	.string	"__FLT_MIN_EXP__ (-125)"
.LASF242:
	.string	"__FLT64_HAS_QUIET_NAN__ 1"
.LASF765:
	.string	"p_status"
.LASF577:
	.string	"INTR_SHUTDOWN 0xfd"
.LASF469:
	.string	"GDB_PT_PHYSADDR(pt,v) (pt->phys[PTE(v)] & PAGE_MASK)"
.LASF709:
	.string	"EKEYREJECTED 129"
.LASF267:
	.string	"__FLT32X_MAX__ 1.79769313486231570814527423731704357e+308F32x"
.LASF240:
	.string	"__FLT64_HAS_DENORM__ 1"
.LASF148:
	.string	"__FLT_DIG__ 6"
.LASF567:
	.string	"KASSERT_LESSEQ(l,r) KASSERT_GENERIC(l, r, lessthaneq, \"<=\")"
.LASF143:
	.string	"__FLT_EVAL_METHOD__ 0"
.LASF154:
	.string	"__FLT_MAX__ 3.40282346638528859811704183484516925e+38F"
.LASF70:
	.string	"__SCHAR_MAX__ 0x7f"
.LASF440:
	.string	"PAGE_MASK_512GB (0xffffffffffffffff << PAGE_SHIFT_512GB)"
.LASF254:
	.string	"__FLT128_EPSILON__ 1.92592994438723585305597794258492732e-34F128"
.LASF819:
	.string	"kmutex_unlock"
.LASF84:
	.string	"__LONG_WIDTH__ 64"
.LASF726:
	.string	"pid_t"
.LASF480:
	.string	"PF_HASH_SIZE 17"
.LASF249:
	.string	"__FLT128_MAX_10_EXP__ 4932"
.LASF198:
	.string	"__FLT16_MIN_EXP__ (-13)"
.LASF261:
	.string	"__FLT32X_DIG__ 15"
.LASF141:
	.string	"__GCC_IEC_559 2"
.LASF842:
	.string	"intr_enabled"
.LASF418:
	.string	"PAGE_SAME(x,y) (PAGE_ALIGN_DOWN(x) == PAGE_ALIGN_DOWN(y))"
.LASF535:
	.string	"DBG_KB DBG_MODE(22)"
.LASF602:
	.string	"EINVAL 22"
.LASF466:
	.string	"IS_PRESENT(n) ((n)&PT_PRESENT)"
.LASF341:
	.string	"__SIZEOF_PTRDIFF_T__ 8"
.LASF30:
	.string	"__FLOAT_WORD_ORDER__ __ORDER_LITTLE_ENDIAN__"
.LASF47:
	.string	"__UINT8_TYPE__ unsigned char"
.LASF42:
	.string	"__SIG_ATOMIC_TYPE__ int"
.LASF690:
	.string	"ETIMEDOUT 110"
.LASF82:
	.string	"__SHRT_WIDTH__ 16"
.LASF840:
	.string	"/ifs/CS/replicated/home/abenjell/Schoolwork/cs1670hta/weenix_64/kernel"
.LASF306:
	.string	"__DEC128_MANT_DIG__ 34"
.LASF376:
	.string	"__NTERMS__ 3"
.LASF731:
	.string	"list"
.LASF625:
	.string	"EL2NSYNC 45"
.LASF756:
	.string	"vmm_proc"
.LASF323:
	.string	"__GCC_ATOMIC_CHAR_LOCK_FREE 2"
.LASF677:
	.string	"EAFNOSUPPORT 97"
.LASF604:
	.string	"EMFILE 24"
.LASF823:
	.string	"detect_deadlocks"
.LASF788:
	.string	"kc_csdpaddr"
.LASF100:
	.string	"__INT32_MAX__ 0x7fffffff"
.LASF673:
	.string	"EPROTONOSUPPORT 93"
.LASF215:
	.string	"__FLT32_MIN_10_EXP__ (-37)"
.LASF774:
	.string	"phys"
.LASF158:
	.string	"__FLT_DENORM_MIN__ 1.40129846432481707092372958328991613e-45F"
.LASF734:
	.string	"tq_size"
.LASF292:
	.string	"__DEC32_MANT_DIG__ 7"
.LASF528:
	.string	"DBG_SYSCALL DBG_MODE(15)"
.LASF459:
	.string	"PDPE(x) ((((uintptr_t)(x)) >> 30) & INDEX_MASK)"
.LASF443:
	.string	"PAGE_CONTROL_FLAGS(x) ((x) & (PT_PRESENT | PT_WRITE | PT_USER | PT_WRITE_THROUGH | PT_CACHE_DISABLED | PT_SIZE | PT_GLOBAL))"
.LASF411:
	.string	"PAGE_MASK (0xffffffffffffffff << PAGE_SHIFT)"
.LASF521:
	.string	"DBG_INTR DBG_MODE(8)"
.LASF569:
	.string	"INTR_INVALID_OPCODE 0x06"
.LASF747:
	.string	"kt_wchan"
.LASF498:
	.string	"_GREEN_ \"\\x1b[32;40m\""
.LASF74:
	.string	"__LONG_LONG_MAX__ 0x7fffffffffffffffLL"
.LASF196:
	.string	"__FLT16_MANT_DIG__ 11"
.LASF596:
	.string	"EBUSY 16"
.LASF507:
	.string	"_BBLUE_ \"\\x1b[1;34;40m\""
.LASF487:
	.string	"VFS_ROOTFS_DEV \"disk0\""
.LASF375:
	.string	"__RENAMEDIR__ 1"
.LASF290:
	.string	"__FLT64X_HAS_QUIET_NAN__ 1"
.LASF245:
	.string	"__FLT128_DIG__ 33"
.LASF107:
	.string	"__INT8_C(c) c"
.LASF434:
	.string	"PAGE_ALIGN_UP_1GB(x) (PAGE_ALIGN_DOWN_1GB((x)-1) + PAGE_SIZE_1GB)"
.LASF219:
	.string	"__FLT32_MAX__ 3.40282346638528859811704183484516925e+38F32"
.LASF563:
	.string	"KASSERTNEQ(l,r) KASSERT_GENERIC(l, r, notequals, \"!=\")"
.LASF432:
	.string	"PAGE_SIZE_1GB ((uintptr_t)(1UL << PAGE_SHIFT_1GB))"
.LASF165:
	.string	"__DBL_MIN_EXP__ (-1021)"
.LASF45:
	.string	"__INT32_TYPE__ int"
.LASF204:
	.string	"__FLT16_NORM_MAX__ 6.55040000000000000000000000000000000e+4F16"
.LASF213:
	.string	"__FLT32_DIG__ 6"
.LASF482:
	.string	"MAX_FILES 1024"
.LASF6:
	.string	"__VERSION__ \"12.2.0\""
.LASF574:
	.string	"INTR_DISK_PRIMARY 0xd0"
.LASF479:
	.string	"KMEM_FRAC(x) (((x) >> 2) + ((x) >> 3))"
.LASF826:
	.string	"waiter"
.LASF192:
	.string	"__LDBL_HAS_DENORM__ 1"
.LASF335:
	.string	"__HAVE_SPECULATION_SAFE_VALUE 1"
.LASF320:
	.string	"__GCC_HAVE_SYNC_COMPARE_AND_SWAP_4 1"
.LASF269:
	.string	"__FLT32X_MIN__ 2.22507385850720138309023271733240406e-308F32x"
.LASF576:
	.string	"INTR_APICERR 0xff"
.LASF496:
	.string	"_BLACK_ \"\\x1b[30;47m\""
.LASF257:
	.string	"__FLT128_HAS_INFINITY__ 1"
.LASF530:
	.string	"DBG_PGTBL DBG_MODE(17)"
.LASF696:
	.string	"ESTALE 116"
.LASF243:
	.string	"__FLT64_IS_IEC_60559__ 2"
.LASF26:
	.string	"__ORDER_LITTLE_ENDIAN__ 1234"
.LASF658:
	.string	"EREMCHG 78"
.LASF299:
	.string	"__DEC64_MANT_DIG__ 16"
.LASF276:
	.string	"__FLT64X_MANT_DIG__ 64"
.LASF402:
	.string	"KMUTEX_INITIALIZER(mtx) { .km_waitq = KTQUEUE_INITIALIZER((mtx).km_waitq), .km_holder = NULL, .km_link = LIST_LINK_INITIALIZER((mtx).km_link), }"
.LASF71:
	.string	"__SHRT_MAX__ 0x7fff"
.LASF195:
	.string	"__LDBL_IS_IEC_60559__ 2"
.LASF474:
	.string	"GDB_CUR_PHYSADDR(v) GDB_PHYSADDR(curproc->p_pml4, (v))"
.LASF77:
	.string	"__WINT_MAX__ 0xffffffffU"
.LASF353:
	.string	"__code_model_large__ 1"
.LASF710:
	.string	"EOWNERDEAD 130"
.LASF283:
	.string	"__FLT64X_MAX__ 1.18973149535723176502126385303097021e+4932F64x"
.LASF67:
	.string	"__INTPTR_TYPE__ long int"
.LASF389:
	.string	"QUOTE_BY_NAME(...) #__VA_ARGS__"
.LASF207:
	.string	"__FLT16_DENORM_MIN__ 5.96046447753906250000000000000000000e-8F16"
.LASF762:
	.string	"p_pproc"
.LASF56:
	.string	"__UINT_LEAST16_TYPE__ short unsigned int"
.LASF314:
	.string	"__USER_LABEL_PREFIX__ "
.LASF40:
	.string	"__CHAR16_TYPE__ short unsigned int"
.LASF291:
	.string	"__FLT64X_IS_IEC_60559__ 2"
.LASF793:
	.string	"KT_SLEEP"
.LASF398:
	.string	"list_prev(current,type,member) list_tail(&(current)->member, type, member)"
.LASF138:
	.string	"__INTPTR_MAX__ 0x7fffffffffffffffL"
.LASF13:
	.string	"__FINITE_MATH_ONLY__ 0"
.LASF392:
	.string	"LIST_INITIALIZER(list) { .l_next = &(list), .l_prev = &(list) }"
.LASF377:
	.string	"__NDISKS__ 1"
.LASF357:
	.string	"__FXSR__ 1"
.LASF751:
	.string	"kt_mutexes"
.LASF250:
	.string	"__FLT128_DECIMAL_DIG__ 36"
.LASF431:
	.string	"PAGE_MASK_1GB (0xffffffffffffffff << PAGE_SHIFT_1GB)"
.LASF338:
	.string	"__SIZEOF_INT128__ 16"
.LASF560:
	.string	"KASSERT(x) do { if (!(x)) panic(\"assertion failed: %s\", #x); } while (0)"
.LASF800:
	.string	"proc_state_t"
.LASF829:
	.string	"intr_disable"
.LASF493:
	.string	"CORE_SPECIFIC_DATA __attribute__((section(\".csd\"))) = {0}"
.LASF495:
	.string	"_NORMAL_ \"\\x1b[0m\""
.LASF383:
	.string	"noreturn __attribute__((noreturn))"
.LASF115:
	.string	"__INT_LEAST64_MAX__ 0x7fffffffffffffffL"
.LASF388:
	.string	"CONTAINER_OF(obj,type,member) ((type *)((char *)(obj)-offsetof(type, member)))"
.LASF694:
	.string	"EALREADY 114"
.LASF583:
	.string	"ESRCH 3"
.LASF783:
	.string	"context_t"
.LASF753:
	.string	"kmutex_t"
.LASF354:
	.string	"__MMX__ 1"
.LASF382:
	.string	"inline __attribute__((always_inline, used))"
.LASF484:
	.string	"MAX_VNODES 1024"
.LASF439:
	.string	"PAGE_SIZE_512GB ((uintptr_t)(1UL << PAGE_SHIFT_512GB))"
.LASF750:
	.string	"kt_qlink"
.LASF820:
	.string	"kmutex_lock"
.LASF585:
	.string	"EIO 5"
.LASF80:
	.string	"__SIZE_MAX__ 0xffffffffffffffffUL"
.LASF125:
	.string	"__UINT64_C(c) c ## UL"
.LASF841:
	.string	"kmutex_has_waiters"
.LASF473:
	.string	"GDB_PHYSADDR(pml4,v) (GDB_PT_PHYSADDR( GDB_PD_PHYSADDR( GDB_PDP_PHYSADDR(GDB_PML4_PHYSADDR(pml4, (v)) + PHYS_OFFSET, (v)) + PHYS_OFFSET, (v)) + PHYS_OFFSET, (v)) + PHYS_OFFSET)"
.LASF561:
	.ascii	"KA"
	.string	"SSERT_GENERIC(left,right,comparator,comp_str) do { int __left = (int)(left); int __right = (int)(right); if (!comparator(__left, __right)) { panic(\"assertion failed: %s %s %s. Left: %d, Right: %d\\n\", #left, comp_str, #right, __left, __right); } } while (0)"
.LASF543:
	.string	"DBG_MEMDEV DBG_MODE(33)"
.LASF515:
	.string	"DBG_INIT DBG_MODE(2)"
.LASF679:
	.string	"EADDRNOTAVAIL 99"
.LASF58:
	.string	"__UINT_LEAST64_TYPE__ long unsigned int"
.LASF172:
	.string	"__DBL_MIN__ ((double)2.22507385850720138309023271733240406e-308L)"
.LASF669:
	.string	"EDESTADDRREQ 89"
.LASF285:
	.string	"__FLT64X_MIN__ 3.36210314311209350626267781732175260e-4932F64x"
.LASF828:
	.string	"intr_wait"
.LASF662:
	.string	"ELIBMAX 82"
.LASF526:
	.string	"DBG_PFRAME DBG_MODE(13)"
.LASF433:
	.string	"PAGE_ALIGN_DOWN_1GB(x) (((uintptr_t)(x)) & PAGE_MASK_1GB)"
.LASF627:
	.string	"EL3RST 47"
.LASF53:
	.string	"__INT_LEAST32_TYPE__ int"
.LASF286:
	.string	"__FLT64X_EPSILON__ 1.08420217248550443400745280086994171e-19F64x"
.LASF4:
	.string	"__GNUC_MINOR__ 2"
.LASF419:
	.string	"PAGE_NSIZES 8"
.LASF182:
	.string	"__LDBL_MIN_10_EXP__ (-4931)"
.LASF328:
	.string	"__GCC_ATOMIC_INT_LOCK_FREE 2"
.LASF582:
	.string	"ENOENT 2"
.LASF537:
	.string	"DBG_PRINT DBG_MODE(24)"
.LASF270:
	.string	"__FLT32X_EPSILON__ 2.22044604925031308084726333618164062e-16F32x"
.LASF619:
	.string	"ENOTEMPTY 39"
.LASF177:
	.string	"__DBL_HAS_QUIET_NAN__ 1"
.LASF372:
	.string	"__S5FS__ 1"
.LASF674:
	.string	"ESOCKTNOSUPPORT 94"
.LASF587:
	.string	"E2BIG 7"
.LASF449:
	.string	"PT_USER 0x004"
.LASF680:
	.string	"ENETDOWN 100"
.LASF356:
	.string	"__SSE2__ 1"
.LASF255:
	.string	"__FLT128_DENORM_MIN__ 6.47517511943802511092443895822764655e-4966F128"
.LASF513:
	.string	"DBG_CORE DBG_MODE(0)"
.LASF313:
	.string	"__REGISTER_PREFIX__ "
.LASF312:
	.string	"__DEC128_SUBNORMAL_MIN__ 0.000000000000000000000000000000001E-6143DL"
.LASF164:
	.string	"__DBL_DIG__ 15"
.LASF773:
	.string	"p_vmmap"
.LASF644:
	.string	"ENONET 64"
.LASF23:
	.string	"__SIZEOF_SIZE_T__ 8"
.LASF238:
	.string	"__FLT64_EPSILON__ 2.22044604925031308084726333618164062e-16F64"
.LASF525:
	.string	"DBG_VNREF DBG_MODE(12)"
.LASF362:
	.string	"__SEG_GS 1"
.LASF236:
	.string	"__FLT64_NORM_MAX__ 1.79769313486231570814527423731704357e+308F64"
.LASF369:
	.string	"__DECIMAL_BID_FORMAT__ 1"
.LASF629:
	.string	"EUNATCH 49"
.LASF632:
	.string	"EBADE 52"
.LASF349:
	.string	"__ATOMIC_HLE_RELEASE 131072"
.LASF63:
	.string	"__UINT_FAST8_TYPE__ unsigned char"
.LASF532:
	.string	"DBG_EXEC DBG_MODE(19)"
.LASF105:
	.string	"__UINT64_MAX__ 0xffffffffffffffffUL"
.LASF667:
	.string	"EUSERS 87"
.LASF346:
	.string	"__SIZEOF_FLOAT80__ 16"
.LASF358:
	.string	"__SSE_MATH__ 1"
.LASF717:
	.string	"short int"
.LASF799:
	.string	"PROC_DEAD"
.LASF79:
	.string	"__PTRDIFF_MAX__ 0x7fffffffffffffffL"
.LASF121:
	.string	"__UINT16_C(c) c"
.LASF230:
	.string	"__FLT64_MIN_EXP__ (-1021)"
.LASF205:
	.string	"__FLT16_MIN__ 6.10351562500000000000000000000000000e-5F16"
.LASF722:
	.string	"uint64_t"
.LASF316:
	.string	"__NO_INLINE__ 1"
.LASF689:
	.string	"ETOOMANYREFS 109"
.LASF802:
	.string	"vnode"
.LASF618:
	.string	"ENOSYS 38"
.LASF280:
	.string	"__FLT64X_MAX_EXP__ 16384"
.LASF7:
	.string	"__ATOMIC_RELAXED 0"
.LASF387:
	.string	"MAX(a,b) ((a) > (b) ? (a) : (b))"
.LASF220:
	.string	"__FLT32_NORM_MAX__ 3.40282346638528859811704183484516925e+38F32"
.LASF562:
	.string	"KASSERTEQ(l,r) KASSERT_GENERIC(l, r, equals, \"==\")"
.LASF95:
	.string	"__SIG_ATOMIC_MAX__ 0x7fffffff"
.LASF809:
	.string	"sched_wakeup_on"
.LASF322:
	.string	"__GCC_ATOMIC_BOOL_LOCK_FREE 2"
.LASF603:
	.string	"ENFILE 23"
.LASF64:
	.string	"__UINT_FAST16_TYPE__ long unsigned int"
.LASF472:
	.string	"GDB_PML4_PHYSADDR(pml4,v) (pml4->phys[PML4E(v)] & PAGE_MASK)"
.LASF584:
	.string	"EINTR 4"
.LASF550:
	.ascii	"DBG_TAB {\"error\", DBG_ERROR, _BWHITE_}, {\"temp\", DBG_TEM"
	.ascii	"P, _NORMAL_}, {\"print\", DBG_PRINT, _NORMAL_}, {\"test\", D"
	.ascii	"BG_TEST, _RED_}, {\"testpass\", DBG_TESTPASS, _GREEN_}, {\"t"
	.ascii	"estfail\", DBG_TESTFAIL, _RED_}, {\"proc\", DBG_PROC, _BLUE_"
	.ascii	"}, {\"thr\", DBG_THR, _CYAN_}, {\"sched\", DBG_SCHED, _GREEN"
	.ascii	"_}, {\"init\", DBG_INIT, _NORMAL_}, {\"term\", DBG_TERM, _BM"
	.ascii	"AGENTA_}, {\"disk\", DBG_DISK, _YELLOW_}, {\"memdev\", DBG_M"
	.ascii	"EMDEV, _BBLUE_}, {\"vfs\", DBG_VFS, _WHITE_}, {\"fref\", DBG"
	.ascii	"_FREF, _MAGENTA_}, {\"vnref\", DBG_VNREF, _CYAN_}, {\"s5fs\""
	.ascii	", DBG_S5FS, _BRED_}, {\"pframe\", DBG_PFRAME, _BMAGENTA_}, {"
	.ascii	"\"anon\", DBG_ANON, _WHITE_}, {\"vmmap\", DBG_VMMAP, _BGREEN"
	.ascii	"_}, {\"fork\", DBG_FORK, _BYELLOW_}, {\"brk\", DBG_BRK, _YEL"
	.ascii	"LOW_}, {\"exec\", DBG_EXEC, _BRED_}, {\"elf\", DBG_ELF, _BGR"
	.ascii	"EEN_}, {\"pgtbl\", DBG_PGTBL, _BBLUE_}, {\"osyscall\", DBG_O"
	.ascii	"SYSCALL, _BMAGENTA_}, {\"vm\", DBG_VM, _RED_}, {\"btree\", D"
	.ascii	"BG_BTREE, _NORMAL_}, {\"syscall\", DBG_SYSCALL, _RED_}, "
	.string	"{\"intr\", DBG_INTR, _BRED_}, {\"kmalloc\", DBG_KMALLOC, _MAGENTA_}, {\"pagealloc\", DBG_PAGEALLOC, _WHITE_}, {\"kb\", DBG_KB, _BLUE_}, {\"core\", DBG_CORE, _GREEN_}, {\"mm\", DBG_MM, _RED_}, {\"user\", DBG_USER, _BYELLOW_}, {\"all\", DBG_ALL, _NORMAL_}, { NULL, 0, NULL }"
.LASF634:
	.string	"EXFULL 54"
.LASF20:
	.string	"__SIZEOF_FLOAT__ 4"
.LASF663:
	.string	"ELIBEXEC 83"
.LASF302:
	.string	"__DEC64_MIN__ 1E-383DD"
.LASF647:
	.string	"ENOLINK 67"
.LASF666:
	.string	"ESTRPIPE 86"
.LASF169:
	.string	"__DBL_DECIMAL_DIG__ 17"
.LASF738:
	.string	"km_holder"
.LASF415:
	.string	"PAGE_ALIGNED(x) (!PAGE_OFFSET(x))"
.LASF416:
	.string	"PN_TO_ADDR(x) ((void *)(((uintptr_t)(x)) << PAGE_SHIFT))"
.LASF672:
	.string	"ENOPROTOOPT 92"
.LASF475:
	.string	"GET_CSD(core,type,name) ((type *)(csd_vaddr_table[(core)] + PAGE_OFFSET(&(name))))"
.LASF171:
	.string	"__DBL_NORM_MAX__ ((double)1.79769313486231570814527423731704357e+308L)"
.LASF99:
	.string	"__INT16_MAX__ 0x7fff"
.LASF429:
	.string	"PAGE_SAME_2MB(x,y) (PAGE_ALIGN_DOWN_2MB(x) == PAGE_ALIGN_DOWN_2MB(y))"
.LASF586:
	.string	"ENXIO 6"
.LASF379:
	.string	"SPINLOCK_INITIALIZER(lock) { .s_locked = 0 }"
.LASF83:
	.string	"__INT_WIDTH__ 32"
.LASF573:
	.string	"INTR_KEYBOARD 0xe0"
.LASF437:
	.string	"PAGE_SAME_1GB(x,y) (PAGE_ALIGN_DOWN_1GB(x) == PAGE_ALIGN_DOWN_1GB(y))"
.LASF244:
	.string	"__FLT128_MANT_DIG__ 113"
.LASF142:
	.string	"__GCC_IEC_559_COMPLEX 2"
.LASF97:
	.string	"__SIG_ATOMIC_WIDTH__ 32"
.LASF645:
	.string	"ENOPKG 65"
.LASF445:
	.string	"VMMAP_DIR_LOHI 1"
.LASF188:
	.string	"__LDBL_NORM_MAX__ 1.18973149535723176502126385303097021e+4932L"
.LASF789:
	.string	"core_t"
.LASF340:
	.string	"__SIZEOF_WINT_T__ 4"
.LASF733:
	.string	"tq_list"
.LASF643:
	.string	"ENOSR 63"
.LASF0:
	.string	"__STDC__ 1"
.LASF427:
	.string	"PAGE_OFFSET_2MB(x) (((uintptr_t)(x)) & ~PAGE_MASK_2MB)"
.LASF754:
	.string	"vmmap"
.LASF41:
	.string	"__CHAR32_TYPE__ unsigned int"
.LASF224:
	.string	"__FLT32_HAS_DENORM__ 1"
.LASF477:
	.string	"DEFAULT_STACK_SIZE (DEFAULT_STACK_SIZE_PAGES << PAGE_SHIFT)"
.LASF615:
	.string	"EDEADLK 35"
.LASF50:
	.string	"__UINT64_TYPE__ long unsigned int"
.LASF284:
	.string	"__FLT64X_NORM_MAX__ 1.18973149535723176502126385303097021e+4932F64x"
.LASF22:
	.string	"__SIZEOF_LONG_DOUBLE__ 16"
.LASF518:
	.string	"DBG_TEMP DBG_MODE(5)"
.LASF772:
	.string	"p_start_brk"
.LASF35:
	.string	"__PTRDIFF_TYPE__ long int"
.LASF251:
	.string	"__FLT128_MAX__ 1.18973149535723176508575932662800702e+4932F128"
.LASF624:
	.string	"ECHRNG 44"
.LASF763:
	.string	"p_list_link"
.LASF235:
	.string	"__FLT64_MAX__ 1.79769313486231570814527423731704357e+308F64"
.LASF564:
	.string	"KASSERT_GREATER(l,r) KASSERT_GENERIC(l, r, greaterthan, \">\")"
.LASF256:
	.string	"__FLT128_HAS_DENORM__ 1"
.LASF516:
	.string	"DBG_SCHED DBG_MODE(3)"
.LASF652:
	.string	"EMULTIHOP 72"
.LASF203:
	.string	"__FLT16_MAX__ 6.55040000000000000000000000000000000e+4F16"
.LASF73:
	.string	"__LONG_MAX__ 0x7fffffffffffffffL"
.LASF333:
	.string	"__GCC_CONSTRUCTIVE_SIZE 64"
.LASF566:
	.string	"KASSERT_GREQ(l,r) KASSERT_GENERIC(l, r, greaterthaneq, \">=\")"
.LASF749:
	.string	"kt_plink"
.LASF390:
	.string	"QUOTE_BY_VALUE(x) QUOTE_BY_NAME(x)"
.LASF424:
	.string	"PAGE_MASK_2MB (0xffffffffffffffff << PAGE_SHIFT_2MB)"
.LASF824:
	.string	"held"
.LASF94:
	.string	"__INTMAX_WIDTH__ 64"
.LASF792:
	.string	"KT_RUNNABLE"
.LASF197:
	.string	"__FLT16_DIG__ 3"
.LASF140:
	.string	"__UINTPTR_MAX__ 0xffffffffffffffffUL"
.LASF350:
	.string	"__GCC_ASM_FLAG_OUTPUTS__ 1"
.LASF698:
	.string	"ENOTNAM 118"
.LASF342:
	.string	"__amd64 1"
.LASF556:
	.string	"dbgq(mode,...) do { if (dbg_active(mode)) { DEBUG_ENTER dbg_print(\"%s\", dbg_color(mode)); dbg_print(\"C%ld P%ld \", curcore.kc_id, curproc ? curproc->p_pid : -1L); dbg_print(__VA_ARGS__); dbg_print(\"%s\", _NORMAL_); DEBUG_EXIT } } while (0)"
.LASF189:
	.string	"__LDBL_MIN__ 3.36210314311209350626267781732175260e-4932L"
.LASF367:
	.string	"__unix__ 1"
.LASF489:
	.string	"PROC_MAX_COUNT 65536"
.LASF617:
	.string	"ENOLCK 37"
.LASF755:
	.string	"vmm_list"
.LASF571:
	.string	"INTR_PAGE_FAULT 0x0e"
.LASF180:
	.string	"__LDBL_DIG__ 18"
.LASF486:
	.string	"NFILES 32"
.LASF78:
	.string	"__WINT_MIN__ 0U"
.LASF598:
	.string	"EXDEV 18"
.LASF229:
	.string	"__FLT64_DIG__ 15"
.LASF304:
	.string	"__DEC64_EPSILON__ 1E-15DD"
.LASF156:
	.string	"__FLT_MIN__ 1.17549435082228750796873653722224568e-38F"
.LASF108:
	.string	"__INT_LEAST8_WIDTH__ 8"
.LASF52:
	.string	"__INT_LEAST16_TYPE__ short int"
.LASF455:
	.string	"PT_GLOBAL 0x100"
.LASF718:
	.string	"short unsigned int"
.LASF225:
	.string	"__FLT32_HAS_INFINITY__ 1"
.LASF621:
	.string	"EWOULDBLOCK EAGAIN"
.LASF181:
	.string	"__LDBL_MIN_EXP__ (-16381)"
.LASF519:
	.string	"DBG_KMALLOC DBG_MODE(6)"
.LASF173:
	.string	"__DBL_EPSILON__ ((double)2.22044604925031308084726333618164062e-16L)"
.LASF347:
	.string	"__SIZEOF_FLOAT128__ 16"
.LASF442:
	.string	"PAGE_ALIGN_UP_512GB(x) (PAGE_ALIGN_DOWN_512GB((x)-1) + PAGE_SIZE_512GB)"
.LASF542:
	.string	"DBG_TESTFAIL DBG_MODE(32)"
.LASF426:
	.string	"PAGE_ALIGN_UP_2MB(x) (PAGE_ALIGN_DOWN_2MB((x)-1) + PAGE_SIZE_2MB)"
.LASF122:
	.string	"__UINT_LEAST32_MAX__ 0xffffffffU"
.LASF837:
	.string	"equals"
.LASF806:
	.string	"curthr"
.LASF648:
	.string	"EADV 68"
.LASF260:
	.string	"__FLT32X_MANT_DIG__ 53"
.LASF745:
	.string	"kt_proc"
.LASF704:
	.string	"EMEDIUMTYPE 124"
.LASF86:
	.string	"__WCHAR_WIDTH__ 32"
.LASF613:
	.string	"EDOM 33"
.LASF626:
	.string	"EL3HLT 46"
.LASF110:
	.string	"__INT16_C(c) c"
.LASF578:
	.string	"IPL_LOW 0"
.LASF277:
	.string	"__FLT64X_DIG__ 18"
.LASF724:
	.string	"uintptr_t"
.LASF707:
	.string	"EKEYEXPIRED 127"
.LASF210:
	.string	"__FLT16_HAS_QUIET_NAN__ 1"
.LASF522:
	.string	"DBG_TERM DBG_MODE(9)"
.LASF697:
	.string	"EUCLEAN 117"
.LASF11:
	.string	"__ATOMIC_ACQ_REL 4"
.LASF759:
	.string	"p_name"
.LASF631:
	.string	"EL2HLT 51"
.LASF364:
	.string	"__linux 1"
.LASF166:
	.string	"__DBL_MIN_10_EXP__ (-307)"
.LASF131:
	.string	"__INT_FAST32_WIDTH__ 64"
.LASF174:
	.string	"__DBL_DENORM_MIN__ ((double)4.94065645841246544176568792868221372e-324L)"
.LASF659:
	.string	"ELIBACC 79"
.LASF713:
	.string	"DEBUG_DEADLOCKS 1"
.LASF688:
	.string	"ESHUTDOWN 108"
.LASF408:
	.string	"MEMORY_MAP_BASE 0x9000"
.LASF458:
	.string	"PML4E(x) ((((uintptr_t)(x)) >> 39) & INDEX_MASK)"
.LASF430:
	.string	"PAGE_SHIFT_1GB 30"
.LASF163:
	.string	"__DBL_MANT_DIG__ 53"
.LASF835:
	.string	"lessthan"
.LASF72:
	.string	"__INT_MAX__ 0x7fffffff"
.LASF124:
	.string	"__UINT_LEAST64_MAX__ 0xffffffffffffffffUL"
.LASF656:
	.string	"ENOTUNIQ 76"
.LASF400:
	.string	"list_iterate_reverse(list,var,type,member) for (type *var = list_tail(list, type, member), *__next_ ##var = list_prev(var, type, member); &var->member != (list); var = __next_ ##var, __next_ ##var = list_prev(var, type, member))"
.LASF653:
	.string	"EDOTDOT 73"
.LASF766:
	.string	"p_state"
	.ident	"GCC: (Debian 12.2.0-14) 12.2.0"
	.section	.note.GNU-stack,"",@progbits
