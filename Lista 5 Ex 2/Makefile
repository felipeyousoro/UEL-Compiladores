all: Automaton.o State.o 
	g++ l5e2.cpp Automaton.o State.o -o l5e2

Automaton.o : Automaton.cpp Automaton.hpp 
	g++ -c Automaton.cpp

State.o : State.cpp State.hpp
	g++ -c State.cpp

clean:
	rm -rf l5e2
    