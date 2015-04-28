__author__ = 'tjd08a'
import serial
import sys
import time
import random as rand

rand.seed()
file = open('course.txt', 'r')
counter = 0
column_size = None
map = None
for line in file:
    if not counter:
        column_size = int(line)
        map = [0] * column_size * 3
        counter += 1
    else:
        ramp_location = int(line)
        map[ramp_location] = 1

print map
counter = 0
column = 0
port = None
for arg in sys.argv:
    port = arg

ser = serial.Serial(port, baudrate=57600, timeout=100)
ser.flushOutput()
ser.flushInput()

send_num = True

lower_bound = 5
middle_bound = 61
upper_bound = 155
ramp_angle = 20
variance = 10
lower_variance = lower_bound + variance
middle_variance_low = middle_bound - variance
middle_variance_high = middle_bound + variance

turned = False
'''
# Reboot sequence below
ser.write('$$$')
time.sleep(1)
ser.write('reboot\r')
time.sleep(5)
'''

clear_to_send = True

'''
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
'''

command_mode = False
while 1:
    bytesWaiting = ser.inWaiting()
    if bytesWaiting:
        # print bytesWaiting
        letter = ser.read(1)
        val = ord(letter)
           # print "Received %i" % val
        if (val > 5):
            print "Error: Invalid Message Received %d" % val
            #sys.exit(0)
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
                    pass
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
        right1 = 0
        right2 = 0
        left2 = 0
        left1 = 0
        angle = 0

        if(clear_to_send):
            if counter % column_size == 0 and not turned and counter>0:
                right1 = 0
                right2 = 0
                left1 = 0
                left2 = 0
                turned = True
                column += 1
            else:
                turned = False
                if column == 0:
                    right1 = 0
                    right2 = 0
                    left1 = rand.randint(lower_bound, lower_variance)
                    left2 = rand.randint(lower_bound, lower_variance)
                elif column == 1:
                    is_upper = rand.randint(0,1)
                    if is_upper:
                        right1 = rand.randint(middle_bound, middle_variance_high)
                        difference = right1 - middle_bound
                        right2 = middle_bound - difference

                        left1 = rand.randint(middle_variance_low, middle_bound)
                        difference = middle_bound - left1
                        left2 = middle_bound + difference
                    else:
                        left1 = rand.randint(middle_bound, middle_variance_high)
                        difference = left1 - middle_bound
                        left2 = middle_bound - difference

                        right1 = rand.randint(middle_variance_low, middle_bound)
                        difference = middle_bound - right1
                        right2 = middle_bound + difference
                else:
                    right1 = rand.randint(lower_bound,lower_variance)
                    right2 = rand.randint(lower_bound,lower_variance)
                    left1 = 0
                    left2 = 0

            word = ""

            print map[counter]
            if map[counter] == 1:
                angle_value = ramp_angle
            else:
                angle_value = 0

            if not turned:
                counter += 1

            to_ascii = chr(right1)
            to_ascii2 = chr(right2)
            to_ascii3 = chr(left1)
            to_ascii4 = chr(left2)
            to_ascii5 = chr(angle_value)
            word = to_ascii + to_ascii2 + to_ascii3 + to_ascii4 + to_ascii5
            ser.write(word)

            clear_to_send = False
            if counter >= len(map):
                sys.exit(0)