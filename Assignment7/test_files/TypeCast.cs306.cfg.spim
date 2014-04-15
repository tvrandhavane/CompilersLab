
	.text 			# The .text assembler directive indicates
	.globl main		# The following is the code (as oppose to data)
main:				# .globl makes main know to the 
				# outside of the program.
# Prologue begins 
	sw $ra, 0($sp)		# Save the return address
	sw $fp, -4($sp)		# Save the frame pointer
	sub $fp, $sp, 8		# Update the frame pointer

	sub $sp, $sp, 40		# Make space for the locals
# Prologue ends


label2:    	
	li $v0, 3
	sw $v0, -24($fp)
	li.d $f2, 2.30
	s.d $f2, 0($fp)
	lw $v0, -24($fp)
	mtc1 $f2, $v0
	l.d $f4, 0($fp)
	div.d $f6, $f2, $f4
	mfc1 $v0, $f6
	sw $v0, -28($fp)
	j epilogue_main

# Epilogue Begins
epilogue_main:
	add $sp, $sp, 40
	lw $fp, -4($sp)  
	lw $ra, 0($sp)   
	jr        $31		# Jump back to the called procedure
# Epilogue Ends

