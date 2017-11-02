FROM disconnect3d/nsjail:6d29c19

RUN apt-get update && apt-get install -y \
    g++

RUN mkdir /task
WORKDIR /task
ADD src/. /task
RUN g++ -std=c++14 -O3 -Wno-unused-result -Wno-narrowing -fstack-protector-all -finline-functions -Wstack-protector -Wall -Wextra -pedantic -Werror crypto_service.cpp -o online_crypto && rm crypto_service.cpp
