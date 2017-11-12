#!/usr/bin/env python3
from sys import *
import os.path
import struct
from socket import *
from datetime import datetime

#-------- Rocket.py  -------------------------------
# call structure: python Rocket.py groundIP groundPort
# sample call:    python Rocket.py "192.168.0.1" 7500

#-------- INITIALIZATION ---------------------
# get command line input
groundIP = argv[1]
groundPort = int(argv[2])


# get unique file name
initialPath = "/home/guest/Desktop/Avionics/flightData"
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



# open local file
localFile = open(localPath, "wb+")
# create and bind socket to port	
speaker = socket(AF_INET,SOCK_DGRAM)
#----------------------------------------------------




#-------- TRANSMIT ----------------------------
while 1:
        # get data to transmit
        dt = datetime.now()
        msg = str(dt.hour)+":"+str(dt.minute)+":"+str(dt.second)+":"+str(dt.microsecond)

        
        # encode transmission in variable teleData (string, encoded in ascii)
        teleData = msg.encode()

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

#----------------------------------------------------
