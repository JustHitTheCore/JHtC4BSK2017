FROM disconnect3d/nsjail:6d29c19

RUN apt-get update && apt-get install -y \
    g++

RUN mkdir /task
WORKDIR /task
ADD src/. /task
RUN g++ -std=c++14 -O3 -fstack-protector-all -finline-functions -Wstack-protector -Wall -Wextra -pedantic -Werror labyrinth_server.cpp -o labyrinth_server && rm labyrinth_server.cpp
