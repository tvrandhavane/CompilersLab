
	.text 			# The .text assembler directive indicates
	.globl main		# The following is the code (as oppose to data)
main:				# .globl makes main know to the 
				# outside of the program.
# Prologue begins 
	sw $ra, 0($sp)		# Save the return address
	sw $fp, -4($sp)		# Save the frame pointer
	sub $fp, $sp, 8		# Update the frame pointer

	sub $sp, $sp, 32		# Make space for the locals
# Prologue ends


label2:    	
	li $v0, 2
	sw $v0, -20($fp)
	li $v0, 4
	sw $v0, -16($fp)
	li $v0, 10
	sw $v0, -12($fp)
	li $v0, 3
	sw $v0, -8($fp)
	li $v0, 8
	sw $v0, -4($fp)
	li $v0, 7
	sw $v0, 0($fp)
	lw $v0, -12($fp)
	lw $t0, -8($fp)
	div $t1, $v0, $t0
	lw $v0, -16($fp)
	add $t0, $t1, $v0
	lw $v0, -20($fp)
	sle $t1, $t0, $v0
	lw $v0, -4($fp)
	lw $t0, -20($fp)
	sub $t2, $v0, $t0
	slt $v0, $t1, $t2
	lw $t0, 0($fp)
	sne $t1, $v0, $t0
	bne $t1, $zero, label3 
	j label4

label3:    	
	lw $v0, -8($fp)
	lw $t0, -4($fp)
	mul $t1, $v0, $t0
	lw $v0, 0($fp)
	div $t0, $t1, $v0
	lw $v0, -16($fp)
	sle $t1, $t0, $v0
	lw $v0, -20($fp)
	add $t0, $t1, $v0
	sw $t0, -20($fp)
	j label5

label4:    	
	li $v0, 1
	sw $v0, -16($fp)

label5:    	
	j epilogue_main

# Epilogue Begins
epilogue_main:
	add $sp, $sp, 32
	lw $fp, -4($sp)  
	lw $ra, 0($sp)   
	jr        $31		# Jump back to the called procedure
# Epilogue Ends

