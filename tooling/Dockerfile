FROM ubuntu

WORKDIR /usr/app

RUN apt-get update -y
RUN apt-get install build-essential -y
RUN apt-get install python3.11-dev -y
RUN apt-get install curl -y
RUN curl -sS https://bootstrap.pypa.io/get-pip.py | python3.11

COPY requirements.txt .
RUN pip install -r requirements.txt