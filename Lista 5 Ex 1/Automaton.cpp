#include "Automaton.hpp"

using namespace std;

Automaton::Automaton(State state, vector<State> states){
    this->states = states;
    this->firstState = states[0];
}

State Automaton::getNextState(State state, char symbol){
    int nextState = state.getTransition(symbol);

    if(nextState == -1){
        return State(-1);
    }

    for(State s : this->states){
        if(s.getId() == nextState){
            return s;
        }
    }

    return State(-1);
}

void Automaton::addAlphabet(char symbol){
    this->alphabet.push_back(symbol);
}

void Automaton::addAlphabet(char symbol_from, char symbol_to){
    for (char symbol = symbol_from; symbol <= symbol_to; symbol++){
        this->alphabet.push_back(symbol);
    }
}

bool Automaton::isInAlphabet(char symbol){
    for (char s : this->alphabet){
        if (s == symbol){
            return true;
        }
    }

    return false;
}

static int getCurrentFinalIndex(State state, int lastFinalIndex){
    if(state.isFinal()){
        return state.getId();
    }

    return lastFinalIndex;
}



void Automaton::analyze(string input){
    int lastFinalIndex = 0;
    int currentFinalIndex = 0;
    string nextline = "";
    State currentState = this->firstState;

    for(int i = 0; i < input.length(); i++){
        currentState = this->getNextState(currentState, input[i]);

        if(currentState.getId() != -1){
            if(currentState.isFinal()){
                currentFinalIndex = i;
            }
        }
        else{
            string output = input.substr(lastFinalIndex, currentFinalIndex - lastFinalIndex + 1);
            if(this->isInAlphabet(output[0]) == true){
                if(output[output.length() - 1] == '\n'){
                    output = output.substr(0, output.length() - 1);
                }

                cout << nextline << output;
            }
            else{
                if(output[0] != '\n'){
                    cout << nextline << "ERRO";
                }
            }

            lastFinalIndex = currentFinalIndex + 1;
            currentFinalIndex = lastFinalIndex;
            i = lastFinalIndex;
            nextline = "\n";

            currentState = this->firstState;
            currentState = this->getNextState(currentState, input[i]);
            // if(this->isInAlphabet(input[i]) == true){
            //     string output = input.substr(lastFinalIndex, currentFinalIndex - lastFinalIndex + 1);
            //     if(this->isInAlphabet(output[0]) == true){
            //         cout << output << endl;
            //     }
            //     else{
            //         if(output[0] != '\n'){
            //             cout << "ERRO" << endl;
            //         }
            //     }

            //     lastFinalIndex = currentFinalIndex + 1;
            //     currentFinalIndex = lastFinalIndex;
            //     i = lastFinalIndex;
                

            //     currentState = this->firstState;
            //     currentState = this->getNextState(currentState, input[i]);
            // }
            // else{
            //     string output = input.substr(lastFinalIndex, currentFinalIndex - lastFinalIndex + 1);
            //     if(this->isInAlphabet(output[0]) == true){
            //         cout << output << endl;
            //     }
            //     else{
            //         if(output[0] != '\n'){
            //             cout << "ERRO" << endl;
            //         }
            //     }

            //     lastFinalIndex = currentFinalIndex + 1;
            //     currentFinalIndex = lastFinalIndex;
            //     i = lastFinalIndex;

            //     currentState = this->firstState;
            //     currentState = this->getNextState(currentState, input[i]);
            // }
        
        }

    }

    if(this->isInAlphabet(input[lastFinalIndex]) == true){
        cout << input.substr(lastFinalIndex, currentFinalIndex - lastFinalIndex + 1) << endl;
    }




}