 /**
  * Interrupt Vector Table for ISRs
  * @author Curt Henrichs
  * @date 1-26-16, revised 3-10-17
  * @file exceptions.S
  *
  * PURPOSE: Runs the exception handler. Only thing needed to modify is the
  * irq_table which is the vecotr table for the IRSs
  * TESTED: 2-16-16: Exception handler now saves all necessary registers,
  * therefore protecting the program from breaking an instruction.
  */

 //make isr table global
.global IRQ_0
.global IRQ_1
.global IRQ_2
.global IRQ_3
.global IRQ_4
.global IRQ_5
.global IRQ_6
.global IRQ_7
.global IRQ_8

irq_isr_table:
IRQ_0:
	.word default_isr //timer0
IRQ_1:
	.word default_isr //pushbutton
IRQ_2:
	.word default_isr
IRQ_3:
	.word default_isr
IRQ_4:
	.word default_isr
IRQ_5:
	.word default_isr
IRQ_6:
	.word default_isr
IRQ_7:
	.word default_isr
IRQ_8:
	.word default_isr
	IRQ_ISR_TABLE_ENTRIES = (. - irq_isr_table) / 4

// ** Exception Section **
.section .exceptions, "ax"

/*
The Monitor Program automatically places the ".exceptions' section at the
exception location specified in the CPU settings in SOPC Builder.
Note: "ax" is REQUIRED to designate the section as allocatable and executable.
*/

/**
 * Exception handler for the NIOS processor.
*/
.global exception_handler
exception_handler:
	//Save to stack
	subi sp, sp, 28 * 4
	stw et, 0 * 4(sp)
	rdctl et, ipending
	// Check if interrupt is external
	beq et, r0, skip_ea_dec
	// Decrement ea by one instruction for external interrupts
	// so the interrupted instruction wil be started again
	// adter eret
	subi ea, ea, 4
skip_ea_dec:
	// Save all registers than are normally caller saved
	stw ea, 4*1(sp)
	stw r1, 4*2(sp)
	stw r2, 4*3(sp)
	stw r3, 4*4(sp)
	stw r4, 4*5(sp)
	stw r5, 4*6(sp)
	stw r5, 4*7(sp)
	stw r6, 4*8(sp)
	stw r7, 4*9(sp)
	stw r8, 4*10(sp)
	stw r9, 4*11(sp)
	stw r10, 4*12(sp)
	stw r11, 4*13(sp)
	stw r12, 4*14(sp)
	stw r13, 4*15(sp)
	stw r14, 4*16(sp)
	stw r15, 4*17(sp)
	stw r16, 4*18(sp)
	stw r17, 4*19(sp)
	stw r18, 4*20(sp)
	stw r19, 4*21(sp)
	stw r20, 4*22(sp)
	stw r21, 4*23(sp)
	stw r22, 4*24(sp)
	stw r23, 4*25(sp)
	stw r28, 4*26(sp) // fp
	stw r31, 4*27(sp) // ra

	rdctl et, ipending
	// Check if interrupt is external
	bne et, zero, check_irqs
not_ei:
	// Exception must be unimplemented instruction or
	// trap instruction, do not handle
	br end_isr

check_irqs:
	// Check levels and dispatch to ISR for the IRQ
	movui r9, 1
	movui r10, IRQ_ISR_TABLE_ENTRIES
	movia r11, irq_isr_table
check_level_loop:
	beq r10, zero, end_isr
	// Check for interrupt level N
	and r8, et, r9
	beq r8, zero, irq_level_not_pending
	ldw r12, 0(r11)
	callr r12
	br end_isr
irq_level_not_pending:
	addi r11, r11, 4
	slli r9, r9, 1
	subi r10, r10, 1
	br check_level_loop

end_isr:
	// Restore from stack
	ldw et, 0*4(sp)
	ldw ea, 1*4(sp)
	ldw r1, 4*2(sp)
	ldw r2, 4*3(sp)
	ldw r3, 4*4(sp)
	ldw r4, 4*5(sp)
	ldw r5, 4*6(sp)
	ldw r5, 4*7(sp)
	ldw r6, 4*8(sp)
	ldw r7, 4*9(sp)
	ldw r8, 4*10(sp)
	ldw r9, 4*11(sp)
	ldw r10, 4*12(sp)
	ldw r11, 4*13(sp)
	ldw r12, 4*14(sp)
	ldw r13, 4*15(sp)
	ldw r14, 4*16(sp)
	ldw r15, 4*17(sp)
	ldw r16, 4*18(sp)
	ldw r17, 4*19(sp)
	ldw r18, 4*20(sp)
	ldw r19, 4*21(sp)
	ldw r20, 4*22(sp)
	ldw r21, 4*23(sp)
	ldw r22, 4*24(sp)
	ldw r23, 4*25(sp)
	ldw r28, 4*26(sp) // fp
	ldw r31, 4*27(sp) // ra
	addi sp, sp, 28 * 4
	eret

/**
 * Default noop interrupt service routine
*/
.global default_isr
default_isr:
	// Save to stack
	subi sp, sp, 1*4
	stw ra, 0*4(sp)

	// Restore from stack
	ldw ra, 0, 0*4(sp)
	addi sp, sp, 1 * 4
	ret


