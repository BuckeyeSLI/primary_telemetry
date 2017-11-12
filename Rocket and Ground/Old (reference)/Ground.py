#!/usr/bin/env python3
from sys import *
import os.path
import struct
from socket import *
from datetime import datetime

#-------- Ground.py  ---------------------------
# call structure: python Ground.py groundIP groundPort
# sample call:    python Ground.py "192.168.0.1" 7500

#-------- INITIALIZATION ---------------------
# server IP
groundIP = argv[1]
# port to bind server to
groundPort = int(argv[2])
# socket to accept connections 				
listener = socket(AF_INET, SOCK_DGRAM)
# binding to port
listener.bind((groundIP,groundPort))


# get unique file name (UPDATE FOR WINDOWS!!!!!!!!!!!)
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
newline = "\n"
#----------------------------------------------------



#-------- RECEIVE -----------------------------	
while 1:
        # get data
        teleData = listener.recvfrom(1024)
        print(teleData)
        # exit condition
        if(teleData == "0"):
                break
        # process rocket data (write to file)
        else:
                writing = teleData[0].decode() + newline
                localFile.write(writing.encode())
#----------------------------------------------------

localFile.close()
listener.close()
quit()




