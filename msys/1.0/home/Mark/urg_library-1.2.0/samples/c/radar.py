#!/usr/bin/env python3
from OSC import OSCServer
import sys
from time import sleep
import os
import OSC
import threading
import datetime
cc = OSC.OSCClient()
cc.connect(('192.168.0.252', 12002))   # localhost, port 57120
dd = OSC.OSCClient()
dd.connect(('192.168.0.250', 7111))   # localhost, port 57120

server = OSCServer( ("0.0.0.0", 12001) )
server.timeout = 0.001
run = True

def to250(pos, speed, msg="/mode"):
    global dd
    #unit = random.randint(5,5)
    oscmsg = OSC.OSCMessage()
    oscmsg.setAddress(msg)
    oscmsg.append(pos)
    oscmsg.append(speed)
    print oscmsg
    dd.send(oscmsg)

def click(pos, speed, msg="/button"):
    global cc
    #unit = random.randint(5,5)
    oscmsg = OSC.OSCMessage()
    oscmsg.setAddress(msg)
    oscmsg.append(pos)
    oscmsg.append(speed)
    print oscmsg
    cc.send(oscmsg)

# this method of reporting timeouts only works by convention
# that before calling handle_request() field .timed_out is 
# set to False
def handle_timeout(self):
    self.timed_out = True

# funny python's way to add a method to an instance of a class
import types
server.handle_timeout = types.MethodType(handle_timeout, server)

def cell_callback(path, tags, args, source):
    click(args[0],args[1])

def user_callback(path, tags, args, source):
    # which user will be determined by path:
    # we just throw away all slashes and join together what's left
    #user = ''.join(path.split("/"))
    # tags will contain 'fff'
    # args is a OSCMessage with data
    # source is where the message came from (in case you need to reply)
    print 'i ={:>5} , x ={:>15} , y ={:>15}'.format(args[0],args[1],args[2])
    print mode
    if args[0] == 3: 
        if mode[args[0]] == 0:
            if args[1] > -862.0 and args[1] < -544.0 and args[2] < -993.0 and args[2] > -1333.0:
                click(args[0],0)
            elif args[1] > -809.0 and args[1] < -538.0 and args[2] < -1682.0 and args[2] > -1922.0:
                click(args[0],1)
        elif mode[args[0]] == 1:
            if args[2] < -1038.0 and args[2] > -1257.0:
                if args[1] > -1238.0 and args[1] < -985.0:
                    click(args[0],0)
                elif args[1] < -720.0:
                    click(args[0],1)
                elif args[1] < -472.0:
                    click(args[0],2)
                elif args[1] < -225.0:
                    click(args[0],3)
        elif mode[args[0]] == 2:
            if args[2] < -1708.0 and args[2] > -1931.0:
                if args[1] > -1261.0 and args[1] < -1001.0:
                    click(args[0],0)
                elif args[1] < -715.0:
                    click(args[0],1)
                elif args[1] < -472.0:
                    click(args[0],2)
                elif args[1] < -269.0:
                    click(args[0],3)
    elif args[0] == 4: 
        if mode[args[0]] == 0:
            if args[1] > 594.0 and args[1] < 964.0 and args[2] < -1124.0 and args[2] > -1515.0:
                click(args[0],0)
            elif args[1] > 615.0 and args[1] < 894.0 and args[2] < -1805.0 and args[2] > -2055.0:
                click(args[0],1)
        elif mode[args[0]] == 1:
            if args[2] < -1231.0 and args[2] > -1440.0:
                if args[1] > 293.0 and args[1] < 494.0:
                    click(args[0],0)
                elif args[1] < 750.0:
                    click(args[0],1)
                elif args[1] < 992.0:
                    click(args[0],2)
                elif args[1] < 1203.0:
                    click(args[0],3)
        elif mode[args[0]] == 2:
            if args[2] < -1881.0 and args[2] > -2089.0:
                if args[1] > 286.0 and args[1] < 553.0:
                    click(args[0],0)
                elif args[1] < 759.0:
                    click(args[0],1)
                elif args[1] < 1021.0:
                    click(args[0],2)
                elif args[1] < 1223.0:
                    click(args[0],3)
    elif args[0] == 5: 
        if mode[args[0]] == 0:
            if args[1] > 631.0 and args[1] < 954.0 and args[2] < -2412.0 and args[2] > -2867.0:
                click(args[0],0)
            elif args[1] > 688.0 and args[1] < 1019.0 and args[2] < -3018.0 and args[2] > -3373.0:
                click(args[0],1)
        elif mode[args[0]] == 1:
            if args[2] < -2460.0 and args[2] > -2832.0:
                if args[1] > 319.0 and args[1] < 560.0:
                    click(args[0],0)
                elif args[1] < 839.0:
                    click(args[0],1)
                elif args[1] < 1104.0:
                    click(args[0],2)
                elif args[1] < 1283.0:
                    click(args[0],3)
        elif mode[args[0]] == 2:
            if args[2] < -3104.0 and args[2] > -3399.0:
                if args[1] > 328.0 and args[1] < 592.0:
                    click(args[0],0)
                elif args[1] < 838.0:
                    click(args[0],1)
                elif args[1] < 1097.0:
                    click(args[0],2)
                elif args[1] < 1324.0:
                    click(args[0],3)
    elif args[0] == 6: 
        if mode[args[0]] == 0:
            if args[1] > -903.0 and args[1] < -688.0 and args[2] < -2411.0 and args[2] > -2900.0:
                click(args[0],0)
            elif args[1] > -1119.0 and args[1] < -688.0 and args[2] < -3144.0 and args[2] > -3515.0:
                click(args[0],1)
        elif mode[args[0]] == 1:
            if args[2] < -2519.0 and args[2] > -2864.0:
                if args[1] > -1444.0 and args[1] < -1191.0:
                    click(args[0],0)
                elif args[1] < -913.0:
                    click(args[0],1)
                elif args[1] < -630.0:
                    click(args[0],2)
                elif args[1] < -362.0:
                    click(args[0],3)
        elif mode[args[0]] == 2:
            if args[2] < -3185.0 and args[2] > -3487.0:
                if args[1] > -1435.0 and args[1] < -1176.0:
                    click(args[0],0)
                elif args[1] < -898.0:
                    click(args[0],1)
                elif args[1] < -635.0:
                    click(args[0],2)
                elif args[1] < -362.0:
                    click(args[0],3)
    elif args[0] == 7: 
        if mode[args[0]] == 0:
            if args[1] > 653.0 and args[1] < 1036.0 and args[2] < -2377.0 and args[2] > -2933.0:
                click(args[0],0)
            elif args[1] > 639.0 and args[1] < 1017.0 and args[2] < -3101.0 and args[2] > -3428.0:
                click(args[0],1)
        elif mode[args[0]] == 1:
            if args[2] < -2454.0 and args[2] > -2919.0:
                if args[1] > 342.0 and args[1] < 602.0:
                    click(args[0],0)
                elif args[1] < 860.0:
                    click(args[0],1)
                elif args[1] < 1103.0:
                    click(args[0],2)
                elif args[1] < 1371.0:
                    click(args[0],3)
        elif mode[args[0]] == 2:
            if args[2] < -3165.0 and args[2] > -3409.0:
                if args[1] >  342.0 and args[1] < 560.0:
                    click(args[0],0)
                elif args[1] < 848.0:
                    click(args[0],1)
                elif args[1] < 1094.0:
                    click(args[0],2)
                elif args[1] < 1333.0:
                    click(args[0],3)
    elif args[0] == 8: 
        if mode[args[0]] == 0:
            if args[1] > -1074.0 and args[1] < -725.0 and args[2] < -2406.0 and args[2] > -2796.0:
                click(args[0],0)
            elif args[1] > -1027.0 and args[1] < -710.0 and args[2] < -3049.0 and args[2] > -3357.0:
                click(args[0],1)
        elif mode[args[0]] == 1:
            if args[2] < -2448.0 and args[2] > -2822.0:
                if args[1] > -1382.0 and args[1] < -1145.0:
                    click(args[0],0)
                elif args[1] < -874.0:
                    click(args[0],1)
                elif args[1] < -615.0:
                    click(args[0],2)
                elif args[1] < -362.0:
                    click(args[0],3)
        elif mode[args[0]] == 2:
            if args[2] < -3082.0 and args[2] > -3388.0:
                if args[1] > -1434.0 and args[1] < -1150.0:
                    click(args[0],0)
                elif args[1] < -871.0:
                    click(args[0],1)
                elif args[1] < -601.0:
                    click(args[0],2)
                elif args[1] < -389.0:
                    click(args[0],3)
    
