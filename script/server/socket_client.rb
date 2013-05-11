require 'socket'      # Sockets are in standard library

host= 'localhost'
port = 7456

s = TCPSocket.open(host, port)

puts "please input file name"

s.puts gets

while line = s.gets   # Read lines from the socket
	  puts line.chop      # And print with platform line terminator
end
s.close               # Close the socket when done
