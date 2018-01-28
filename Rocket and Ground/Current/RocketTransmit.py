#!/usr/bin/env python3
from sys import *
import os.path
import struct
from socket import *
from datetime import datetime

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
def getData(clusterNum):
	data = 0
	# Perform async call to the rs-232 port associated with clusterNum
	# assign cluster numbers
	# check if data, skip if none, else read data, timeout (below in a function, call serialRead)
		#read 1 byte at a time, parsing data
	# if timeout value, data = -1
	# perform asyc read, if fail, increment counter (reset if success), if fail count too large, reset count and issue restart to cluster
	
	finalData = parseData(data)
	return finalData
	
def finalData (data):
	# if data = 0, set cluster not ready message
	# if data = -1, set error message
	# else de-encapsulate
		# If gateworks needs to process id, parse and handle w/ functions
		# prep for transmit
	return finalData
	
#----------------------------------------------------

#-------- MAIN ----------------------------
def main():
    # get command line input, currently validating # of args
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
    # create and bind socket to port
    speaker = socket(AF_INET,SOCK_DGRAM)
	# Initialize microcontrollers
	
    #TODO - alive test, handshake?
    #TODO - section for additional functions/socket declarations for sending commands.
    #-------- TRANSMIT ----------------------------
    while 1:
        # get data to transmit
        dt = datetime.now()
        msg = str(dt.hour)+":"+str(dt.minute)+":"+str(dt.second)+":"+str(dt.microsecond)
		
		#iterate ports,

        # encode transmission in variable teleData (string, encoded in ascii)
        teleData = msg.encode() #TODO encode as UTF-8 (binary?)

        # write data to file TODO
        writing = msg + "\n"
        localFile.write(writing.encode())

        # send data to ground
        speaker.sendto(teleData,(groundIP,groundPort))

        # exit condition
        if(msg == "0"):
            break

    # close file, socket, and exit
    localFile.close()
    speaker.close()
    quit()
    return
#----------------------------------------------------
main()
