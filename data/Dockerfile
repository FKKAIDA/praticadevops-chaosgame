FROM alpine:3.22

WORKDIR /app

RUN apk add tzdata
RUN ln -s /usr/share/zoneinfo/America/Sao_Paulo /etc/localtime

RUN apk update
RUN apk upgrade 
RUN apk add g++ asio-dev

COPY source ./source
COPY include ./include

RUN g++ -std=c++20 -Iinclude ./source/*.cpp -o main 

CMD ["./main"]

EXPOSE ${PORT}

