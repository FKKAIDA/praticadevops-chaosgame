#!bin/bash

if [ "$1" == "setup" ]; then
    minikube start

    cd frontend
    docker build . -t front:latest
    minikube image load front:latest
    cd ..

    cd fractal
    docker build . -t fractal:latest
    minikube image load fractal:latest
    cd ..

    cd data
    docker build . -t data:latest
    minikube image load data:latest
    cd ..

    minikube image load registry.k8s.io/ingress-nginx/controller:v1.12.2
    minikube addons enable ingress
    minikube addons enable dashboard
    minikube addons enable metrics-server

elif [ "$1" == "start" ]; then
    gnome-terminal -- minikube mount ./data/examples:/examples
    minikube dashboard &
    helm install chaosgame-language charts
elif [ "$1" == "stop" ]; then
    helm uninstall chaosgame-language --wait
elif [ "$1" == "remove" ]; then
    docker image rm front:latest
    docker image rm fractal:latest
    docker image rm data:latest
    minikube image rm fractal:latest
    minikube image rm data:latest
    minikube image rm front:latest
    minikube stop
fi