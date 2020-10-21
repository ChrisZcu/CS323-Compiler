CC=gcc
FLEX=flex
BISON=bison


.lex: calc.l
	$(FLEX) calc.l
.cal: cal.y
	$(BISON) -t -d cal.y
calc: .lex .cal
	$(CC) cal.tab.c -lfl -ly -D CALC_MAIN -o calc.out
clean:
	@rm -f lex.yy.c cal.tab.* *.out *.so