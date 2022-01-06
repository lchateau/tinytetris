#include "OPiece.h"

OPiece::OPiece()
{
    availableStates = new int[states]{348480, 348480, 348480, 348480};
    initState();
}

OPiece::~OPiece()
{
    if (availableStates)
    {
        delete[] availableStates;
    }
}
