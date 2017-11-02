FROM python:3.6

RUN apt-get update && apt-get install python-dev espeak -y

COPY requirements.txt /tmp/requirements.txt
RUN pip install -r /tmp/requirements.txt && rm /tmp/requirements.txt

# This could be done shorter but idk
RUN groupadd -g 99999 jailed
RUN useradd --uid 99999 --gid 99999 jailed
RUN mkdir /home/jailed
RUN chown jailed /home/jailed -R
RUN chmod 755 /home/jailed -R
USER jailed

WORKDIR /task
# the task content will be mounted as RO so it cannot be deleted with command injection
