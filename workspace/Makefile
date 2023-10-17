check-install:
	javac --version

# First export SDK Library
# export PATH=/Users/justin.dekeyser/Documents/perso/jaguar/jdk-21.0.1.jdk/Contents/Home/bin:${PATH}

run:
	cd dist && rm -rf *
	javac \
		--module jaguar \
		--module-source-path src \
		-d dist
	java \
		--module-path dist \
		--module jaguar/jdevelopment.Jaguar
