import serial
import sys
import time
import matplotlib.pyplot as plt
import numpy


def update_line(h1, x, y):
    h1.set_xdata(numpy.append(h1.get_xdata(), x))
    h1.set_ydata(numpy.append(h1.get_ydata(), y))
    plt.draw()


'''
__author__ = 'tjd08a'
'''
port = None
for arg in sys.argv:
    port = arg

ser = serial.Serial(port, baudrate=57600, timeout=10)
readNum = False
counter = 0
seconds = 0
# h1, = plt.plot([], [])

# Reboot sequence below
ser.write('$$$')
time.sleep(1)
ser.write('reboot\r')
time.sleep(3)

start = None
stop = None
initial = True
#plt.show(h1)
while 1:
    #ser.write("hello world")
    bytesWaiting = ser.inWaiting()
    if bytesWaiting:
        # print bytesWaiting
        letter = ser.read(1)
        val = ord(letter)

        if not readNum:
            if val >= 128 :
                #print "Ready To Receive"
                ser.write("r")
                readNum = True
                if initial:
                    start = time.time()
                    initial = False
        else:
            end = time.time()
           # print "Received %i" % val

            if (end - start >= 1):
                seconds += 1
                print "%d: Total Messages Received - %d" % (seconds, counter)
                start = time.time()

            if (val > 100):
                if(val == 255):
                    #print "Stop byte received"
                    ser.write('e')
                    readNum = False
                else:
                    print "Error: Incorrect value received"
                    print val

            #print val
            #update_line(h1, counter, val)
            counter += 1

            ser.flush()