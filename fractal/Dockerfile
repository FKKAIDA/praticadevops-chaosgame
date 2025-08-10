FROM alpine:3.22

WORKDIR /app

RUN apk update
RUN apk upgrade 
RUN apk add g++ asio-dev

COPY source ./source
COPY include ./include

RUN g++ -std=c++20 -Iinclude ./source/*.cpp -o main 

CMD ["./main"]

EXPOSE ${PORT}

