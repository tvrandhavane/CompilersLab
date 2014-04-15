
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
	li $v0, 3
	sw $v0, -8($fp)
	li.d $f2, 2.30
	s.d $f2, 0($fp)
	lw $v0, -8($fp)
	li $t0, 0
	seq $t1, $v0, $t0
	bne $t1, $zero, label3 
	j label4

label3:    	
	l.d $f2, 0($fp)
	mfc1 $v0, $f2
	sw $v0, -8($fp)
	j label7

label4:    	
	l.d $f2, 0($fp)
	li.d $f4, 0.00
	sgt $v0, $f2, $f4
	li $t0, 0
	sne $t1, $v0, $t0
	bne $t1, $zero, label5 
	j label6

label5:    	
	lw $v0, -8($fp)
	mtc1 $f2, $v0
	s.d $f2, 0($fp)
	j label7

label6:    	
	lw $v0, -8($fp)
	mtc1 $f2, $v0
	l.d $f4, 0($fp)
	add.d $f6, $f2, $f4
	mfc1 $v0, $f6
	sw $v0, -8($fp)
	l.d $f2, 0($fp)
	li.d $f4, 1.00
	add.d $f6, $f2, $f4
	s.d $f6, 0($fp)

label7:    	
	j epilogue_main

# Epilogue Begins
epilogue_main:
	add $sp, $sp, 20
	lw $fp, -4($sp)  
	lw $ra, 0($sp)   
	jr        $31		# Jump back to the called procedure
# Epilogue Ends

