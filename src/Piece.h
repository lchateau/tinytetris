#pragma once

class Piece
{
public:
    Piece();
    virtual ~Piece();
    void rotate();
    int getCurrentState();
    int getCurrentStateId();

protected:
    void initState();
    int states = 4;
    int currentState;
    int currentStateId = 0;
    int *availableStates;
};
