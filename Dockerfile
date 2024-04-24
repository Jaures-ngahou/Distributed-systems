FROM socket_service:latest
WORKDIR /home/jaures/client/
COPY . .
EXPOSE 8082
