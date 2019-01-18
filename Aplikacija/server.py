#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import socket

s = socket.socket()  # create a socket object
host = socket.gethostname()  # get local machine name
print(host)
port = 12345  # reserve a port for your service.
s.bind((host, port))  # bind to the port
s.listen(5)  # wait for client connection.

while True:
    c, addr = s.accept()  # establish connection with client.
    print("Got connection from", addr)
    print("Receiving...")
    imageName = c.recv(1024)
    print(type(imageName))
    imageName = imageName.decode("utf-8")
    print(imageName)
    f = open(str(imageName), "rb")
    imageChunk = f.read(1024)
    while(imageChunk):
        print("Sending...")
        c.send(imageChunk)
        imageChunk = f.read(1024)
    f.close()
    print("Done sending image")
    c.close()  # close the connection
