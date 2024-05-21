FROM socketclient2:latest
WORKDIR /home/jaures/client/
COPY . .
EXPOSE 8082
