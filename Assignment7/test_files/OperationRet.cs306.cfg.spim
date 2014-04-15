
	.text 			# The .text assembler directive indicates
	.globl fn		# The following is the code (as oppose to data)
fn:				# .globl makes main know to the 
				# outside of the program.
# Prologue begins 
	sw $ra, 0($sp)		# Save the return address
	sw $fp, -4($sp)		# Save the frame pointer
	sub $fp, $sp, 8		# Update the frame pointer

	sub $sp, $sp, 8		# Make space for the locals
# Prologue ends


label2:    	
	lw $v0, 12($fp)
	mtc1 $f2, $v0
	l.d $f4, 20($fp)
	add.d $f6, $f2, $f4
	lw $v0, 24($fp)
	mtc1 $f2, $v0
	sub.d $f4, $f6, $f2
	l.d $f2, 32($fp)
	sle $v0, $f4, $f2
	lw $t0, 12($fp)
	lw $t1, 24($fp)
	mul $t2, $t0, $t1
	sne $t0, $v0, $t2
	move $v1, $t0
	j epilogue_fn

# Epilogue Begins
epilogue_fn:
	add $sp, $sp, 8
	lw $fp, -4($sp)  
	lw $ra, 0($sp)   
	jr        $31		# Jump back to the called procedure
# Epilogue Ends


	.text 			# The .text assembler directive indicates
	.globl main		# The following is the code (as oppose to data)
main:				# .globl makes main know to the 
				# outside of the program.
# Prologue begins 
	sw $ra, 0($sp)		# Save the return address
	sw $fp, -4($sp)		# Save the frame pointer
	sub $fp, $sp, 8		# Update the frame pointer

	sub $sp, $sp, 12		# Make space for the locals
# Prologue ends


label2:    	
	li $v0, 10
	sw $v0, 0($fp)
	li.d $f2, 3.60
	s.d $f2, 0($sp)
	lw $v0, 0($fp)
	sw $v0, -8($sp)
	li.d $f2, 2.30
	s.d $f2, -12($sp)
	li $v0, 1
	sw $v0, -20($sp)
	sub $sp, $sp, 24
	jal fn
	add $sp, $sp, 24
	move $v0, $v1
	sw $v0, 0($fp)
	j epilogue_main

# Epilogue Begins
epilogue_main:
	add $sp, $sp, 12
	lw $fp, -4($sp)  
	lw $ra, 0($sp)   
	jr        $31		# Jump back to the called procedure
# Epilogue Ends

