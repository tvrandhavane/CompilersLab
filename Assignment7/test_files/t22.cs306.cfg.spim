
	.text 			# The .text assembler directive indicates
	.globl fun		# The following is the code (as oppose to data)
fun:				# .globl makes main know to the 
				# outside of the program.
# Prologue begins 
	sw $ra, 0($sp)		# Save the return address
	sw $fp, -4($sp)		# Save the frame pointer
	sub $fp, $sp, 8		# Update the frame pointer

	sub $sp, $sp, 24		# Make space for the locals
# Prologue ends


label2:    	
	li.d $f2, 2.30
	s.d $f2, -8($fp)
	li.d $f2, 3.20
	s.d $f2, 0($fp)
	l.d $f2, 0($fp)
	li.d $f4, 2.10
	div.d $f6, $f2, $f4
	l.d $f2, -8($fp)
	sub.d $f4, $f6, $f2
	s.d $f4, -8($fp)
	l.d $f2, -8($fp)
	mfc1 $v0, $f2
	move $v1, $v0
	j epilogue_fun

# Epilogue Begins
epilogue_fun:
	add $sp, $sp, 24
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

	sub $sp, $sp, 20		# Make space for the locals
# Prologue ends


label2:    	
	jal fun
	move $v0, $v1
	sw $v0, -4($fp)
	lw $v0, -4($fp)
	li $t0, 3
	seq $t1, $v0, $t0
	bne $t1, $zero, label3 
	j label4

label3:    	
	lw $v0, -4($fp)
	li $t0, -2
	add $t1, $v0, $t0
	sw $t1, -8($fp)
	j label5

label4:    	
	li $v0, 1
	sw $v0, -8($fp)

label5:    	
	lw $v0, -8($fp)
	sw $v0, 0($fp)
	j epilogue_main

# Epilogue Begins
epilogue_main:
	add $sp, $sp, 20
	lw $fp, -4($sp)  
	lw $ra, 0($sp)   
	jr        $31		# Jump back to the called procedure
# Epilogue Ends

