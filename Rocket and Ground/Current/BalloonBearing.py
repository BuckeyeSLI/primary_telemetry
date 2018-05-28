#!/usr/bin/env python3
from sys import *
import os.path
import struct
import datetime
import serial
from math import *
from tkinter import *


# Global values
window = Tk()
window.geometry('400x200')
window.title("HAB Antenna Calculator")
serialIn = 0 #TODO serial port goes here


#-----------------------------------------------------------------------------------------------
def getBearing(latBall, lonBall, latGrnd, lonGrnd):
	
	latBall = radians(latBall)
	lonBall = radians(lonBall)
	latGrnd = radians(latGrnd)
	lonGrnd = radians(lonGrnd)
	
	x = cos(latBall) * sin(abs(lonBall-lonGrnd))
	y = cos(latGrnd) * sin(latBall) - sin(latGrnd) * cos(latBall) * cos(abs(lonBall-lonGrnd))
	bearing = round(degrees(atan2(x,y)), 3)
	
	return "  Brg:    " + str(bearing) + "  "
#------------------------------------------------------------------------------------------------   
def getElevation(latBall, lonBall, altBall, latGrnd, lonGrnd, altGrnd):
	
	# Earth radius in meters
	R = 6371000
	
	# Lats in radians
	radLatBall = radians(latBall)
	radLatGrnd = radians(latGrnd)
	radDLat = radians(latBall - latGrnd)
	radDLon = radians(lonBall - lonGrnd)
	
	a = sin(radDLat/2) * sin(radDLat/2) + cos(radLatGrnd) * cos(radLatBall) * sin(radDLon/2) * sin(radDLon/2)
	
	c = 2 * atan2(sqrt(a), sqrt(1-a))
	
	dist = R * c
	height = altBall - altGrnd
	
	elevation = degrees(atan(height/dist))
	
	roundElev = round(elevation,2)
	
	return "  Elev:  " + str(roundElev) + "  "	
#------------------------------------------------------------------------------------------------
def getAllCalcs(elevationLbl, bearingLbl, latGrndTxt, lonGrndTxt, altGrndTxt, latBallTxt, lonBallTxt, altBallTxt):
	
	# Get current balloon values
	latBall = (float)(latBallTxt.get())
	lonBall = (float)(lonBallTxt.get())
	altBall = (float)(altBallTxt.get())
	
	
	# Get user input ground values
	latGrnd = (float)(latGrndTxt.get())
	lonGrnd = (float)(lonGrndTxt.get())
	altGrnd = (float)(altGrndTxt.get())
	
	# Get bearing, set it
	bearingLbl.configure(text=getBearing(latBall, lonBall, latGrnd, lonGrnd))
	# Get elevation, set it
	elevationLbl.configure(text=getElevation(latBall, lonBall, altBall, latGrnd, lonGrnd, altGrnd))

	return
#------------------------------------------------------------------------------------------------
def geodetic_to_ecef(lat, lon, alt): # Currently unused AND untested
	a = 6378137
	b = 6356752.3142
	f = (a-b)/a
	e_sq = f * (2-f)
	
	lamb = radians(lat)
	phi = radians(lon)
	s = sin(lamb)
	N = a/sqrt(1 - e_sq * s * s)
	
	sin_lambda = sin(lamb)
	cos_lambda = sin(lamb)
	sin_phi = sin(phi)
	cos_phi = cos(phi)
	
	x = (alt + N) * cos_lambda * cos_phi
	y = (alt + N) * cos_lambda * sin_phi
	z = (alt +(1-e_sq) * N) * sin_lambda
	
	return x, y, z	
#------------------------------------------------------------------------------------------------

def main():
	
	# Labels for columns
	latLbl = Label(window, text="Lat", font=("Arial Bold", 14))
	latLbl.grid(column=1,row=0)
	
	lonLbl = Label(window, text="Lon", font=("Arial Bold", 14))
	lonLbl.grid(column=2,row=0)
	
	altLbl = Label(window, text="Alt", font=("Arial Bold", 14))
	altLbl.grid(column=3,row=0)
	
	
	# HighAltitudeBalloon info
	ballLbl = Label(window, text="HAB:", font=("Arial Bold", 14))
	ballLbl.grid(column=0,row=1)
	
	latBallTxt = Entry(window, width=10, name="latBallTxt", font=("Arial", 12))
	latBallTxt.grid(column=1,row=1,padx=(10,10))
	
	lonBallTxt = Entry(window, width=10, name="lonBallTxt", font=("Arial", 12))
	lonBallTxt.grid(column=2,row=1,padx=(10,10))
	
	altBallTxt = Entry(window, width=10, name="altBallTxt", font=("Arial", 12))
	altBallTxt.grid(column=3,row=1,padx=(10,10))
	
	# Row spacer
	spacerLbl1 = Label(window, text="")
	spacerLbl1.grid(column = 0, row = 2)
	
	# Ground info
	grndLbl = Label(window, text="Grn:", font=("Arial Bold", 14))
	grndLbl.grid(column=0,row=3)
	
	latGrndTxt = Entry(window, width=10, name="latGrndTxt", font=("Arial", 12))
	latGrndTxt.grid(column=1,row=3)
	
	lonGrndTxt = Entry(window, width=10, name="lonGrndTxt", font=("Arial", 12))
	lonGrndTxt.grid(column=2,row=3)
	
	altGrndTxt = Entry(window, width=10, name="altGrndTxt", font=("Arial", 12))
	altGrndTxt.grid(column=3,row=3)

	# Row spacer
	spacerLbl2 = Label(window, text="")
	spacerLbl2.grid(column = 0, row = 4)

	# Output
	elevationLbl = Label(window, text="    N/A    ", font=("Arial","12"))
	bearingLbl = Label(window, text="    N/A    ", font=("Arial", "12"))
	
	elevationLbl.grid(column=2,row=5)
	bearingLbl.grid(column=3,row=5)
	
	# Submit input for calculation
	calculate = Button(window, text="Calculate", command= lambda: getAllCalcs(elevationLbl, bearingLbl, latGrndTxt, lonGrndTxt, altGrndTxt, latBallTxt, lonBallTxt, altBallTxt))
	calculate.grid(column=1,row=5)
	
	window.mainloop()
	return

main()
