#! /usr/bin python
'''
This is the Sensor Network Server for a Rasberry Pi or Carambola

	Spring 2013
	
	Michael Bartling				
		bart4128@tamu.edu				

Fun Fact:
		Instead of one server for Clients and Modules, we have TWO!
		One server manages the Clients,
		The other  manages the Modules,
		The Transfer Manager prevents the two servers from fighting over resources.
		YAY!
	
Current Status:
	Implemented a Traffic Manager to perform IPC between the Client and Module Handlers
	
	Added Deque Support instead of Queues
	Added Multi-line command support

	Added MORE COMMENTS!

	TODO: implement unique identifier check
	TODO: add functionality for giving a module an alias. e.g. instead of writing "tell 192.168.1.2 something" we can write "tell temp_meter something"

'''
import errno
from socket import *
import thread
import Queue
from collections import deque


BUFF = 1024 
CLIENT_PORT = 9200
MODULE_PORT = 9100
MODULE_SRC_PORT = 9101
CLIENT_HOST = '0.0.0.0'
MODULE_HOST = '0.0.0.0'

'''
	The MULTI_LINE_SEP ("!") lets us cram multiple commands together at the command line. Basically, think of "!" as "and then..."

	For example) 
		tell 192.168.1.2 Initialize! tell 192.168.3 Initialize! tell 192.168.1.4 Do something cool
	 is basitelly saying
		tell 192.168.1.2 Initialize and then tell 192.168.3 Initialize and then tell 192.168.1.4 Do something cool

	This isn't as useful for human command line interaction, however, having a delimiter makes scripting macros much easier!

'''
MULTI_LINE_SEP = "!"


'''
gxQ = General Queue
rxQ = Client Queue
txQ = Module Queue

ClientList holds the names of connected clients
ModuleList holds the names of connected modules
'''
ClientList = []
ModuleList = []

gxQ = Queue.Queue()

#======================================================================================================
# POST CARDS
#======================================================================================================
'''
	A post card holds the command, payload, and relavent addresses.
	This class represents the headers in our interprocess communications.

	cmd: 		A command for the server to do. Ex) "tell", "list", etc
	msgFrom:	Basitelly the return address on our package
	msgTo:		The destination address
	payload:	(optional) Includes any module specific commands and/or data to transfer.

	See parse_incoming_data() for examples of command line syntax.
	
	TODO:
	add support for PS (protocol specific) packet info
'''
class postcard:
	
	def __init__(self, cmd , msgFrom, msgTo, payload):
		self.cmd 	 = cmd
		self.msgFrom = msgFrom
		self.dstAddr = msgTo
		self.payload = payload

#======================================================================================================
# TRAFFIC MANAGER
#======================================================================================================

'''
	The Traffic Manager takes the postcards in gxQ and determines whether they belong in clientQ or moduleQ 

	This separate thread facilitates IPC. 
'''
def TrafficManager(gxQ, clientQ, moduleQ):
	while 1:
		if gxQ.empty() == False:		
			package = gxQ.get()
			if (package.dstAddr not in ClientList) and (package.dstAddr not in ModuleList):
				print "Invalid dstAddr\n"
			if (package.dstAddr in ClientList):
				#TODO: add ability to notify named module/client connection thread to listen for a package
				clientQ.appendleft(package)
			if (package.dstAddr in ModuleList):
				moduleQ.appendleft(package)

#======================================================================================================
# ACCEPT HANDLER
#======================================================================================================
'''
	The acceptHndler thread accepts the initial connections from either Clients or Modules and does checks if necessary. 
		
	Modules:	
		- If the acceptHndler is told to accept Module connections, it will wait for the Modules to say "READY"
		- Next the server closes the connection, initiates a connection thread, and passes the connection address to the connection thread.
	 	- Simultaneously, we add the connection address to the connection list.
		- This way, we don't have to worry about the modules sporadically disconnecting from the server.
	
	Clients:
		- Clients don't need to tell the server that they are ready. If we connected to the server, we are probably ready to do things.
		- Unlike module connections, Client Connections are not closed. Instead, we directly pass the client socket to the client_handler thread.
		- - Our current model assumes only 1-2 client connections at a time.

'''
def acceptHndler((host, port), rxQ, txQ, connList, connType):
	# Do the standard socket creation
	serversock = socket(AF_INET, SOCK_STREAM)
	serversock.setsockopt(SOL_SOCKET, SO_REUSEADDR, 1)
	serversock.bind((host,port))
	serversock.listen(5)
	#listen for connections
	print 'waiting for connection... listening on port', port
	while 1:
		clientsock, addr = serversock.accept()
		print '...connected from:', addr
		# If connected to a Client, Immediately start a client_handler thread
		if connType == "Client":
			print "Welcome Client ", addr[0]
			thread.start_new_thread(client_handler, (clientsock, rxQ, addr[0], connList))
			connList.append(addr[0])

		# If connected to a Module, wait for "READY" then start the module_handler thread
		if connType == "Module":	
			data = clientsock.recv(BUFF)
			tempData = data.strip()			#ignore leading white space
			#if we don't receive any data, then something went wrong!
			if not data:
				print 'o crap\n'
				break
				
			# type 'close' on module console to cancel connection to the server	
			if "close\r\n" == data.rstrip(): break 
			
			#Consider splitting the incoming stream and parse for "READY" instead of current "hard hacking" method
			if "READY" == tempData[:5]: #YAY!
				print "Module at: ", repr(addr), "ready!"
				print "Welcome Module ", addr[0] 
				connList.append(addr[0])
			clientsock.close()
			thread.start_new_thread(module_handler, (MODULE_SRC_PORT, rxQ, addr[0], connList))
	

