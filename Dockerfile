FROM ubuntu:22.04
WORKDIR /home/client/
COPY . .
RUN apt-get update && \
 apt-get install -y iputils-ping net-tools nano 
EXPOSE 8082
