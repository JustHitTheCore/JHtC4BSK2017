FROM disconnect3d/nsjail:6d29c19

RUN apt-get update && apt-get install -y \
    g++

RUN mkdir /task
WORKDIR /task
ADD src/. /task
RUN g++ -std=c++14 -O3 -Wno-unused-result -Wno-type-limits -fstack-protector-all -finline-functions -Wstack-protector -Wall -Wextra -pedantic -Werror lottery.cpp -o lottery && rm lottery.cpp
