from Tkinter import *
from PIL import ImageTk
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
import sys
import time
import serial
import matplotlib.pyplot as plt
__author__ = 'tjd08a'

port = None
for arg in sys.argv:
    port = arg
ser = serial.Serial(port, baudrate=57600, timeout=10)

data_points = 0
path_img = "Images/"
is_started = False
command_mode = False
clear_to_send = False
delay = 20

fig = plt.Figure()
ax = fig.add_subplot(111)
ax.set_ylim(0, 100)
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
time.sleep(5)

def exit_call(event=None):
    global root
    root.destroy()
    sys.exit(0)

def up_press(event):
    global up, hal, rover_forward, hal_img
    hal.config(image=rover_forward)
    up.flash()
    up.invoke()
    hal.after(250, lambda: hal.config(image=hal_img))

def back_press(event):
    global back
    global rover_backward

    hal.config(image=rover_backward)
    back.flash()
    back.invoke()
    hal.after(250, lambda: hal.config(image=hal_img))

def left_press(event):
    global left
    left.flash()
    left.invoke()

def right_press(event):
    global right
    right.flash()
    right.invoke()

def up_call():
    global command_mode
    global clear_to_send

    if not command_mode:
        command_mode = True
        clear_to_send = True
    else:
        if clear_to_send:
            val = chr(1)
            ser.write(val)

def back_call():
    global command_mode
    global clear_to_send

    if not command_mode:
        command_mode = True
        clear_to_send = True
    else:
        if clear_to_send:
            val = chr(3)
            ser.write(val)

def left_call():
    global command_mode
    global clear_to_send

    if not command_mode:
        command_mode = True
        clear_to_send = True
    else:
        if clear_to_send:
            val = chr(4)
            ser.write(val)

def right_call():
    global command_mode
    global clear_to_send

    if not command_mode:
        command_mode = True
        clear_to_send = True
    else:
        if clear_to_send:
            val = chr(2)
            ser.write(val)

def hal_call():
    print "I can't let you do that Dave"

def start_call(event=None):
    global is_started
    global start
    global log_panel

    start.flash()
    if not is_started:
        is_started = True
        log_txt = "Control Panel Will Begin Receiving Data\n"
        log_panel.insert(END, log_txt)
        ser.write('RD')

def data_callback():
    global fig
    global line
    global xdata
    global ydata
    global ax
    global canvas
    global is_started
    global command_mode
    global log_panel
    global data_points
    global clear_to_send

    if is_started:
        bytes_waiting = ser.inWaiting()
        if bytes_waiting:
            letter = ser.read(1)
            val = ord(letter)

            if command_mode:
                if val != 0:
                    error_text = "Error: Invalid command mode value received %d\n" % val
                    log_panel.insert(END, error_text)
                else:
                    clear_to_send = True
            else:
                if(val > 100):
                    error_text = "Error: Invalid value received %d\n" % val
                    log_panel.insert(END, error_text)
                else:
                    xdata.append(data_points)
                    ydata.append(val)
                    line.set_xdata(xdata)
                    line.set_ydata(ydata)
                    xmin, xmax = ax.get_xlim()

                    if data_points > xmax:
                        ax.set_xlim(xmin, 2*xmax)

                    fig.canvas.draw()
                    data_points += 1
                ser.write(chr(0))
        if command_mode:
            idle = chr(5)
            ser.write(idle)

    root.after(delay, data_callback)

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

scroll = Scrollbar(root, command = log_panel.yview)
scroll.grid(row=4, column=3, padx=(0,2), sticky='nsew')
log_panel['yscrollcommand'] = scroll.set

canvas = FigureCanvasTkAgg(fig, master=root)
canvas.show()
canvas.get_tk_widget().grid(column=4, row=1, rowspan=4)

root.bind('<Up>', up_press)
root.bind('<Down>',back_press)
root.bind('<Left>',left_press)
root.bind('<Right>',right_press)
root.bind('<Escape>',exit_call)
root.bind('<Return>', start_call)
root.configure(bg="azure")
root.after(delay, data_callback)
root.mainloop()
