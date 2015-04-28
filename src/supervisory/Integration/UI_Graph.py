from Tkinter import *
from PIL import ImageTk
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
import sys
import time
import serial
import matplotlib.pyplot as plt
__author__ = 'tjd08a'

# Using user given port, open a serial connection
port = "COM3"
testing = False
debug_mode = True

ser = None
if not testing:
    ser = serial.Serial(port, baudrate=57600, timeout=10)

data_points = 0 # Data points collected from serial connection
path_img = "Images/" # Path to find pictures for GUI
is_started = False # True if GUI will start receiving data
command_mode = False # True if user can input commands in the GUI
clear_to_send = False # True if the GUI can send a message
command_sent = False # True if a command has been inputted
send_start = True # True if the Wifly connection does an initial handshake
left_activated = False
mid_left_activated = False
right_activated = False
mid_right_activated = False
previous = 9
previous_angle = 0
current_command = None

# Configuration Variables
delay = 400 # Time for method to interrupt/how often to check for serial messages
reset_sleep = 5 # How long to make the wifly connection sleep at start up
y_upper_limit = 71 # Biggest data value from serial connection to expect
y_lower_limit = 5 # Lowest data value from serial connection to expect
angle = 20
up_number = 1 # Value to represent the Up Direction
back_number = 5 # Value to represent the Down/Back direction
right_number = 2 # Value to represent a Right Turn
left_number = 3 # Value to represent a Left Turn
idle_number = 5 # Value to represent an Idle Command
ack = 0 # Value to represent an acknowledgement
right_limit = 30
left_limit = 30
start_command = chr(0xF)
stop_command = chr(0x0)
release_command = chr(0xE)
# End Configuration

# Initiate the graph for sensor data
fig = plt.Figure()
ax = fig.add_subplot(211)
ax2 = fig.add_subplot(212)
ax.set_ylim(y_lower_limit, y_upper_limit)
ax.set_xlim(0, 10)
ax.set_ylabel("Sensor Reading (cm)")
ax.set_title("Sensor Data (Left)")
ax2.set_ylim(y_lower_limit, y_upper_limit)
ax2.set_xlim(0, 10)
ax2.set_ylabel("Sensor Reading (cm)")
ax2.set_title("Sensor Data (Right)")
xdata, ydata = [], []
ydata1, ydata2, ydata3 = [], [], []
line, = ax.plot(xdata, ydata, 'r-')
line1, = ax.plot(xdata, ydata, 'g-')
line2, = ax2.plot(xdata, ydata, 'b-')
line3, = ax2.plot(xdata, ydata, 'm-')

def reboot(event=None):
    global reset_sleep
    # Reboot sequence below
    ser.write('$$$')
    time.sleep(1)
    ser.write('reboot\r')
    time.sleep(reset_sleep)
    ser.flushInput()
    ser.flushOutput()

if not testing:
    reboot()
    # Reboot sequence below
    '''
    ser.write('$$$')
    time.sleep(1)
    ser.write('reboot\r')
    time.sleep(reset_sleep)
    ser.flushInput()
    ser.flushOutput()
    '''

# Add a message to the textbox in the GUI
# Used for debugging and errors
def update_log(txt):
    global log_panel
    log_panel.insert(END, txt)
    log_panel.yview(END)

# Terminates the GUI/Program
def exit_call(event=None):
    global root
    root.destroy()
    sys.exit(0)

# Triggers when up button is pressed
def up_press(event):
    global up, hal, rover_forward, hal_img

    hal.config(image=rover_forward) # Switches the up arrow image on the GUI
    up.flash() # Makes the up button flash
    up.invoke() # Invoke the callback for the up button
    hal.after(250, lambda: hal.config(image=hal_img)) # Switch back to the arrow

# Triggers when back button is pressed
def back_press(event):
    global back, rover_backward

    hal.config(image=rover_backward)
    back.flash()
    back.invoke()
    hal.after(250, lambda: hal.config(image=hal_img))

# Triggers when left button is pressed
def left_press(event):
    global left
    left.flash()
    left.invoke()

# Triggers when right button is pressed
def right_press(event):
    global right
    right.flash()
    right.invoke()

# Callback for up button
def up_call():
    global command_mode, clear_to_send
    global command_sent, up_number
    global current_command

    # Switches GUI to command mode
    if not command_mode:
        command_mode = True
        clear_to_send = True

    # Sends a command if one hasn't been already been sent
    if clear_to_send and command_mode:
        current_command = up_number
       # ser.write(val)
        command_sent = True

