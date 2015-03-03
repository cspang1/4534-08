from Tkinter import *
from PIL import ImageTk
import sys

__author__ = 'tjd08a'

path_img = "Images/"
def exit_call(event=None):
    sys.exit(0)

def up_press(event):
    global up, hal, rover_forward, hal_img
    hal.config(image=rover_forward)
    up.flash()
    up.invoke()
    hal.after(250, lambda: hal.config(image=hal_img))

def back_press(event):
    global back
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
    print "up"

def back_call():
    print "back"

def left_call():
    print "left"

def right_call():
    print "right"

def hal_call():
    print "I can't let you do that Dave"

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
intro.grid(row=0,column=0,columnspan=3)

hal = Button(root, image=hal_img, command=hal_call)
hal.grid(row=2,column=1)

up = Button( root, image=straight_arrow, command=up_call, bg="red")
up.grid(row=1,column=1)

back = Button( root, image=back_arrow, command=back_call, bg="red")
back.grid(row=3,column=1)

left = Button( root, image=left_arrow, command=left_call, bg="red")
left.grid(row=2,column=0)

right = Button( root, image=right_arrow, command=right_call, bg="red")
right.grid(row=2,column=2)

exit_button = Button(root, text="EXIT", bg="firebrick", fg="white", command=exit_call, font=('Arial',22))
exit_button.grid(row=3,column=2)

start = Button(root, text="START", bg="green", fg="white", font=('Arial',22))
start.grid(row=3,column=0)

root.bind('<Up>', up_press)
root.bind('<Down>',back_press)
root.bind('<Left>',left_press)
root.bind('<Right>',right_press)
root.bind('<Escape>',exit_call)
root.configure(bg="azure")
root.mainloop()
