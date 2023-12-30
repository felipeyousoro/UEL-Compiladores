#pragma once

#include <vector>
#include <string>
#include <iostream>

typedef struct transition_s {
    int state;
    char symbol;

} transition_t;

class State {
    private :
        int id;
        bool __isFinal;
        bool __isToken;
        std::string token;

        std::vector<transition_t> transitions;

    public :
        State();
        State(int id);
        State(int id, bool isFinal);
        State(int id, bool isFinal, std::string token);
        
        void addTransition(char symbol, int state_id);
        void addTransition(char symbol_from, char symbol_to, int state_id);

        int getId();
        int getTransition(char symbol);

        std::string getToken();

        bool isFinal();
        bool isToken();

        bool operator==(const State& other) const;
        bool operator!=(const State& other) const;

};