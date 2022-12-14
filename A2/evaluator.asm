# MIPS Assembly Program for evaluating an expression in postfix format
# From MIHIR MEENA(2019CS10370) and NISHANT KUMAR(2019CS50586)
# -----------------------------------------------------
# Data Declarations
.data
newline:    .asciiz    "\n"
msg_input:  .asciiz    "Input: "
msg_intro:  .asciiz    "MIPS Assembly Program for evaluating an expression in postfix format\n"
msg_dec:    .asciiz     "------------------------------------------------\n"
msg_ans:    .asciiz    "Answer: "
msg_error:  .asciiz    "Wrong Input"
input:      .space     52 
char:       .space     2

# -----------------------------------------------------
# Code Section
.text
.globl main
.ent main
main:
# -----------

li $v0, 4           #
la $a0, msg_intro   # prints a text message(msg_intro) on the console
syscall             #

li $v0, 4           #
la $a0, msg_dec     # prints a text message(msg_dec) on the console
syscall             #

li $v0, 4           #
la $a0, msg_input   # prints a text message(msg_input) on the console
syscall             # 
# -----------

lb $t1, newline         # loads the newline char into $t1
li $t5, 0               # $t5 stores the no. of elements present in the stack
# takes input of single char 
Loop: 
li $v0, 8       #
la $a0, char    # takes a single char input and stores it in char
li $a1, 2       # 
syscall
lb $t0, char            # loads the input char into $t0
j inputValidation
updation:
beq $t0, $t1, printAns # terminates the loop if "/n" is hit indicating the end of input
j Loop

# To check if the input is correct and if correct call appropriate function 
inputValidation:
beq $t0, 42, oprMult        # char is "*"
beq $t0, 43, oprAdd         # char is "+"
beq $t0, 45, oprSub         # char is "-"
beq $t0, 10, updation       # char is "\n"
bgt $t0, 57, inputError    
blt $t0, 48, inputError
addi $t0, $t0, -48          # converts the ascii value to dec value
subu $sp, $sp, 4            
sw $t0, ($sp)               # pushes the digit to the stack
add $t5, $t5, 1             # updates the count of elements in stack
j updation                  # proceed to take the next input char

# if "*" is the input char
oprMult:
blt $t5, 2, inputError  # checks if the stack has enough elements to operate on 
lw $t2, ($sp)           # pop top element of the stack
addu $sp, $sp, 4
lw $t3, ($sp)           # pop next top element of the stack
addu $sp, $sp, 4
mul $t2, $t2, $t3       # product of top two elements of stack stored in $t2
subu $sp, $sp, 4    
sw $t2, ($sp)           # pushes the product of top two elements of stack into the stack
add $t5, $t5, -1        # updates the count of elements in stack
j updation

# if "+" is the input char
oprAdd:
blt $t5, 2, inputError  # checks if the stack has enough elements to operate on 
lw $t2, ($sp)           # pop top element of the stack
addu $sp, $sp, 4
lw $t3, ($sp)           # pop next top element of the stack
addu $sp, $sp, 4
add $t2, $t2, $t3       # sum of top two elements of stack stored in $t2
subu $sp, $sp, 4    
sw $t2, ($sp)           # pushes the sum of top two elements of stack into the stack
add $t5, $t5, -1        # updates the count of elements in stack
j updation

# if "-" is the input char
oprSub:
blt $t5, 2, inputError  # checks if the stack has enough elements to operate on 
lw $t2, ($sp)           # pop top element of the stack
addu $sp, $sp, 4
lw $t3, ($sp)           # pop next top element of the stack
add $sp, $sp, 4
sub $t2, $t3, $t2       # difference of top two elements of stack stored in $t2
subu $sp, $sp, 4    
sw $t2, ($sp)           # pushes the product of top two elements of stack into the stack
add $t5, $t5, -1        # updates the count of elements in stack
j updation

# to throw error on wrong input
inputError:
li $v0, 4
la $a0, newline
syscall
li $v0, 4
la $a0, msg_error
syscall
j terminate

# to print answer 
printAns:
bne $t5, 1, inputError  # checks if the stack has only 1 element in the end
li $v0, 4
la $a0, msg_ans
syscall
li $v0, 1
lw $a0, ($sp)
syscall

# Done, terminate program.
terminate:
li $v0, 4
la $a0, newline
syscall
li $v0, 4
la $a0, msg_dec
syscall

li $v0, 10 # terminate
syscall 
.end main
# -----------------------------------------------------
