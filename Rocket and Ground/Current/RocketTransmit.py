#!/usr/bin/env python3
from sys import *
import os.path
import struct
from socket import *
from datetime import datetime
import serial

#-------- Rocket.py  -------------------------------
# call structure: python Rocket.py groundIP groundPort
# sample call:    python Rocket.py "192.168.0.1" 7500
# proper structure is to place this in the same directory as the log files directory

#-------- INITIALIZATION ---------------------

# Returns unique logfile path in logData
def getLogPath ():
    # get unique file name
    initialPath = "flightCode/logData"
    fileID = len(os.listdir(initialPath))
    localPath = initialPath+"/flightLog_"+str(fileID)

    # ensure is unique
    # NOTE: WHEN PROCESSING LOG DATA, ALL LOGS MUST BE REMOVED FROM THE FOLDER
    nameInvalid = 1
    while(nameInvalid):
        if(os.path.isfile(localPath)):
            print("LOG FILE MISSING, CURRENT ATTEMPTED PATH:\n" + localPath + "\n ")
            fileID = fileID + 100
            print("INCREMENTING FILE PATH BY 100, NEW PATH ID:" + str(fileID)+"\n")
            localPath = initialPath+"/flightLog_"+str(fileID)
        else:
            nameInvalid = 0

    return localPath
    
# Performs rs-232 communication  
def getData(port):
	data = 0
	avail = port.in_waiting
	startFlag = 0x7E
	stopFlag = 0x81
	escapeFlag = 0x99
	failureCount = 100 
	# Read if data present on port
	if (avail > 0):
		# discard bytes until start flag reached (size = 1, in loop) TODO create some limit to set failure if needed
		
		test = hex(port.read(1)) # Check if first byte is start flag
		ready = (test == startFlag)
		
		currentCount = 0 # Read up to failureCount bytes TODO handle escape byte 
		while (!ready && (currentCount < failureCount) && (hex(port.read(1)) != startFlag)):
			currentCount += 1
		
		if (currentCount == 100):
			print("sensor failure: recovery not yet implemented, exiting to preserve software integrity")
			endTelemetry()
		else:
			currentCount = 0
			
	
		#TODO	store input to array correctly, bodged solution: parse through until unescaped stop byte found	

		# TODO onboard processing later, for now just sending to ground
		# read message id, read message (size = #bytes in that message) If fail, set data to -1
		# if no failure, read in end flag and confirm to remove from buffer (size = 1)
	
	final = parseData(data)
	return final
	
def parseData (data):
	finalData = data
	# if data = 0, set cluster not ready message
	# if data = -1, set error message, tally errors for port and reset MC if needed
	# else de-encapsulate
		# convert from immutable "bytes" object to mutable "bytearray" object
		# read byte by byte, remove escape bytes
		# process ID, parse message if for Gateworks
		# put into object type for transmission (bytes or bytearray)
	return finalData
	
	
def endTelemetry (localFile, speaker, port):
	#TODO close port
	localFile.close()
    speaker.close()
    quit()
	
#----------------------------------------------------

#-------- MAIN ----------------------------
def main():
    # get command line input, currently validating # of args ONLY
    groundIP = None
    groundPort = None
    # TODO - Validate all aspects of input, function to handle error cases
    if (len(sys.argv) != 2):
        print("ERROR: Invalid input parameters, terminating...")
        quit()
    else:
        groundIP = argv[1]
        groundPort = int(argv[2])
    localPath = getLogPath()

    # open local file
    localFile = open(localPath, "wb+")
    # create and bind socket to port, TODO open rs-232 ports with timeout = x seconds
    speaker = socket(AF_INET,SOCK_DGRAM)
	# TODO Initialize microcontrollers
	
    #TODO - alive test, handshake?
    #TODO - section for additional functions/socket declarations for sending commands.
    #-------- TRANSMIT ----------------------------
    while 1:
        #TODO (needs changed) get data to transmit
        dt = datetime.now()
        msg = str(dt.hour)+":"+str(dt.minute)+":"+str(dt.second)+":"+str(dt.microsecond)
		
		#TODO iterate ports, create variable to loop over

        # encode transmission in variable teleData (string, encoded in ascii)
        teleData = msg.encode() #TODO encode in different format

        # write data to file TODO
        writing = msg + "\n"
        localFile.write(writing.encode())

        # send data to ground
        speaker.sendto(teleData,(groundIP,groundPort))

        # TODO exit condition
        if(msg == "0"):
            break

    # close file, socket, ports, and exit
    endTelemetry(localFile, speaker, port)
    return
#----------------------------------------------------
main()