def auto_callback(path, tags, args, source):
    # don't do this at home (or it'll quit blender)
    global AUTO
    AUTO = args[0]
    print AUTO

def mode_callback(path, tags, args, source):
    # don't do this at home (or it'll quit blender)
    mode[args[0]] = args[1]
    #print args[0], args[1], path
    click(args[0], args[1], path)
    to250(args[0], args[1], path)

def update_callback(path, tags, args, source):
    #print args[0], args[1], path
    to250(args[0], args[1], path)
    
def myos_callback(path, tags, args, source):
    global myos
    os.system(myos)
    
server.addMsgHandler( "/button1", user_callback )
server.addMsgHandler( "/auto", auto_callback )
server.addMsgHandler( "/mode", mode_callback )
server.addMsgHandler( "/myos", myos_callback )
server.addMsgHandler( "/cell", cell_callback )
server.addMsgHandler( "/update", update_callback )

# user script that's called by the game engine every frame
def each_frame():
    # clear timed_out flag
    server.timed_out = False
    # handle all pending requests then return
    while not server.timed_out:
        server.handle_request()

def AS(zero):
    global myos
    if mode[6] > 0 or mode[5] > 0:
        threading.Timer(90, AS, [0]).start()
    else:
        os.system(myos)
        threading.Timer(300, AS, [0]).start()
    
mode = [-1,-1,-1,-1,-1,-1,-1,-1,-1,-1]
myos = '\"C:\\Users\\Radar III\\Documents\\ReRadar.bat\"'
os.system(myos)    
#threading.Timer(300, AS, [0]).start()
AUTO = 0
# simulate a "game engine"
while run:
    NOW = datetime.datetime.now()
    if NOW.minute == 15:
        if NOW.hour == 9:
            if NOW.second == 0:
                os.system(myos)  
    elif 1 == AUTO and NOW.minute >= 56 and NOW.minute <= 58:
        click(5, -1, "/mode")
        click(6, -1, "/mode")
        # do the game stuff:
        sleep(0.1)    
    # call user script
    each_frame()

server.close()