#include <stdio.h>
#include "declarations.h"
#include <string.h>
#include <stdint.h>


/*
1. Create a unit_delay() function 
that just counts from 0 to 0xF FF FF FF. 
Treat this as a “unit of time”. 
(the function does not print anything, and does not return anything).
*/
void unit_delay(){
    int startValue = 0;
    int endValue = 0xFFFFFFF;
    for(int i=startValue; i<=endValue; i++);
}

/*
2. Create another function  delay() 
that delays for n units of time, where n is a parameter.
*/
void delay(int n){
    for(int i=0; i<=n;i++);
}

/*
3. Write a function print_bits() that prints 
out the bits of an integer,
preferably grouped in 4s. 
It also prints out the equivalent number in hex.
*/
void print_bits(int number){
    // Block for HEX
    {
        printf("The Hex equivalent of %d = %X\n", number, number);
    }
    
    // Block BINARY
    {
        int arrayBits[32] = {0}; // the size is 32 with 32 zero's by default
        int theBitPosition = 31; // for the last   
        
        // store the bits in  arrayBits
        for(; number > 0; ){
            arrayBits[theBitPosition] = number % 2;
            theBitPosition -= 1;
            number = number / 2;
        }
        
        // printing the bits
        theBitPosition = 1;
        for(; theBitPosition <= 32; ){
            printf("%d",arrayBits[theBitPosition-1]);
            if(theBitPosition %4 == 0){printf(" ");} //separate into groups of 4
            theBitPosition++;
        }
    }
}

/*
 Write a function toggle() that can 
 toggle a particular 
 bit in an unsigned 32 bit integer 
 passed to it. The function accepts two parameters: 
 the unsigned integer 
 as well as the bit position to toggle. 
 There is no return type, but the bit must be changed. 
 This function does not print anything.
*/

void toggle(unsigned int *theInteger, int theBitPosition[2] ){
    // char theBitPosition[2] is = {param1, param2 }
    // if param1 is = 0; it does not toggle all. It check the param2 to get the toggle position
    // if param1 is = 1; it toggles all the bit regardless of param2
    
    //  
    switch(theBitPosition[0]){
        case(0):{
            // check for the position to toggle and toggle
            *theInteger = *theInteger ^ (1UL << theBitPosition[1]);
            break;
        }
        case(1):{
            *theInteger = ~ (*theInteger);
            break;
        }
    }

    
}

/*
 Write a function print_char that accepts a character and prints out 
 the character and its corresponding ASCII value separated by a tab. 
 (print the char as an int).

*/
void print_char(char theCharacter){
    printf("%c\t%d\n",theCharacter, theCharacter);
    
}

/*
Q6. Write a function print_string() that accepts a “string” as
a parameter and prints each character and its corresponding ASCII character.
Accomplish this by calling the print_char(). 
Hint: use a property of strings to know when to stop printing .
*/
void print_string(char theString[]){
    for(int i=0; i<strlen(theString);i++){
        print_char(theString[i]);
    }
    
}




/*
Q7.
Write a function set_mux() 
to set bits 7,8,9  in a 16bit register to 1, 0, 1 respectively.
*/
void set_mux(uint16_t *bit16Register){
 
    
    #define MASK(X) (1UL << X)
    // set 
    *bit16Register = *bit16Register | MASK(7) | MASK(9) ;
    *bit16Register = *bit16Register & ~(MASK(8));
}


/*
 Write a function that takes no arguments store_long_no() that does the following:
Declare a uint32_t integer;
Declare another variable tmp as uint8_t.
The goal is to store the number 0xF7 31 EC 81 1 into 
the uint32_t 8-bits at a time.
Assign 0xF7 to tmp. Store this number in the most significant byte (byte 3) 
of the unsigned uint32_t, next, assign  0x31 to tmp, and store this 
in byte position 2. Etc. Do this repeatedly for all 4 bytes. 
Afterward, print out the stored number in hex format. Have you been successful?


*/


void store_long_no(){
    uint32_t anInteger;
    uint8_t tmp;
    uint32_t intValue  = 0xFFFFFFFF;
    tmp = 0xF7;
    intValue = intValue << 8 | tmp;
    tmp =  0x31;
  intValue = intValue << 8 | tmp;
    tmp = 0xEC;
  intValue = intValue << 8 | tmp;
  tmp = 0x81;
  intValue = intValue << 8 | tmp;


    printf("%X",intValue);
}





