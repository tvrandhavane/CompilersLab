
	.data
a:	.word 0
b:	.word 0

	.text 			# The .text assembler directive indicates
	.globl fn		# The following is the code (as oppose to data)
fn:				# .globl makes main know to the 
				# outside of the program.
# Prologue begins 
	sw $ra, 0($sp)		# Save the return address
	sw $fp, -4($sp)		# Save the frame pointer
	sub $fp, $sp, 8		# Update the frame pointer

	sub $sp, $sp, 20		# Make space for the locals
# Prologue ends


label2:    	
	lw $v0, 12($fp)
	sw $v0, -8($fp)
	l.d $f2, 20($fp)
	s.d $f2, 0($fp)
	j epilogue_fn

# Epilogue Begins
epilogue_fn:
	add $sp, $sp, 20
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

	sub $sp, $sp, 8		# Make space for the locals
# Prologue ends


label2:    	
	l.d $f2, b
	s.d $f2, 0($sp)
	lw $v0, a
	sw $v0, -8($sp)
	sub $sp, $sp, 12
	jal fn
	add $sp, $sp, 12
	j epilogue_main

# Epilogue Begins
epilogue_main:
	add $sp, $sp, 8
	lw $fp, -4($sp)  
	lw $ra, 0($sp)   
	jr        $31		# Jump back to the called procedure
# Epilogue Ends

