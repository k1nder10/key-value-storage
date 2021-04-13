#!/usr/bin/env python3

import socket

def main():
  HOST = '127.0.0.1'
  PORT = 27901

  for i in range(0, 1):
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock.connect((HOST, PORT))

    sock.sendall(b'set <key1> <value1>')

    data = sock.recv(1024)
    if not data:
      print('Read error')
    else:
      print(f'Message from server: {data}')
    
    sock.close()

if __name__ == '__main__':
  main()