import serial
import sys
import time
'''
__author__ = 'tjd08a'
'''
port = None
for arg in sys.argv:
    port = arg

ser = serial.Serial(port, baudrate=57600, timeout=10)
readNum = False
counter = 0
time.sleep(3)

format_bits = 128
size = 15
format_bits += size
msg = chr(format_bits)
ser.write(msg)

kickoff = ser.read(1)
if(kickoff == "r"):
    print "Ready to Transmit"
else:
    print "Error: Cannot talk to host"
    sys.exit(1)

messages_sent = 0
#while
#ser.read(2)
word = None

while 1:
    if(messages_sent < size):
        word = chr(counter)
        ser.write(word)
        messages_sent += 1
    else:
        stop_byte = 255
        word = chr(stop_byte)
        ser.write(word)

        response = ser.read(1)
        if(response != "e"):
            print "Error: Stop byte not received"
            sys.exit(1)
        else:
            print "Stop byte received, restart"
            """
            if(size >= 15):
                size = 1
            else:
                size += 1
            """
            format_bits = 128 + size
            letter = chr(format_bits)
            ser.write(letter)

            kickoff = ser.read(1)
            if(kickoff == "r"):
                print "Ready to Transmit"
                messages_sent = 0
            else:
                print "Error: Cannot talk to host"
                sys.exit(1)

            """
            print "Stopping program"
            sys.exit(0)
            """

    if counter == 100:
        counter = 0
    else:
        counter += 1

    #time.sleep(1)
