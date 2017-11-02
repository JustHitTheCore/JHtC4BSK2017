FROM disconnect3d/nsjail:6d29c19

RUN apt-get update && apt-get install -y \
    python3

RUN mkdir /task

# Python requires /etc/passwd mounted and uid/gid existence so we create em...
# See https://bugs.python.org/issue10496
RUN groupadd -g 99999 jailed
RUN useradd --uid 99999 --gid 99999 jailed

WORKDIR /task
ADD src/pow_test.py /task/pow_test.py
ADD src/flag.txt /task/flag.txt

