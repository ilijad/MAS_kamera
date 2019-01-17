#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import socket
import io
from PIL import Image
from subprocess import call
import time

s = socket.socket()  # create a socket object
host = socket.gethostname()  # get local machine name
port = 12345  # reserve a port for your service.

s.connect((host, port))
s.send("test.jpg".encode())
print("Sending...")
f = open("enk", "wb")  # stream instead of file
imageChunk = s.recv(1024)
while(imageChunk):
    print("Receiving...")
    f.write(imageChunk)
    imageChunk = s.recv(1024)
print("Done receiving image")
s.shutdown(socket.SHUT_WR)
s.close()  # close the socket when done
f.close()
call(["./dekripcija.exe"])
img = Image.open("dekriptirano.jpg")
img.show()

