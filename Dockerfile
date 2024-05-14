FROM socket_service:latest
WORKDIR /home/jaures/pt/Distributed-systems/
COPY . .
RUN  apt-get install -y libcurl4-gnutls-dev curl  libmicrohttpd-dev 

EXPOSE 8082
