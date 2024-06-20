<<<<<<< HEAD
FROM socket_service:latest
WORKDIR /home/jaures/pt/Distributed-systems/
COPY . .
RUN  apt-get install -y libcurl4-gnutls-dev curl  libmicrohttpd-dev 

=======
FROM socketclient2:latest
WORKDIR /home/jaures/client/
COPY . .
>>>>>>> 6195504adae3aef66c055e07073ce9720d0a0b0e
EXPOSE 8082


