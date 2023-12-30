#include <iostream>
#include <vector>
#include "Automaton.hpp"
#include "State.hpp"

using namespace std;

int main(){

    // CONFIGURANDO AUTOMATO
    State state_1 = State(1);
    state_1.addTransition('h', 2);
    state_1.addTransition('s', 4);
    state_1.addTransition('c', 11);
    state_1.addTransition('+', 17);
    state_1.addTransition('-', 18);
    state_1.addTransition('0', '9', 19);
    
    //falta other
    State state_2 = State(2);
    state_2.addTransition('e', 3);
    state_2.addTransition('i', 7);

    State state_3 = State(3, true, "ELE");
    state_3.addTransition('r', 9);

    State state_4 = State(4);
    state_4.addTransition('h', 5);

    State state_5 = State(5);
    state_5.addTransition('e', 6);

    State state_6 = State(6, true, "ELA");
 
    State state_7 = State(7);
    state_7.addTransition('s', 8);

    State state_8 = State(8, true, "DELE");

    State state_9 = State(9);
    state_9.addTransition('s', 10);

    State state_10 = State(10, true, "DELA");

    State state_11 = State(11);
    state_11.addTransition('a', 12);

    State state_12 = State(12);
    state_12.addTransition('t', 13);
    state_12.addTransition('r', 14);

    State state_13 = State(13, true, "GATO");
    state_13.addTransition('s', 15);

    State state_14 = State(14, true, "CARRO");
    state_14.addTransition('s', 16);

    State state_15 = State(15, true, "GATOS");

    State state_16 = State(16, true, "CARROS");

    State state_17 = State(17, true, "MAIS");

    State state_18 = State(18, true, "MENOS");

    State state_19 = State(19, true, "INTEIRO");
    state_19.addTransition('0', '9', 19);
    state_19.addTransition('e', 20);

    State state_20 = State(20);
    state_20.addTransition('+', 21);
    state_20.addTransition('-', 21);
    state_20.addTransition('0', '9', 22);

    State state_21 = State(21);
    state_21.addTransition('0', '9', 22);

    State state_22 = State(22, true, "REAL");
    state_22.addTransition('0', '9', 22);

    vector<State> states = {state_1, state_2, state_3, state_4, state_5, state_6, state_7, state_8, state_9, state_10, state_11, state_12, state_13, state_14, state_15, state_16, state_17, state_18, state_19, state_20, state_21, state_22};

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
    automaton.analyzePrintToken(input);

    return 0;
}