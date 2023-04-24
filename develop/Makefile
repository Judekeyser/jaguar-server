devenv:
	docker run --rm -v${PWD}:/usr/app -it jaguar-develop /bin/bash

lbuild:
	cd jaguarserver && gcc -Wall -Wextra -Werror jaguar.c worker.c && ./a.out