# Callback for back button
def back_call():
    global command_mode, clear_to_send
    global command_sent, back_number
    global current_command

    if not command_mode:
        command_mode = True
        clear_to_send = True

    if clear_to_send and command_mode:
        val = chr(back_number)
        current_command = None
        #ser.write(val)
        command_sent = True

# Callback for left button
def left_call():
    global command_mode, clear_to_send
    global command_sent, left_number
    global current_command

    if not command_mode:
        command_mode = True
        clear_to_send = True

    if clear_to_send and command_mode:
        #val = chr(left_number)
        current_command = left_number
       # ser.write(val)
        command_sent = True

# Callback for right button
def right_call():
    global command_mode
    global clear_to_send
    global command_sent
    global right_number
    global current_command

    if not command_mode:
        command_mode = True
        clear_to_send = True

    if clear_to_send and command_mode:
        #val = chr(right_number)
        current_command = right_number
        #ser.write(val)
        command_sent = True

# Callback for hal or center button
def hal_call(event=None):
    global command_mode, debug_mode
    global release_command, stop_command
    # Turns off command mode if on
    # Does nothing if not in command mode
    if debug_mode:
        command_mode = True

    if command_mode:
      #  command_mode = False
       # log_txt = "Exiting command mode, opening pod bay doors\n"
       # update_log(log_txt)
        if debug_mode:
            ser.write(stop_command)
        #log_txt = "I can't let you do that Dave, this button does nothing.\n"
       # update_log(log_txt)

# Callback for start button
def start_call(event=None):
    global is_started, start
    global log_panel, send_start
    global start_command

    start.flash() # Flash start button
    # Lets the GUI start receiving commands if it's not doing so
    # Sends a handshake if enabled
    if send_start:
      ser.write(start_command)

    '''
    if not is_started:
        is_started = True
        log_txt = "Control Panel Will Begin Receiving Data\n"
        update_log(log_txt)

    '''

def send_call(event=None):
    global movement_var, angle_var, current_command
    global left_number, right_number, up_number
    global movement_enter, angle_enter

    if current_command != None:
        value = None
        if current_command == right_number or current_command == left_number:
            if angle_var.get() != 0:
                value = angle_var.get()
            else:
                value = int(angle_enter.get())
        else:
            if movement_var.get() != 0:
                value = movement_var.get()
            else:
                value = int(movement_enter.get())
        print current_command
        command = current_command << 6
        command = command + value
        print "command %i" % command
        ser.write(chr(command))
    return

