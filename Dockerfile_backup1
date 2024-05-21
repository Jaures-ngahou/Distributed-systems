FROM socket_service:latest
WORKDIR /home/jaures/client/
COPY . .
RUN  apt-get install libcurl4-gnutls-dev vsftpd -y

EXPOSE 8082
