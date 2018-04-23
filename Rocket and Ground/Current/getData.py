import time, serial

# configure the serial connections (the parameters differs on the device you are connecting to)
ser = serial.Serial(port, 9600, timeout = 1)

# Performs rs-232 communication  
def getData(port):
	#data - mutable bytearray
	data = bytearray()
	#return the number of bytes in the receive buffer
	avail = port.in_waiting

	startFlag = 0x7E
	stopFlag = 0x81
	escapeFlag = 0x99

	failureCount = 100 
	done = false

	#set read timeout to 1 second
	ser.timeout = 1
	# read until stop flag encountered
	if (avail > 0):
		# read byte by byte, place into data (mutable bytearray). If end flag, check for escape in data

		# read the port, 1 byte
		readValue = port.read(1)
		previousValue = -1
		
		# while readValue is not an escaped stop
		while(readValue != stopFlag and previousFlag != escapeFlag):
		    #parse readValue into data
            data.append(readValue)

            previousValue = readValue
            readValue = port.read(1)

		#fails if timeout, readValue != stopFlag or escapeFlag
		if (readValue == stopFlag or readValue == escapeFlag):
			data = -1
			failureCount++
		else:
			# if no failure, read in end flag
            data.append(readValue)
			#and confirm to remove from buffer (size = 1), not this way, but how??????????
			port.reset_input_buffer()

                    
			
	
	#TODO	store input to array correctly, bodged solution: parse through until unescaped stop byte found	
	#TODO timeout the reads 
	 

	# read message id, read message (size = #bytes in that message) If fail, set data to -1
	# if no failure, read in end flag and confirm to remove from buffer (size = 1)
	
	
	final = parseData(data)
	return final

