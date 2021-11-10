all:
	gcc -std=c99 -Wall -Wextra -Werror setcal.c -o setcal

test:
	./setcal sets.txt
	@echo ""
	@echo "expected result: "
	@echo "U a b c x y z"
	@echo "S a b c x"
	@echo "S x y z"
	@echo "S x"
	@echo "S a b c"
	./setcal rel.txt
	@echo ""
	@echo "expected result: "
	@echo "U dad mom girl boy man woman"
	@echo "R (dad boy) (dad girl) (mom boy) (mom girl)"
	@echo "R (dad man) (boy man) (mom woman) (girl woman)"
	@echo "S boy girl"
	@echo "true"

clear:
	rm setcal