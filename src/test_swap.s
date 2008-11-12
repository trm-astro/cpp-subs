	.file	"test_swap.cc"
	.section	.ctors,"aw",@progbits
	.align 4
	.long	_GLOBAL__I_main
	.local	_ZSt8__ioinit
	.comm	_ZSt8__ioinit,1,1
	.text
	.align 2
	.p2align 4,,15
.globl main
	.type	main, @function
main:
.LFB1456:
	pushl	%ebp
.LCFI0:
	movl	$200000000, %ecx
	movl	%esp, %ebp
.LCFI1:
	subl	$24, %esp
.LCFI2:
	andl	$-16, %esp
	subl	$16, %esp
	movw	$0, -2(%ebp)
	.p2align 4,,7
.L2:
	movw	-2(%ebp), %ax
	incl	%eax
	movw	%ax, -2(%ebp)
	movw	-2(%ebp), %dx
	movw	-2(%ebp), %ax
	shrw	$8, %dx
	sall	$8, %eax
	orl	%edx, %eax
	decl	%ecx
	movw	%ax, -4(%ebp)
	jne	.L2
	xorl	%eax, %eax
	leave
	ret
.LFE1456:
	.size	main, .-main
	.align 2
	.p2align 4,,15
	.type	__tcf_0, @function
__tcf_0:
.LFB1464:
	pushl	%ebp
.LCFI3:
	movl	%esp, %ebp
.LCFI4:
	movl	$_ZSt8__ioinit, 8(%ebp)
	popl	%ebp
.LCFI5:
	jmp	_ZNSt8ios_base4InitD1Ev
.LFE1464:
	.size	__tcf_0, .-__tcf_0
	.align 2
	.p2align 4,,15
	.type	_Z41__static_initialization_and_destruction_0ii, @function
_Z41__static_initialization_and_destruction_0ii:
.LFB1463:
	pushl	%ebp
.LCFI6:
	cmpl	$65535, %edx
	sete	%dl
	decl	%eax
	movl	%esp, %ebp
.LCFI7:
	sete	%al
	pushl	%ecx
	testb	%al, %dl
	pushl	%ecx
	jne	.L14
	leave
	ret
	.p2align 4,,7
.L14:
	subl	$12, %esp
	pushl	$_ZSt8__ioinit
.LCFI8:
	call	_ZNSt8ios_base4InitC1Ev
	addl	$12, %esp
	pushl	$__dso_handle
	pushl	$0
	pushl	$__tcf_0
	call	__cxa_atexit
	addl	$16, %esp
	leave
	ret
.LFE1463:
	.size	_Z41__static_initialization_and_destruction_0ii, .-_Z41__static_initialization_and_destruction_0ii
	.align 2
	.p2align 4,,15
	.type	_GLOBAL__I_main, @function
_GLOBAL__I_main:
.LFB1465:
	pushl	%ebp
.LCFI9:
	movl	$65535, %edx
	movl	$1, %eax
	movl	%esp, %ebp
.LCFI10:
	popl	%ebp
	jmp	_Z41__static_initialization_and_destruction_0ii
.LFE1465:
	.size	_GLOBAL__I_main, .-_GLOBAL__I_main
	.weak	pthread_mutex_unlock
	.weak	pthread_mutex_trylock
	.weak	pthread_mutex_lock
	.weak	pthread_cancel
	.weak	pthread_setspecific
	.weak	pthread_getspecific
	.weak	pthread_key_delete
	.weak	pthread_key_create
	.weak	pthread_once
	.section	.eh_frame,"a",@progbits
.Lframe1:
	.long	.LECIE1-.LSCIE1
.LSCIE1:
	.long	0x0
	.byte	0x1
	.string	"zP"
	.uleb128 0x1
	.sleb128 -4
	.byte	0x8
	.uleb128 0x5
	.byte	0x0
	.long	__gxx_personality_v0
	.byte	0xc
	.uleb128 0x4
	.uleb128 0x4
	.byte	0x88
	.uleb128 0x1
	.align 4
.LECIE1:
.LSFDE5:
	.long	.LEFDE5-.LASFDE5
.LASFDE5:
	.long	.LASFDE5-.Lframe1
	.long	.LFB1463
	.long	.LFE1463-.LFB1463
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI6-.LFB1463
	.byte	0xe
	.uleb128 0x8
	.byte	0x85
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI7-.LCFI6
	.byte	0xd
	.uleb128 0x5
	.byte	0x4
	.long	.LCFI8-.LCFI7
	.byte	0x2e
	.uleb128 0x10
	.align 4
.LEFDE5:
	.ident	"GCC: (GNU) 4.0.1 (4.0.1-5mdk for Mandriva Linux release 2006.0)"
	.section	.note.GNU-stack,"",@progbits
