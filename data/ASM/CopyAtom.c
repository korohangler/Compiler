
struct Atom
{
    int data;
    int type;
};

struct Atom CopyAtom(struct Atom a)
{
    struct Atom result;

    result.type = a.type;

    if(a.type == 0)
    {
        result.data = a.data;
    }
    else
    {
        result.data = malloc(50);
        strcpy((char*)result.data, (char*)a.data);
    }

    return result;
}