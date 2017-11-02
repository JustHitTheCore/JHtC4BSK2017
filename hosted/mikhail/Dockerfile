FROM disconnect3d/nsjail:6d29c19

#RUN apt-get update && apt-get install -y \
#    g++

RUN mkdir /task

WORKDIR /task

ADD src/ /task/

#RUN g++ -std=c++14 -g3 -fomit-frame-pointer -fno-stack-protector -fPIE -Wl,-z,execstack -Wno-unused-variable -Wall -Wextra -pedantic /task/exec.cpp -o /task/exec && rm exec.cpp
