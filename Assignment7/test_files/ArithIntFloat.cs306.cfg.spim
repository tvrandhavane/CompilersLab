
	.text 			# The .text assembler directive indicates
	.globl main		# The following is the code (as oppose to data)
main:				# .globl makes main know to the 
				# outside of the program.
# Prologue begins 
	sw $ra, 0($sp)		# Save the return address
	sw $fp, -4($sp)		# Save the frame pointer
	sub $fp, $sp, 8		# Update the frame pointer

	sub $sp, $sp, 28		# Make space for the locals
# Prologue ends


label2:    	
	li $v0, 2
	sw $v0, -16($fp)
	li.d $f2, 3.40
	s.d $f2, -8($fp)
	lw $v0, -16($fp)
	mtc1 $f2, $v0
	l.d $f4, -8($fp)
	lw $v0, -16($fp)
	mtc1 $f6, $v0
	div.d $f8, $f4, $f6
	add.d $f4, $f2, $f8
	s.d $f4, 0($fp)
	j epilogue_main

# Epilogue Begins
epilogue_main:
	add $sp, $sp, 28
	lw $fp, -4($sp)  
	lw $ra, 0($sp)   
	jr        $31		# Jump back to the called procedure
# Epilogue Ends