# Callback method that occurs after user specified delay
# Handles data processing and non-GUI actions
def data_callback():
    global fig, line, ax, ax2
    global line1, line2, line3
    global xdata, ydata
    global ydata1, ydata2, ydata3
    global is_started, command_mode
    global data_points, clear_to_send
    global command_sent, y_upper_limit, y_lower_limit
    global idle_number, ack
    global left_sensor, right_sensor
    global right_limit, left_limit
    global left_activated, right_activated
    global angle, map_tiles, previous, previous_angle
    global debug_mode

    # Only activates if the GUI has been started
    if is_started:

        # Checks for data from the serial connection
        # True if there is data waiting
        bytes_waiting = ser.inWaiting()
        if bytes_waiting:
            # Reads a byte from the connection
            # Converts the ASCII to a number
            val = ser.read(1)
            print val
            ''''
            letters = ser.read(5)
            val = ord(letters[0])
            val2 = ord(letters[1])
            val3 = ord(letters[2])
            val4 = ord(letters[3])
            val5 = ord(letters[4])
            '''
            if command_mode:
                if debug_mode:
                    clear_to_send = True
                # Receives a non-ack value in command mode
                if val != 0:
                    error_text = "Error: Invalid command mode value received %d\n" % val
                    update_log(error_text)
                else:
                    clear_to_send = True
            else:
                # Receives a value outside the expected range
                if(val > y_upper_limit and val < y_lower_limit):
                    error_text = "Error: Invalid value received %d\n" % val
                    #update_log(error_text)
                else:
                    pass
                    '''
                    if(val < right_limit or val2 < right_limit):
                        if(not right_activated):
                            right_sensor.config(bg="red")
                            right_activated = True
                    else:
                        if(right_activated):
                            right_sensor.config(bg="green")
                            right_activated = False

                    if(val3 < left_limit or val4 < left_limit):
                        if(not left_activated):
                            left_sensor.config(bg="red")
                            left_activated = True
                    else:
                        if(left_activated):
                            left_sensor.config(bg="green")
                            left_activated = False

                    # If valid data has been received, appends it to the graph
                    xdata.append(data_points)
                    ydata.append(val)
                    ydata1.append(val2)
                    ydata2.append(val3)
                    ydata3.append(val4)

                    line.set_xdata(xdata)
                    line.set_ydata(ydata)

                    line1.set_xdata(xdata)
                    line1.set_ydata(ydata1)

                    line2.set_xdata(xdata)
                    line2.set_ydata(ydata2)

                    line3.set_xdata(xdata)
                    line3.set_ydata(ydata3)

                    xmin, xmax = ax.get_xlim()

                    # Increases the size of the x-axis if enough data points
                    # have been received.
                    if data_points > xmax:
                        ax.set_xlim(xmin, xmax + 5)
                        ax2.set_xlim(xmin, xmax + 5)

                    # Updates graph
                    fig.canvas.draw()

                    if previous != None:
                        if(previous_angle == 20):
                            map_tiles[previous].config(bg="blue", fg="black", text="H")
                        else:
                            map_tiles[previous].config(bg="green", fg="green")

                    previous_angle = val5
                    if data_points < 8:
                        map_tiles[8 - data_points].config(bg="yellow", fg="yellow")
                        previous = 8 - data_points
                    elif data_points == 8:
                        map_tiles[10].config(bg="yellow", fg="yellow")
                        previous = 10
                    elif data_points < 17:
                        map_tiles[2 + data_points].config(bg="yellow", fg="yellow")
                        previous = 2 + data_points
                    elif data_points == 17:
                        map_tiles[29].config(bg="yellow", fg="yellow")
                        previous = 29
                    else:
                        map_tiles[46 - data_points].config(bg="yellow", fg="yellow")
                        previous = 46 - data_points

                    if(46 - data_points == 21):
                        is_started = False
                        if(previous_angle == 20):
                            map_tiles[21].config(bg="blue", fg="black", text="H")
                    data_points += 1

                    ser.write(chr(ack))
                    '''

        # If a command hasn't been sent by the GUI,
        # sends an idle command. Used to keep the connection alive.
        if command_mode:
            if not command_sent:
                idle = chr(idle_number)
                if not debug_mode:
                    ser.write(idle)
            else:
                command_sent = False

    root.after(delay, data_callback)

# Builds/lays out GUI
root = Tk()
scroll_region = Canvas(root, width=1240, height=680)
scroll_region.grid(row=0, column=0, sticky = 'nsew')

container = Frame(scroll_region)
container.pack(fill="both", expand=True)
scroll_region.create_window((0,0), window=container, anchor="nw",
                                  tags="self.frame")

intro_txt = "Rover 9000 Control Panel"
straight_arrow = ImageTk.PhotoImage(file=path_img + "straight.png")
right_arrow = ImageTk.PhotoImage(file=path_img + "right_turn.png")
left_arrow = ImageTk.PhotoImage(file=path_img + "left_turn.png")
back_arrow = ImageTk.PhotoImage(file=path_img + "back.png")
hal_img = ImageTk.PhotoImage(file=path_img + "hal.jpg")
rover_forward = ImageTk.PhotoImage(file=path_img + "rover_forward.png")
rover_backward = ImageTk.PhotoImage(file=path_img + "rover_backward.png")
rover = ImageTk.PhotoImage(file=path_img + "Rover2.png")
ramp = ImageTk.PhotoImage(file=path_img + "ramp2.jpg")
intro = Label(container, text=intro_txt, bg="azure", font=('Arial', 22))
intro.grid(row=0,column=0,columnspan=5)

hal = Button(container, image=hal_img, command=hal_call)
hal.grid(row=2,column=1)

up = Button(container, image=straight_arrow, command=up_call, bg="red")
up.grid(row=1,column=1)

back = Button(container, image=back_arrow, command=back_call, bg="red")
back.grid(row=3,column=1, pady=(0,20))

left = Button(container, image=left_arrow, command=left_call, bg="red")
left.grid(row=2,column=0, padx=(10,0))

right = Button(container, image=right_arrow, command=right_call, bg="red")
right.grid(row=2,column=2)

exit_button = Button(container, text="EXIT", bg="firebrick", fg="white", command=exit_call, font=('Arial',22))
exit_button.grid(row=3,column=2)

