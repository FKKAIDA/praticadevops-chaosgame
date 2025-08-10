FROM node:16.14.0-alpine 

WORKDIR /app

COPY favicon.ico ./public/
COPY package.json .
COPY frontend.js .
COPY index.html ./public/

EXPOSE ${PORT}

RUN npm install
CMD ["npm", "start"]
