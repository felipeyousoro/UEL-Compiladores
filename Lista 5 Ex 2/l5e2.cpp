#include <iostream>
#include <vector>
#include "Automaton.hpp"
#include "State.hpp"

using namespace std;

int main(){

    // CONFIGURANDO AUTOMATO
    State state_1 = State(1);
    state_1.addTransition('i', 2);
    state_1.addTransition('a', 'h', 4);
    state_1.addTransition('j', 'z', 4);
    state_1.addTransition('.', 5);
    state_1.addTransition('0', '9', 7);
    state_1.addTransition('-', 9);
    state_1.addTransition(' ', 12);
    //falta other

    State state_2 = State(2, true, "ID");
    state_2.addTransition('f', 3);
    state_2.addTransition('a', 'e', 4);
    state_2.addTransition('g', 'z', 4);
    state_2.addTransition('0', '9', 4);

    State state_3 = State(3, true, "IF");
    state_3.addTransition('a', 'z', 4);
    state_3.addTransition('0', '9', 4);

    State state_4 = State(4, true, "ID");
    state_4.addTransition('a', 'z', 4);
    state_4.addTransition('0', '9', 4);

    State state_5 = State(5, true, "error");
    state_5.addTransition('0', '9', 6);

    State state_6 = State(6, true, "REAL");
    state_6.addTransition('0', '9', 6);
 
    State state_7 = State(7, true, "NUM");
    state_7.addTransition('0', '9', 7);
    state_7.addTransition('.', 8);

    State state_8 = State(8, true, "REAL");
    state_8.addTransition('0', '9', 8);

    State state_9 = State(9, true, "error");
    state_9.addTransition('-', 10);

    State state_10 = State(10);
    state_10.addTransition('a', 'z', 10);
    state_10.addTransition(' ', 10);
    state_10.addTransition('\n', 11);

    State state_11 = State(11, true, "comment");

    State state_12 = State(12, true, "white space");
    state_12.addTransition(' ', 12);

    vector<State> states = {state_1, state_2, state_3, state_4, state_5, state_6, state_7, state_8, state_9, state_10, state_11, state_12};

    Automaton automaton = Automaton(state_1, states);

    automaton.addAlphabet('a', 'z');
    automaton.addAlphabet('0', '9');
    automaton.addAlphabet('+');
    automaton.addAlphabet('-');
    automaton.addAlphabet('.');
    automaton.addAlphabet(' ');
    automaton.addAlphabet('\n');

    // LENDO ENTRADA
    string input;
    char symbol;
    while(cin.get(symbol)){
        input += symbol;
    }

    input += '\0';
    // ANALISANDO ENTRADA E RETORNANDO 
    automaton.analyzePrintToken(input);

    return 0;
}