#======================================================================================================
# CLIENT HANDLER 
#======================================================================================================
'''
The client_handler manages the client connections.

Internal functions:
		module_requested():		Return True if I am the requested Client/Module
		parse_incoming_data():	Takes the incoming data and makes post cards out of it. See parse_incoming_data();

'''
def client_handler(clientsock, rxQ, givenName, connList):
	print "beginning module handler\n"
	breakcnter = 0
	name = givenName

	while 1:
		data = ''
		# if rxQ is not Empty, check if need to do anything
		if rxQ:
			print "rxQ is not Empty for" + repr(name)
			try:
				package = rxQ.pop()
				requested = module_requested(name, package.dstAddr)
			except IndexError:
				requested = "ERR"
			if requested == "ERR":
				#print "Err\n"	
				pass
			elif requested == False:
				rxQ.append(package)
				print "not requested\n"
			elif requested == True:
				clientsock.send( str(package.payload) + "\n" )
		else:
			dummy = 0
		clientsock.setblocking(False)
		try:
			data = clientsock.recv(BUFF)
		except IOError as e:
			if e.errno == errno.EWOULDBLOCK:#errno.EWOULDBLOCK:
				pass
		if "close\r\n" == data.strip(): break
		#Parse the incoming data 
		if data.strip() != '':
			parse_incoming_data(name, data.strip())

	#if we get here then it means the connection was closed. Therefore, remove the connection from connList.
	connList.remove(name)
	clientsock.close()
	print name, "- closed connection" #log on console


#======================================================================================================
# MODULE HANDLER 
#======================================================================================================
'''
	module_handler maintains the connection to each module. 
	In addition, this thread pre-processes and posts the incoming data to the general Queue, gxQ

	Unlike the client_handler, the module_handler re-initiates the module connections on a different socket. This way we separate the data streams.

	See client_handler.
'''
#def module_handler(clientsock,addr, rxQ, txQ, givenName, connList):
def module_handler(port, rxQ, givenName, connList):
	print "beginning module handler\n"
	breakcnter = 0
	name = givenName
	clientsock = socket(AF_INET, SOCK_STREAM)
	client_address = (name, port)
	clientsock.connect(client_address)

	while 1:
		data = ''
		# need to make a list of all known names and pre check the list for connections
		#if rxQ.empty() == False:
		if rxQ:
			try:
				package = rxQ.pop()
				requested = module_requested(name, package.dstAddr)
			except IndexError:
				requested = "ERR"
			if requested == "ERR":
				#print "Err\n"	
				pass
			elif requested == False:
				rxQ.append(package)
				print "not requested\n"
			elif requested == True:
				clientsock.send( str(package.payload) + "\n" )
		#clientsock.setblocking(False)
		#try:
		#	data = clientsock.recv(BUFF)
		#except IOError as e:
		#	if e.errno == errno.EWOULDBLOCK:#errno.EWOULDBLOCK:
		#		pass
				data = clientsock.recv(BUFF)
				if "close\r\n" == data.strip(): break
				'''
				Parse the incoming data for valid commands and post-card syntax
				'''
				clientPackage = postcard("tell", name, package.msgFrom, data)
				gxQ.put(clientPackage)
	connList.remove(name)
	clientsock.close()
	print addr, "- closed connection" #log on console

#======================================================================================================
# PARSE INCOMING DATA 
#======================================================================================================
'''
	Split the incoming data into separate tokens ( string words)
	First token is the command
	-> process the incoming data according to the command
	-> write a post card
	-> mail post card to general queue (gxQ)

	If receive a "!", split incoming data into multiple postcards.
'''
def parse_incoming_data(name, data):
	tokens = data.split(MULTI_LINE_SEP)
	for idata in tokens:
		idata = idata.split()
		cmd = idata[0]
		print cmd
		if cmd == "tell":
			dstAddr = idata[1]
			print dstAddr
			payloadtemp = idata[2:]
			payload = ''
			for stuff in payloadtemp:
				payload = payload + " " + stuff
			print payload
			package = postcard(cmd, name, dstAddr, payload.strip())
			gxQ.put(package)
		elif cmd == "list":
			dstAddr = name
			lines = "================================\n"
			payload = lines + "Client List\n"
			for conn in ClientList:
				payload = payload + conn + "\n"
			payload = payload + "\n\nModule List\n" + lines
			for conn in ModuleList:
				payload = payload + conn + "\n"
			print payload
			package = postcard(cmd, name, dstAddr, payload)
			gxQ.put(package)
		else:
			print "Did not recognize command", cmd, "\n"
def module_requested(name, dstAddr):
	return name == dstAddr

#======================================================================================================
# MAIN 
#======================================================================================================
if __name__=='__main__':
	
	#rxQ = Queue.Queue()
	#txQ = Queue.Queue()
	rxQ = deque() 
	txQ = deque()
	
	CLIENT_ADDR = (CLIENT_HOST, CLIENT_PORT)
	MODULE_ADDR = (MODULE_HOST, MODULE_PORT)
	
	thread.start_new_thread(acceptHndler, (CLIENT_ADDR, rxQ, txQ, ClientList, "Client"))
	thread.start_new_thread(acceptHndler, (MODULE_ADDR, txQ, rxQ, ModuleList, "Module"))
	thread.start_new_thread(TrafficManager, (gxQ, rxQ, txQ))
	print "I have started the threads\n"
	
	while 1:
		#do nothing
		h = 0
