#include "ZPiece.h"

ZPiece::ZPiece()
{
    availableStates = new int[states]{431424, 598356, 431424, 598356};
    initState();
}

ZPiece::~ZPiece()
{
    if (availableStates)
    {
        delete[] availableStates;
    }
}