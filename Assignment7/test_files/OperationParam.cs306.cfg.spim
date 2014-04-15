
	.text 			# The .text assembler directive indicates
	.globl fn		# The following is the code (as oppose to data)
fn:				# .globl makes main know to the 
				# outside of the program.
# Prologue begins 
	sw $ra, 0($sp)		# Save the return address
	sw $fp, -4($sp)		# Save the frame pointer
	sub $fp, $sp, 8		# Update the frame pointer

	sub $sp, $sp, 12		# Make space for the locals
# Prologue ends


label2:    	
	lw $v0, 12($fp)
	sw $v0, 0($fp)
	j epilogue_fn

# Epilogue Begins
epilogue_fn:
	add $sp, $sp, 12
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

	sub $sp, $sp, 16		# Make space for the locals
# Prologue ends


label2:    	
	li $v0, 10
	sw $v0, -4($fp)
	li $v0, 2
	sw $v0, 0($fp)
	lw $v0, 0($fp)
	lw $t0, -4($fp)
	mul $t1, $v0, $t0
	lw $v0, 0($fp)
	div $t0, $t1, $v0
	lw $v0, -4($fp)
	add $t1, $t0, $v0
	lw $v0, -4($fp)
	lw $t0, 0($fp)
	add $t2, $v0, $t0
	sle $v0, $t1, $t2
	sw $v0, 0($sp)
	sub $sp, $sp, 4
	jal fn
	add $sp, $sp, 4
	j epilogue_main

# Epilogue Begins
epilogue_main:
	add $sp, $sp, 16
	lw $fp, -4($sp)  
	lw $ra, 0($sp)   
	jr        $31		# Jump back to the called procedure
# Epilogue Ends

