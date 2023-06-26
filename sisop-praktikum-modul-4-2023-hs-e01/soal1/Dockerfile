FROM ubuntu:latest

# Install system dependencies
RUN apt-get update && apt-get install -y \
    python3 \
    python3-pip \
    build-essential \
    wget unzip

# Set the working directory
WORKDIR /root

# Install required dependencies
RUN pip install kaggle


# Set up Kaggle API credentials
RUN mkdir /root/.kaggle
COPY kaggle.json /root/.kaggle/kaggle.json
RUN mkdir /root/.kg
# RUN mkdir /root/.local/bin
RUN touch /root/.kg/kaggle

RUN echo '#!/usr/bin/python3' >> /root/.kg/kaggle && \
    echo '# -*- coding: utf-8 -*-' >> /root/.kg/kaggle && \
    echo 'import re' >> /root/.kg/kaggle && \
    echo 'import sys' >> /root/.kg/kaggle && \
    echo 'from kaggle.cli import main' >> /root/.kg/kaggle && \
    echo 'if __name__ == "__main__":' >> /root/.kg/kaggle && \
    echo '    sys.argv[0] = re.sub(r"(-script\.pyw|\.exe)?$", "", sys.argv[0])' >> /root/.kg/kaggle && \
    echo '    sys.exit(main())' >> /root/.kg/kaggle

RUN echo 'alias kaggle="~/.kg/kaggle"' >> /root/.bashrc
# RUN ln -s ~/.local/bin/kaggle /usr/bin/kaggle

# Copy the C file
COPY storage.c /root/storage.c
# Download the Kaggle dataset and unzip and show data
RUN gcc storage.c -o storage 

# Set the command to be executed when the Docker container starts
CMD ["./storage"]
