loop_invariant: with_loop_invariant.c no_loop_invariant.c
	@gcc -o no_loop_invariant no_loop_invariant.c
	@gcc -o with_loop_invariant with_loop_invariant.c
	@echo "*** Executando o programa sem loop invariant ***"
	@echo
	./no_loop_invariant
	@echo
	@echo
	@echo "*** Executando o programa com loop invariant ***"
	@echo
	./with_loop_invariant
	@echo