#!/usr/bin/env python3
from OSC import OSCServer
import sys
from time import sleep
import os
import OSC
import threading
cc = OSC.OSCClient()
cc.connect(('192.168.0.252', 12002))   # localhost, port 57120

server = OSCServer( ("0.0.0.0", 12001) )
server.timeout = 0.001
run = True

def click(pos, speed):
    global cc
    #unit = random.randint(5,5)
    oscmsg = OSC.OSCMessage()
    oscmsg.setAddress("/button")
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
                click(3,0)
            elif args[1] > -809.0 and args[1] < -538.0 and args[2] < -1682.0 and args[2] > -1922.0:
                click(3,1)
        elif mode[args[0]] == 1:
            if args[2] < -1038.0 and args[2] > -1257.0:
                if args[1] > -1238.0 and args[1] < -985.0:
                    click(3,0)
                elif args[1] < -720.0:
                    click(3,1)
                elif args[1] < -472.0:
                    click(3,2)
                elif args[1] < -225.0:
                    click(3,3)
        elif mode[args[0]] == 2:
            if args[2] < -1708.0 and args[2] > -1931.0:
                if args[1] > -1261.0 and args[1] < -1001.0:
                    click(3,0)
                elif args[1] < -715.0:
                    click(3,1)
                elif args[1] < -472.0:
                    click(3,2)
                elif args[1] < -269.0:
                    click(3,3)
    '''
    elif args[0] == 4: 
        if mode[args[0]] == 0:
        elif mode[args[0]] == 1:
        elif mode[args[0]] == 2:
    elif args[0] == 5: 
        if mode[args[0]] == 0:
        elif mode[args[0]] == 1:
        elif mode[args[0]] == 2:
    elif args[0] == 6: 
        if mode[args[0]] == 0:
        elif mode[args[0]] == 1:
        elif mode[args[0]] == 2:
    elif args[0] == 7: 
        if mode[args[0]] == 0:
        elif mode[args[0]] == 1:
        elif mode[args[0]] == 2:
    elif args[0] == 8: 
        if mode[args[0]] == 0:
        elif mode[args[0]] == 1:
        elif mode[args[0]] == 2:
    '''
def quit_callback(path, tags, args, source):
    # don't do this at home (or it'll quit blender)
    global run
    run = False

def mode_callback(path, tags, args, source):
    # don't do this at home (or it'll quit blender)
    mode[args[0]] = args[1]
    #print args[0], args[1]
    
server.addMsgHandler( "/button1", user_callback )
#server.addMsgHandler( "/quit", quit_callback )
server.addMsgHandler( "/mode", mode_callback )

# user script that's called by the game engine every frame
def each_frame():
    # clear timed_out flag
    server.timed_out = False
    # handle all pending requests then return
    while not server.timed_out:
        server.handle_request()

def AS(zero):
    os.system('\"C:\Users\Radar IV\Documents\ReRadar.bat\"')
    threading.Timer(86400, AS, [0]).start()

mode = [-1,-1,-1,-1,-1,-1,-1,-1,-1,-1]

os.system('\"C:\Users\Radar IV\Documents\ReRadar.bat\"')    
threading.Timer(86400, AS, [0]).start()
    
# simulate a "game engine"
while run:
    # do the game stuff:
    #sleep(0.01)
    # call user script
    each_frame()

server.close()