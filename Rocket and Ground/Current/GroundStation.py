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
def bindListener(groundIP, groundPort):
    # socket to accept connections
    listener = socket(AF_INET, SOCK_DGRAM)
    # binding to port
    listener.bind((groundIP,groundPort))
    return listener

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
#----------------------------------------------------



#-------- MAIN -----------------------------
def main():
    # get command line input, currently validating # of args
    groundIP = None
    groundPort = None
    # TODO - Validate all aspects of input, perhaps breakout to error function
        if (len(sys.argv) != 2):
            print("ERROR: Invalid input parameters, terminating...")
            quit()
        else:
            groundIP = sys.argv[1]
            groundPort = int(sys.argv[2])

    listener = bindListener(groundIP, groundPort)
    localPath = getLogPath()
    localFile = open(localPath, "wb+")
    newline = "\n"

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

    localFile.close()
    listener.close()
    quit()
    return
#----------------------------------------------------
main()
