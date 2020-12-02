#!/usr/bin/env python

# Script for simulating constant message from iXblue's Phins:
# $AIPOV,hhmmss.ssss,  h.hhh,   r.rrr,  p.ppp,   x.xxx,   y.yyy,   z.zzz,   e.ee,    f.ff,   g.gg,
# LL.LLLLLLLL, ll.llllllll,  a.aaa,   i.iii,   j.jjj,   k.kkk,  m.mmm,  n.nnn,  o.ooo,  c.ccc,hhhhhhhh*hh<CR><LF>

# Parameters:
# IP_DEVICE: IP of the device (laptop, etc.)
# IP_PHINS_SIMULATOR: IP of the device (another laptop, raspberry, etc.) where this script is launched

# To use this script with tour device please run:
# $ python phins_simulator.py --ip IP_DEVICE --port 9500


import socket
import sys
import argparse
import time

from datetime import datetime

# Retrieve arguments (IP_VIRTUAL_DEVICE and port)
parser = argparse.ArgumentParser()
parser.add_argument("--ip", help="ip address of the device")
parser.add_argument("--port", help="udp port selected in the device hmi")

args = parser.parse_args()

UDP_IP = str(args.ip)
UDP_PORT = int(args.port)

# Open a socket
sock = socket.socket(socket.AF_INET, # Internet
                     socket.SOCK_DGRAM) # UDP

while True:

	# Construct hour msg
	now = datetime.now()

	# Insert hour
	if (len(str(now.hour)) == 1):
		hour = "0" + str(now.hour)
	else:
		hour = str(now.hour)
	# Insert minutes
	if (len(str(now.minute)) == 1):
		hour = hour + "0" + str(now.minute)
	else:
		hour = hour + str(now.minute)
	# Insert seconds
	if (len(str(now.second)) == 1):
		hour = hour + "0" + str(now.second) + "."
	else:
		hour = hour + str(now.second) + "."
	# Insert microseconds
	if (len(str(now.microsecond)) == 1):
		hour = hour + "00000" + str(now.second)
	elif (len(str(now.microsecond)) == 2):
		hour = hour + "0000" + str(now.second)
	elif (len(str(now.microsecond)) == 3):
		hour = hour + "000" + str(now.second)
	elif (len(str(now.microsecond)) == 4):
		hour = hour + "00" + str(now.second)
	elif (len(str(now.microsecond)) == 5):
		hour = hour + "0" + str(now.second)
	else:
		hour = hour + str(now.second)

	# Construct final msg
	MSG = "$AIPOV," + hour + ",180.000,+120.000,+90.200,+400.000,+300.000,+200.000,+110.00,+100.00,+90.00,+45.00000000,-45.00000000,200.000,+200.000,+150.000,+100.000,+75.000,+50.000,+25.000,180.000,1234ABCD*69"
	# Send msg
	sock.sendto(MSG, (UDP_IP, UDP_PORT))
	
	# f = 25 Hz <-> T = 1/25 = 0.04 s
	time.sleep(0.04)
    
