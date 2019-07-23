#!/usr/bin/env python

# Simple CGI-enabled web server for mycced listening on 8888

CGI_DIR = "/"
DEFAULT_PORT = 8888
 
import sys 
import BaseHTTPServer
import CGIHTTPServer
import cgitb; cgitb.enable()

def start_server(port):
	server = BaseHTTPServer.HTTPServer
	handler = CGIHTTPServer.CGIHTTPRequestHandler
	
	server_address = ("", port)
	handler.cgi_directories = [CGI_DIR]
	httpd = server(server_address, handler)
	print "Starting CGI-enabled mini web server listening on %d" % (port)
	try:
		httpd.serve_forever()
	except KeyboardInterrupt:
		print "\nWeb server Interrupted by user\n"

def main():
	total = len(sys.argv)
	port = DEFAULT_PORT
	if total != 2:
		print "Invalid arguments. Must be: runmserv.py <port>. Taking the default port."
	else:
		try:
			port = int(sys.argv[1])
		except ValueError:
			print "Unrecognized port. Taking the default port instead."		
	start_server(port)

main()


