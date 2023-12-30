#include "State.hpp"

using namespace std;

State::State(){

}

State::State(int id){
    this->id = id;
    this->__isFinal = false;
    this->__isToken = false;
}

State::State(int id, bool isFinal){
    this->id = id;
    this->__isFinal = isFinal;
}

State::State(int id, bool isFinal, string token){
    this->id = id;
    this->__isFinal = isFinal;
    this->__isToken = true;
    this->token = token;
}

void State::addTransition(char symbol, int state_id){
    transition_t transition;
    transition.state = state_id;
    transition.symbol = symbol;

    this->transitions.push_back(transition);
}

void State::addTransition(char symbol_from, char symbol_to, int state_id){
    for (char symbol = symbol_from; symbol <= symbol_to; symbol++){
        this->addTransition(symbol, state_id);
    }
}

int State::getId(){
    return this->id;
}

int State::getTransition(char symbol){
    for (transition_t transition : this->transitions){
        if (transition.symbol == symbol){
            return transition.state;
        }
    }

    return -1;
}

string State::getToken(){
    return this->token;
}

bool State::isFinal(){
    return this->__isFinal;
}

bool State::isToken(){
    return this->__isToken;
}

bool State::operator==(const State& other) const{
    return this->id == other.id;
}

bool State::operator!=(const State& other) const{
    return this->id != other.id;
}


