
struct Atom
{
    int type;
    int data;
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
        result.data = strcpy((char*)a.data);
    }

    return result;
}