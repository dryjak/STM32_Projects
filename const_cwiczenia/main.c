#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

const typedef struct{
    uint8_t Var8;
    uint16_t Var16;
}MyStruct_t;

const MyStruct_t Struct = {5,10};

const uint8_t Var1 = 3;
const uint8_t Tab[16];

int main()
{

    printf("Var1 = %d\n", Var1);

    printf("Tab[0] = %d\n", Tab[0]);

    printf("Struct.Var8 = %d\n", Struct.Var8);



    //Var1 = 2; //Nie możemy zmieniać stałej const
    return 0;
}
