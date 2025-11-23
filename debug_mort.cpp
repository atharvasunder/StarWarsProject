#include "mbed.h"
/*done only for printf. It ensures printf is mapped to the serial output of the Nucleo board (over USB).
It is not present in the current project folder because the compiler 
pulls it from Mbed’s servers, where the whole Mbed OS library lives

mbed.h probably already includes <stdint.h> internally.
That’s why uint16_t works even if we didn’t explicitly include it
*/
#include "debug_mort.h"

/* i think even the names uint16_t are defined with mbed.h*/
void debugprint(uint16_t number)
{
    printf("count is %u\n",number);    
}

void debugprint_2(uint32_t number){
    printf("value is %u\n",number);  
}

void debugprintHelloWorld( void )
{
    printf("I'm aliiiiiivveeee!!!\n");
} 

void debugprintFullQueue( void )
{
    printf("Event queue is full!\n");
}

void debugprintEmptyQueue( void )
{
    printf("Event queue is empty!\n");
} 

void printgummycolour(uint16_t gummy){
    printf("gummy is %u \n", gummy);
}

void printoneinput( void )
{
    printf("1 input!\n");
}

void printtwoinputs( void )
{
    printf("2 inputs!\n");
}

void printthreeinputs( void )
{
    printf("3 inputs!\n");
}

void printtgetoutputs( void )
{
    printf("get outputs!\n");
}
