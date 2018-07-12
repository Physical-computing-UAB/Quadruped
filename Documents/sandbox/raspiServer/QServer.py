#!/usr/bin/python

import socket
import serial
import sys
import time



class QServer:

	def __init__(self, ip='192.168.4.1', port=8787):
		
		self.IP = ip
		self.PORT = port
		print 'Running on', ip, ':', port
		
		self.client_ip = ""
		self.client_port = 0
		
		
		# Message headers and functions to call when receive it
		self.headers = {
						'p': self.ping		# Ping
						}
				
		
		
		
		
		
		# ----- Create socket ----
		try:
			self.sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
			self.sock.bind((self.IP, self.PORT))
		except Exception as e:
			print "Can't create socket :("
			print e
			exit()
		# -----------------------
			
			
		self.run()


		
		
	def run(self):
		while True:
			data, addr = self.sock.recvfrom(255)

			self.client_ip = addr[0]
			self.client_port = addr[1]
			
			header = data[0]
			body = data[1:]
			
			print header, body
			
			for h in self.headers.keys():	# Call the right methon for the received header
				if header == h:
					self.headers[h](body)

			
			
	
	def ping(self, body):
		self.sock.sendto(':D', (self.client_ip, self.client_port))

	
	
	
	

if __name__ == '__main__':

	if len(sys.argv) != 3:
		print 'QServer.py [ServerIp] [Port]'
		qserver = QServer()
	else:
		ip   = str(sys.argv[1])
		port = int(sys.argv[2])
		qserver = QServer(ip, port)
	
