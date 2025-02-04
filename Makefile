clack: Again
	@gcc -Wall clack.c -o bin/clack
	@clang -Wall clack.c -o bin/clackclang
Again: Great
	@echo 'Again'
Great: America
	@echo 'Great'
America:
	@echo 'America'
