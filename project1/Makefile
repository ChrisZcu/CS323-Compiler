FLEX = flex
CC=gcc
BISON=bison
splc:
	@make clean
	@mkdir bin
	touch bin/splc
	@chmod +x bin/splc
	
	${BISON} -d syntax.y
	${FLEX} lex.l
	${CC} syntax.tab.c -lfl -ly -o bin/splc 

clean:
	@rm -rf bin/
.PHONY: splc
