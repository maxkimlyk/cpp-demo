#include "demo-common.h"

#include <iostream>

struct Struct
{
    int field1;
    int field2;
    int field3;
};

void PrintCertainField(const Struct& s, int Struct::*field)
{
    std::cout << s.*field << std::endl;
}

DEMO(pointer_to_field_of_struct)
{
    Struct s{1, 2, 3};
    PrintCertainField(s, &Struct::field1);
    PrintCertainField(s, &Struct::field2);
    PrintCertainField(s, &Struct::field3);
}

RUN_DEMOS
