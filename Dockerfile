FROM ubuntu


WORKDIR /usr/app

RUN apt update
RUN apt install build-essential -y
