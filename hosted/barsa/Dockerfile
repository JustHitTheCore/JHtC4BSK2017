FROM python:2.7

RUN apt-get update && apt-get install python-dev libgmp-dev -y

COPY requirements.txt /tmp/requirements.txt
RUN pip install -r /tmp/requirements.txt && rm /tmp/requirements.txt

COPY src/ /
COPY src/init_db.db /tmp/test.db
