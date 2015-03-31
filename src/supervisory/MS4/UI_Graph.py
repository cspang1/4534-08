from Tkinter import *
from PIL import ImageTk
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
import sys
import time
import serial
import matplotlib.pyplot as plt
__author__ = 'tjd08a'

# Using user given port, open a serial connection
port = None
for arg in sys.argv:
    port = arg
ser = serial.Serial(port, baudrate=57600, timeout=10)

data_points = 0 # Data points collected from serial connection
path_img = "Images/" # Path to find pictures for GUI
is_started = False # True if GUI will start receiving data
command_mode = False # True if user can input commands in the GUI
clear_to_send = False # True if the GUI can send a message
command_sent = False # True if a command has been inputted
send_start = True # True if the Wifly connection does an initial handshake

# Configuration Variables
delay = 35 # Time for method to interrupt/how often to check for serial messages
reset_sleep = 10 # How long to make the wifly connection sleep at start up
y_limit = 100 # Biggest data value from serial connection to expect
up_number = 1 # Value to represent the Up Direction
back_number = 3 # Value to represent the Down/Back direction
right_number = 2 # Value to represent a Right Turn
left_number = 4 # Value to represent a Left Turn
idle_number = 5 # Value to represent an Idle Command
ack = 0 # Value to represent an acknowledgement
# End Configuration

# Initiate the graph for sensor data
fig = plt.Figure()
ax = fig.add_subplot(111)
ax.set_ylim(0, y_limit)
ax.set_xlim(0, 10)
ax.set_xlabel("Data Points")
ax.set_ylabel("Sensor Reading")
ax.set_title("Sensor Data")
xdata, ydata = [], []
line, = ax.plot(xdata, ydata, 'r-')

# Reboot sequence below
ser.write('$$$')
time.sleep(1)
ser.write('reboot\r')
time.sleep(reset_sleep)

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

    # Switches GUI to command mode
    if not command_mode:
        command_mode = True
        clear_to_send = True

    # Sends a command if one hasn't been already been sent
    if clear_to_send and command_mode:
        val = chr(up_number)
        ser.write(val)
        command_sent = True

# Callback for back button
def back_call():
    global command_mode, clear_to_send
    global command_sent, back_number

    if not command_mode:
        command_mode = True
        clear_to_send = True

    if clear_to_send and command_mode:
        val = chr(back_number)
        ser.write(val)
        command_sent = True

# Callback for left button
def left_call():
    global command_mode, clear_to_send
    global command_sent, left_number

    if not command_mode:
        command_mode = True
        clear_to_send = True

    if clear_to_send and command_mode:
        val = chr(left_number)
        ser.write(val)
        command_sent = True

# Callback for right button
def right_call():
    global command_mode
    global clear_to_send
    global command_sent
    global right_number

    if not command_mode:
        command_mode = True
        clear_to_send = True

    if clear_to_send and command_mode:
        val = chr(right_number)
        ser.write(val)
        command_sent = True

# Callback for hal or center button
def hal_call(event=None):
    global command_mode

    # Turns off command mode if on
    # Does nothing if not in command mode
    if command_mode:
        command_mode = False
        log_txt = "Exiting command mode, opening pod bay doors\n"
        update_log(log_txt)
    else:
        log_txt = "I can't let you do that Dave, this button does nothing.\n"
        update_log(log_txt)

# Callback for start button
def start_call(event=None):
    global is_started, start
    global log_panel, send_start

    start.flash() # Flash start button
    # Lets the GUI start receiving commands if it's not doing so
    if not is_started:
        is_started = True
        log_txt = "Control Panel Will Begin Receiving Data\n"
        update_log(log_txt)

        # Sends a handshake if enabled
        if send_start:
            ser.write('RD')

