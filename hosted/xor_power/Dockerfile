FROM disconnect3d/nsjail:6d29c19

RUN apt-get update && apt-get install -y \
    gcc g++

RUN mkdir /task
WORKDIR /task
ADD src/. /task
RUN g++ -std=c++14 -g3 -fstack-protector-all -finline-limit=1 -fno-inline -Wstack-protector -Wall -Wextra -pedantic -Werror task_code.cpp -o xor_power && rm task_code.cpp
