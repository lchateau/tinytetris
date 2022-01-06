#include "JPiece.h"

JPiece::JPiece()
{
    availableStates = new int[states]{599636, 431376, 598336, 432192};
    initState();
}

JPiece::~JPiece()
{
    if (availableStates)
    {
        delete[] availableStates;
    }
}
