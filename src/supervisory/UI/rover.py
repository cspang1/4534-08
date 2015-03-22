__author__ = 'tjd08a'
import serial
import sys
import time
from random import randint

port = None
for arg in sys.argv:
    port = arg

ser = serial.Serial(port, baudrate=57600, timeout=100)
send_num = False
counter = 0
seconds = 0
# h1, = plt.plot([], [])

# Reboot sequence below
ser.write('$$$')
time.sleep(1)
ser.write('reboot\r')
time.sleep(5)

clear_to_send = False

ready = False
while not ready:
    kickoff = ser.read(1)
    if(kickoff == "R"):
        second = ser.read(1)
        if(second == "D"):
            print "Ready to Transmit"
            send_num = True
            clear_to_send = True
            break

command_mode = False
#plt.show(h1)
while 1:
    #ser.write("hello world")
    bytesWaiting = ser.inWaiting()
    if bytesWaiting:
        # print bytesWaiting
        letter = ser.read(1)
        val = ord(letter)
           # print "Received %i" % val
        if (val > 5):
            print "Error: Invalid Message Received %d" % val
            sys.exit(0)
        else:
            if(val > 0 and val <= 5 and not command_mode):
                print "Entering Command Mode"
                command_mode = True

            if(command_mode):
                if(val == 1):
                    print "Move Forward"
                elif(val == 2):
                    print "Right Turn"
                elif(val == 3):
                    print "Move Back"
                elif(val == 4):
                    print "Left Turn"
                elif(val == 5):
                    one=1
                else:
                    print "Error: Invalid command %d issued" % val
                    sys.exit(1)
                ack = chr(0)
                ser.write(ack)
            else:
                if(val == 0):
                    clear_to_send = True
                else:
                    # Ignore idle commands
                    if(val != 5):
                        print "Error: Invalid Response Received %d" % val

    if(send_num and not command_mode):
        if(clear_to_send):
            value = randint(0,100)
            print value
            to_ascii = chr(value)
            ser.write(to_ascii)
            clear_to_send = False