start = Button(container, text="START", bg="green", fg="white", font=('Arial',22), command=start_call)
start.grid(row=3,column=0)

movement_label = Label(container, text="Movement Amount(cm)")
movement_label.grid(row=4, column=1)

movement_var = IntVar(container)
movement_var.set(0)
movement_options = OptionMenu(container, movement_var, 0, 10, 20, 30, 40)
movement_options.config(bg="turquoise")
movement_options['menu'].config(bg="turquoise")
movement_options.grid(row=5, column=0)

movement_enter = Entry(container)
movement_enter.grid(row=5, column=1)

angle_label = Label(container, text="Movement Amount(degrees)")
angle_label.grid(row=6, column=1)

angle_var = IntVar(container)
angle_var.set(0)
angle_options = OptionMenu(container, angle_var, 0, 10, 15, 20, 25)
angle_options.config(bg="turquoise")
angle_options['menu'].config(bg="turquoise")
angle_options.grid(row=7, column=0)

angle_enter = Entry(container)
angle_enter.grid(row=7, column=1)

send_button = Button(container, text="SEND", bg="turquoise", font=('Arial',12), command=send_call)
send_button.grid(row=5,column=2)

reboot_button = Button(container, text="REBOOT", bg="turquoise", font=('Arial',12), command=reboot)
reboot_button.grid(row=7, column=2)

log_panel = Text(container, height=2, width=40)
#log_panel.grid(row=4, column=0, columnspan=3, sticky='nsew')

# Attach scrollbar to log panel text box
scroll = Scrollbar(container, command = log_panel.yview)
#scroll.grid(row=4, column=3, padx=(0,2), sticky='nsew')
log_panel['yscrollcommand'] = scroll.set

window_scroll = Scrollbar(root, command = scroll_region.yview)
window_scroll.grid(row=0, column=1, sticky='nsew')
scroll_region['yscrollcommand'] = window_scroll.set

canvas = FigureCanvasTkAgg(fig, master=container)
canvas.show()
canvas.get_tk_widget().grid(column=4, row=1, rowspan=3)

proximity_sensor = Frame(container)

sensor_label = Label(container, text="Sensor Statuses", bg="azure", font=('Arial', 16))
#sensor_label.grid(row = 5, column = 1)

left_sensor = Label(proximity_sensor, text="L", bg="green", font=('Arial', 22))
left_sensor.grid(row=0, column=0)

right_sensor = Label(proximity_sensor, text="R", bg="green", font=('Arial', 22))
right_sensor.grid(row=0, column=2)

tilt_sensor = Label(container, text="Ramp", bg="green", font=('Arial', 22))
#tilt_sensor.grid(row=7, column=1, pady=(10,0))

rover_icon = Label(proximity_sensor, image=rover)
rover_icon.grid(row=0, column=1)

terrain_map = Frame(container, borderwidth=5)
terrain_map.grid(row=0, column=5, rowspan = 3, padx=(10,0))

proximity_sensor.grid(row=3, column=5, pady=(1, 0))

map_tiles = []
for i in xrange(46):
    tile = Label(terrain_map, bg="green", fg="green", font=('Arial', 14),
                 bd=5, text="OK", relief=RAISED)
    if(i<8):
        tile["bg"] = "red"
        tile["fg"] = "red"
        tile.grid(row=i+1, column=0)
    elif(i<18):
        tile.grid(row=i-8, column=1)
        map_tiles.append(tile)
    elif(i<28):
        tile.grid(row=i-18, column=2)
        map_tiles.append(tile)
    elif(i<38):
        tile.grid(row=i-28, column=3)
        map_tiles.append(tile)
    else:
        tile["bg"] = "red"
        tile["fg"] = "red"
        tile.grid(row=i-37, column=4)

map_tiles[9].config(bg="yellow", fg="yellow")

width = container.winfo_width()
height = container.winfo_height()

scroll_region.configure(scrollregion=(0,0,width,700))
#scroll_region.configure(scrollregion=scroll_region.bbox("all"))
# Binds callback methods to the buttons
root.bind('<Up>', up_press)
root.bind('<Down>',back_press)
root.bind('<Left>',left_press)
root.bind('<Right>',right_press)
root.bind('<Escape>',exit_call)
root.bind('<Return>', start_call)
root.bind('<space>', hal_call)
root.configure(bg="azure")
root.resizable(width=FALSE, height=FALSE)
root.after(delay, data_callback)
root.mainloop()
