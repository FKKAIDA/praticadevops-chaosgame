# Chaos Game

Aplicação web para gerar fractais utilizando uma linguage

## Requisitos

Docker, Docker Compose, minikube e Helm
Ter a seguinte entrada em /etc/hosts: 192.168.49.2 k8s.local

## Instalação

Para usar com docker compose, execute na raiz do projeto:

```bash
docker compose up
```
E accesse localhost:80

Para interromper os containers, deletar os containers, imagens e volumes:

```bash
docker compose down --rmi all -v
```

Para instalar as imagens no minikube:

```bash
bash ./helm.sh setup
```

Para executar a aplicação:

```bash
bash ./helm.sh start
```
E acesse k8s.local
Para interromper a aplicação:

```bash
bash ./helm.sh stop
```
Para deletar as imagens geradas:

```bash
bash ./helm.sh remove
```
