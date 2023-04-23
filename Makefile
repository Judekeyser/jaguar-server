doc:
	cd documentation && make html

devenv:
	docker run --rm -v${PWD}:/usr/app -it jaguar /bin/bash

lbuild:
	cd src/jaguarserver && g++ -Wall -Wextra -Werror jaguar.cpp && ./a.out