#=================================================================
# Copyright 2024 Georgia Tech.  All rights reserved.
# The materials provided by the instructor in this course are for
# the use of the students currently enrolled in the course.
# Copyrighted course materials may not be further disseminated.
# This file must not be made publicly available anywhere.
# =================================================================

# HW2-2
# Student Name: Joshua Maxi
# Date: 2/20/2024
#
#     T o p  o f  P i l e
#
#
# This routine finds the color of the part on top of a pile.
#
#===========================================================================
# CHANGE LOG: brief description of changes made from P1-2-shell.asm
# to this version of code.
# Date  Modification
# 02/20: Set variables to specific registers and started creating the Array and Loop process to mimic my 1-1 C implementation.
# 02/20: Made code to skip the black spaces in the pile.
# 02/22: Created code to locate T's in the pile and go down
# 02/26: Modified the code to go right then left and then down on one specific color
# 03/01: Made the code start going diagonal from the beginning
# 03/02: Made the code go left and right fully before going down to the next section of the color
#===========================================================================

.data
Pile:  .alloc	1024

.text

TopOfPile:		addi	$1, $0, Pile		# point to array base
			swi	545			# generate pile
		
			addi  $7, $0, 7			# Limit for the left to right counter
			addi  $6, $0, 0			# Initializing counter for how many times changed left to right
			addi  $5, $0, 1			# Initialize a way to stop the horizontal movements
			addi  $4, $1, 12           	# Initialize register to point to a value
			addi  $13, $0, 0		# Initlize loop counter
			addi  $14, $0, 10		# Set loop counter limit to 10

Next:			lbu   $8, 0($4)			# Check the value at register 4
			beq   $8, $0, Skip		# If the vlaue at register 4 equals 0 then go to skip
			lbu   $9, 1($4)			# Get the value of the current Array Base + 4
			lbu   $10, 64($4)		# Check the value below register 4
			beq   $8, $9, Hori		# Check if the next value to the right of the current array base is black
			beq   $8, $10, Vert2		# If the value below register 4 equals itself then go to vert
			beq   $9, $0, Horiz		# If the value to the right of register 4 is 0 then go to horiz
		
Skip:			addi  $4, $4, 65		# Current Array Base + 65
			j Next				# Jump to the loop branch

Hori:			addi $4, $4, 1 			# Move the current location 1 to the right
			addi $28, $4, 1			# Place holder for location to the right of reigster 4
			lbu  $8, 0($4)			# Load vlaue at the current location
			lbu  $9, 0($28)			# Load value to the right
			lbu  $11, 2($4)			# Load value to the right
			slt  $12, $6, $7		# Set register 12 if a certain amount of loops have occurred
			beq  $12, $0, Down2		# If there is a T then go down
Back2:			beq  $8, $9, Hori		# Compares if the value to the left is the same then loop again 
			slti $5, $8, 1			# If the value of register 8 equals 1 then set register 5
			addi $6, $6, 1			# Counter for how many times changed left to right
			addi $13, $13, 1		# Increase loop counter by 1
			beq  $9, $0, Horiz		# If the value of register 9 equals 0 then go to horiz
			beq  $11, $0, Horiz		# If the value of register 11 equals 0 then go to horiz
			bne  $8, $9, Change		# If the value of register 9 equals register 8 then go to change and change direction

Vert:			addi  $7, $7, -1		# Lower Second loop counter by 1
Vert2:			addi  $13, $0, 0		# Reset loop counter
			addi $6, $0, 0			# Reset second loop counter
			addi $4, $4, 64			# Move the current location 1 below
			addi $28, $4, 64		# Place holder for location below reigster 4
			lbu  $8, 0($4)			# Load vlaue at the current location
			lbu  $9, 0($28)			# Load value 1 below
			lbu  $10, 1($4)			# Load value to the right
			lbu  $11, 128($4)		# Load value 2 below
			beq  $8, $10, Hori		# If the value of register 8 equals register 10 then go to hori
			beq  $8, $9, Vert2		# If the value of register 9 equals register 8 then loop again
			beq  $9, $0, End		# If the value of register 9 equals 0 then go to end
			beq  $11, $0, End		# If the value of register 11 0 then go to end
			bne  $8, $9, Change		# If the value of register 8 equals register 9 then go to change and change direction

Horiz:			addi $4, $4, -1			# Move the current location 1 to the left
			addi $28, $4, -1		# Place holder for location to the left of reigster 4
			lbu  $8, 0($4)			# Load value at the current location
			lbu  $9, 0($28)			# Load value to the left
			lbu  $11, -2($4)		# Load value 2 to the left
			slt  $12, $6, $7		# Set register 12 if a certain amount of loops have occurred
			beq  $12, $0, Down		# If there is a T then go down
Back1:			beq  $8, $9, Horiz		# Compares if the value to the left is the same then loop again 
			addi $6, $6, 1			# Counter for how many times changed left to right
			addi $13, $13, 1		# Add 1 to loop counter
			beq  $13, $14, End		# If the loop counter = 10 then end
			beq  $9, $0, Hori		# If the value to the left of register 4 is 0 then go to Hori
			beq  $8, $11, Change		# If the value to the left of register 4 is not equal then go to change and change direction
			j Hori

Change:			addi $4, $28, 0			# Change the two values
			j Next				# Jump to next

Down:			lbu  $10, 64($4)		# Load value below
			beq  $8, $10, Vert		# If there is a T then go down
			j Back1

Down2:			lbu  $10, 64($4)		# Load value below
			beq  $8, $10, Vert		# If there is a T then go down
			j Back2
	
End:			lbu  $2, 0($4)			# Input the final answer
			swi	546			# submit answer and check
			jr	$31			# return to caller