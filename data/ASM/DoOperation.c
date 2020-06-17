#include <string.h>
#include <stdlib.h>

struct Atom
{
    int data;
    int type;
};

enum operationTypes
{
    summ = 0,
    sub,
    divide,
    mul,
    less,
    more,
    equal,

};

struct Atom DoOperation(struct Atom a, struct Atom b, int operationType)
{
    struct Atom result;
    result.type = a.type;

    if(a.type == 0)
    {
        float right = *((float*)(&b.data));

        if(b.type != 0)
            right = atof((char*)b.data);

        switch(operationType)
        {
        case 0:
            *((float*)&result.data) = *((float*)&a.data) + right;
            break;
        case 1:
            *((float*)&result.data) = *((float*)&a.data) - right;
            break;
        case 2:
            *((float*)&result.data) = *((float*)&a.data) / right;
            break;
        case 3:
            *((float*)&result.data) = *((float*)&a.data) * right;
            break;
        case 4:
            *((float*)&result.data) = *((float*)&a.data) < right;
            break;
        case 5:
            *((float*)&result.data) = *((float*)&a.data) > right;
            break;
        case 6:
            *((float*)&result.data) = *((float*)&a.data) == right;
            break;
        }
    }
    else
    {
        char* right = (char*)b.data;

        if(b.type == 0)
        {
            right = (char*)malloc(50);

            gcvt(*((float*)&b.data), 6, right);
        }
        
        switch(operationType)
        {
        case 0:
            result.data = (char*)malloc(strlen((char*)a.data) + strlen(right) + 1);
            strcpy((char*)result.data, (char*)a.data);
            strcat((char*)result.data, right);
            break;
        case 1:
            result.data = 0;
            break;
        case 2:
            result.data = 0;
            break;
        case 3:
            result.data = 0;
            break;
        case 4:
            result.data = 0;
            break;
        case 5:
            result.data = 0;
            break;
        case 6:
            result.data = strcmp((char*)a.data, right);
            result.type = 0;
            break;
        }
    }

    return result;
}