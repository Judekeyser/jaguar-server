devenv:
	docker run --rm -v${PWD}:/usr/app -it jaguar-develop /bin/bash

lbuild:
	cd jaguarserver && g++ -Wall -Wextra -Werror jaguar.cpp worker.cpp && ./a.out