# Callback method that occurs after user specified delay
# Handles data processing and non-GUI actions
def data_callback():
    global fig, line, ax
    global xdata, ydata
    global is_started, command_mode
    global data_points, clear_to_send
    global command_sent, y_limit
    global idle_number, ack

    # Only activates if the GUI has been started
    if is_started:

        # Checks for data from the serial connection
        # True if there is data waiting
        bytes_waiting = ser.inWaiting()
        if bytes_waiting:
            # Reads a byte from the connection
            # Converts the ASCII to a number
            letter = ser.read(1)
            val = ord(letter)

            if command_mode:
                # Receives a non-ack value in command mode
                if val != 0:
                    error_text = "Error: Invalid command mode value received %d\n" % val
                    update_log(error_text)
                else:
                    clear_to_send = True
            else:
                # Receives a value outside the expected range
                if(val > y_limit):
                    error_text = "Error: Invalid value received %d\n" % val
                    update_log(error_text)
                else:
                    # If valid data has been received, appends it to the graph
                    xdata.append(data_points)
                    ydata.append(val)
                    line.set_xdata(xdata)
                    line.set_ydata(ydata)
                    xmin, xmax = ax.get_xlim()

                    # Increases the size of the x-axis if enough data points
                    # have been received.
                    if data_points > xmax:
                        ax.set_xlim(xmin, 2*xmax)

                    # Updates graph
                    fig.canvas.draw()
                    data_points += 1

                    ser.write(chr(ack))

        # If a command hasn't been sent by the GUI,
        # sends an idle command. Used to keep the connection alive.
        if command_mode:
            if not command_sent:
                idle = chr(idle_number)
                ser.write(idle)
            else:
                command_sent = False

    root.after(delay, data_callback)

# Builds/lays out GUI
root = Tk()
intro_txt = "Rover 9000 Control Panel"
straight_arrow = ImageTk.PhotoImage(file=path_img + "straight.png")
right_arrow = ImageTk.PhotoImage(file=path_img + "right_turn.png")
left_arrow = ImageTk.PhotoImage(file=path_img + "left_turn.png")
back_arrow = ImageTk.PhotoImage(file=path_img + "back.png")
hal_img = ImageTk.PhotoImage(file=path_img + "hal.jpg")
rover_forward = ImageTk.PhotoImage(file=path_img + "rover_forward.png")
rover_backward = ImageTk.PhotoImage(file=path_img + "rover_backward.png")

intro = Label(root, text=intro_txt, bg="azure", font=('Arial', 22))
intro.grid(row=0,column=0,columnspan=5)

hal = Button(root, image=hal_img, command=hal_call)
hal.grid(row=2,column=1)

up = Button( root, image=straight_arrow, command=up_call, bg="red")
up.grid(row=1,column=1)

back = Button( root, image=back_arrow, command=back_call, bg="red")
back.grid(row=3,column=1, pady=(0,20))

left = Button( root, image=left_arrow, command=left_call, bg="red")
left.grid(row=2,column=0, padx=(10,0))

right = Button( root, image=right_arrow, command=right_call, bg="red")
right.grid(row=2,column=2)

exit_button = Button(root, text="EXIT", bg="firebrick", fg="white", command=exit_call, font=('Arial',22))
exit_button.grid(row=3,column=2)

start = Button(root, text="START", bg="green", fg="white", font=('Arial',22), command=start_call)
start.grid(row=3,column=0)

log_panel = Text(root, height=2, width=40)
log_panel.grid(row=4, column=0, columnspan=3, sticky='nsew')

# Attach scrollbar to log panel text box
scroll = Scrollbar(root, command = log_panel.yview)
scroll.grid(row=4, column=3, padx=(0,2), sticky='nsew')
log_panel['yscrollcommand'] = scroll.set

canvas = FigureCanvasTkAgg(fig, master=root)
canvas.show()
canvas.get_tk_widget().grid(column=4, row=1, rowspan=4)

# Binds callback methods to the buttons
root.bind('<Up>', up_press)
root.bind('<Down>',back_press)
root.bind('<Left>',left_press)
root.bind('<Right>',right_press)
root.bind('<Escape>',exit_call)
root.bind('<Return>', start_call)
root.bind('<space>', hal_call)
root.configure(bg="azure")
root.after(delay, data_callback)
root.mainloop()
