#pragma once

#include <vector>
#include <iostream>
#include <string>

#include "State.hpp"

class Automaton {
    private:
        State firstState;
        std::vector<State> states;
        std::vector<char> alphabet;
        

    public:
        Automaton(State state, std::vector<State> states);

        State getNextState(State state, char symbol);

        void addAlphabet(char symbol);
        void addAlphabet(char symbol_from, char symbol_to);

        bool isInAlphabet(char symbol);

        void analyze(std::string input);
        void analyzePrintToken(std::string input);

};