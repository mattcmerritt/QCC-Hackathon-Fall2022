import serial
import time
from winotify import Notification, audio
from tkinter import *
from tkinter import ttk

personalPort = None
# Tkinter frame instance
root = Tk()
# Setting geometry of frame
root.geometry("500x250")

# Getting port entry from User
def getPort():
    personalPort = portEntry.get()
    print(personalPort)

# Label
ttk.Label(root, text="Input the Port")

# User entry for the port
portEntry = Entry(root, width=40)
portEntry.focus_set()
portEntry.pack()

# Buttons
ttk.Button(root, text="Submit", command=getPort).pack(pady=20)

root.mainloop()

# If port is not empty, latch onto port and read the line
def checkPort():
    if personalPort != None:
        # Initializing port connection
        serialPort = serial.Serial(port = personalPort, baudrate=115200, bytesize=8, timeout=2, stopbits=serial.STOPBITS_ONE)
        # If available connection readLine
        if serialPort.in_waiting > 0:
            serialString = serialPort.readLine()
            # If theft string, call notification
            notification(serialString)

# Setting up windows Notification
notifyStolen = Notification(app_id = "Theft Prevention Script",
                    title = "THEFT ALERT",
                    msg = "Your item is being stolen",
                    duration="long")
notifyStolen.set_audio(audio.LoopingAlarm, loop=True)

notifyActive = Notification(app_id = "Theft Prevention Script",
                    title = "ALARM ACTIVE ALERT",
                    msg = "Your alarm has been primed",
                    duration="short")
notifyActive.set_audio(audio.Default, loop=False)

notifyDisabled = Notification(app_id = "Theft Prevention Script",
                    title = "ALARM DISABLED ALERT",
                    msg = "Your alarm has been disabled",
                    duration="short")
notifyDisabled.set_audio(audio.Default, loop=False)

# Show windows notification
def notification(serialString):
    if serialString == "BEING STOLEN":
        notifyStolen.show()
    if serialString == "ALARM ACTIVE":
        notifyActive.show()
    if serialString == "ALARM DISABLED":
        notifyDisabled.show()
