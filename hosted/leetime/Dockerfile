FROM ruby:2.3.4
# dont work with ruby >= 2.4
# see https://stackoverflow.com/questions/41504106/ruby-2-4-and-rails-4-stack-level-too-deep-systemstackerror

RUN apt-get update -y && \
    apt-get install -y \
        build-essential \
        libpq-dev \
	nodejs

COPY ./src /app
WORKDIR /app

ADD src/Gemfile /app/Gemfile
ADD src/Gemfile.lock /app/Gemfile.lock
RUN bundle install

ADD src/ /app
