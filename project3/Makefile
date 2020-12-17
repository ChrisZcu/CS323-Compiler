# FLEX = flex
# CC=g++
# BISON=bison
# splc:
# 	@make clean
# 	@mkdir bin
# 	touch bin/splc
# 	@chmod +x bin/splc
	
# 	${BISON} -d syntax.y
# 	${FLEX} lex.l
# 	${CC} syntax.tab.c -lfl -ly -o bin/splc 

# clean:
# 	@rm -rf bin/
# .PHONY: splc

PP=g++
FLEX=flex
BISON=bison


.lex: lex.l
	$(FLEX) lex.l
.syntax: syntax.y
	$(BISON) -t -d syntax.y
splc: .lex .syntax
	@make clean
	@mkdir bin
	$(PP) syntax.tab.c  -lfl -ly -o bin/splc
	@chmod +x bin/splc
clean:
	@rm -rf bin/ 
.PHONY: splc
