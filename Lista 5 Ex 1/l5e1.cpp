#include <iostream>
#include <vector>
#include "Automaton.hpp"
#include "State.hpp"

using namespace std;

int main(){

    // CONFIGURANDO AUTOMATO
    State state_1 = State(1);
    state_1.addTransition('a', 'z', 2);
    state_1.addTransition('0', '9', 5);
    state_1.addTransition('+', 3);
    state_1.addTransition('-', 4);

    State state_2 = State(2, true);
    state_2.addTransition('a', 'z', 2);
    state_2.addTransition('0', '9', 2);

    State state_3 = State(3, true);
    State state_4 = State(4, true);

    State state_5 = State(5, true);
    state_5.addTransition('e', 6);
    state_5.addTransition('0', '9', 5);

    State state_6 = State(6);
    state_6.addTransition('0', '9', 8);
    state_6.addTransition('+', 7);
    state_6.addTransition('-', 7);

    State state_7 = State(7);
    state_7.addTransition('0', '9', 8);

    State state_8 = State(8, true);
    state_8.addTransition('0', '9', 8);

    vector<State> states = {state_1, state_2, state_3, state_4, state_5, state_6, state_7, state_8};

    Automaton automaton = Automaton(state_1, states);

    automaton.addAlphabet('a', 'z');
    automaton.addAlphabet('0', '9');
    automaton.addAlphabet('+');
    automaton.addAlphabet('-');

    // LENDO ENTRADA
    string input;
    char symbol;
    while(cin.get(symbol)){
        input += symbol;
    }

    input += '\0';
    // ANALISANDO ENTRADA E RETORNANDO 
    automaton.analyze(input);

    return 0;
}