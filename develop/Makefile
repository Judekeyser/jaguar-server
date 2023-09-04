devenv:
	docker run --rm -v${PWD}:/usr/app -p 8000:8000 -it jaguar-develop /bin/bash

lrun:
	cd jaguarserver && ./a.out

lbuild:
	cd jaguarserver && gcc -Wall -Wextra -Werror -DMAP_DEBUG jaguar.c map.c -lm && ./a.out
