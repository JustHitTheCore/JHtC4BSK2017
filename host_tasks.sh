#!/bin/bash

echo "Trying to host all hosted tasks"
echo "NOTE: This requires docker daemon to be up"

HOSTED=hosted
for dir in $(ls $HOSTED); do
    cd $HOSTED/$dir
    docker-compose up -d
    cd ../..
done
