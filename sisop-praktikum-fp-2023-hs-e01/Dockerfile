FROM ubuntu:latest
RUN apt-get update && apt-get install -y \
    build-essential \
    wget unzip \
    wget zip \
    sudo

RUN useradd -m myuser && echo "myuser:password" | chpasswd && usermod -aG sudo myuser

USER myuser

WORKDIR /myuser

COPY . /myuser

CMD ["/bin/bash"]