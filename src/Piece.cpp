#include "Piece.h"

Piece::Piece()
{
}

Piece::~Piece()
{
}

void Piece::rotate()
{
    ++currentStateId %= states;
    currentState = availableStates[currentStateId];
}

int Piece::getCurrentState()
{
    return currentState;
}

int Piece::getCurrentStateId()
{
    return currentStateId;
}

void Piece::initState()
{
    currentStateId = 0;
    currentState = availableStates[currentStateId];
}
