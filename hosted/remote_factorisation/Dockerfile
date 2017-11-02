FROM disconnect3d/nsjail:6d29c19

RUN apt-get update && apt-get install -y \
    g++ coreutils

RUN mkdir /task
RUN mkdir /task/actual

WORKDIR /task
ADD src/remote_factor.cpp /task/.
ADD src/flag.data /task/actual/.
RUN g++ -std=c++14 -O3 -Wno-unused-result -Wno-narrowing -fstack-protector-all -finline-functions -Wstack-protector -Wall -Wextra -Werror -pedantic remote_factor.cpp -o actual/remote_factor && rm remote_factor.cpp
