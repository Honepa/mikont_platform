# -*- coding: utf-8 -*-
"""
    @author: honepa
"""

#export FLASK_APP=drive_fl.py
#flask run --host 0.0.0.0 --port 8080

from flask import Flask, render_template, request
import json

import sys
from serial import Serial, SerialException

PORTS = ['/dev/ttyUSB0', '/dev/ttyUSB1', '/dev/ttyUSB2', '/dev/ttyUSB3', '/dev/ttyUSB4', '/dev/ttyACM0', '/dev/ttyACM1', '/dev/ttyACM2']

class Arduino():
    
    def __init__(self, x):
        self.port = None
        for port in PORTS:
            try:
                #print(port)
                self.port = Serial(port = port, baudrate = 9600, timeout = 2)
                #print(self.port.readlines())
                self.port.write(b'0000')
                b = self.port.read()
                
                
                if b == x:
                    break               
               
                
            except SerialException as e:
                print(port, 'failed')
            
        if self.port == None:
            raise SerialException('Port is not found')
    
    def __del__(self):
        try:
            self.port.close()
        except AttributeError:
            print('nicht close', file = sys.stderr)        

    def __str__(self):
        return str(self.port)

#arduino_due = Arduino(0)

app = Flask(__name__)

@app.route('/', methods=['GET','POST'])
def index(**qwargs):
    return render_template('index.html')

@app.route('/state', methods=['GET','POST'])
def drive_mod(**qwargs):
    state = request.form
    speed = state.get('speed')
    direct = state.get('dir')
    print(state.get('speed'))
    print(state.get('dir'))
    if state.get('forward') == "1":
        arduino_due.port.write(b'0')
        arduino_due.port.write(b'0')
        arduino_due.port.write(b'1')
        arduino_due.port.write(b,speed)
        arduino_due.port.write(b'0')
        print("go forward", speed, direct)
    elif state.get('back') == "1":
        arduino_due.port.write(b'0')
        arduino_due.port.write(b'0')
        arduino_due.port.write(b'2')
        arduino_due.port.write(b,speed)
        arduino_due.port.write(b'0')
        print("go back", speed, direct)
    elif state.get('left') == "1":
        arduino_due.port.write(b'0')
        arduino_due.port.write(b'0')
        arduino_due.port.write(b'3')
        arduino_due.port.write(b,speed)
        arduino_due.port.write(b'0')
        print("go left", speed, direct)
    elif state.get('right') == "1":
        arduino_due.port.write(b'0')
        arduino_due.port.write(b'0')
        arduino_due.port.write(b'4')
        arduino_due.port.write(b,speed)
        arduino_due.port.write(b'0')
        print("go right", speed, direct)
    elif state.get('on_line') == "1":
        arduino_due.port.write(b'0')
        arduino_due.port.write(b'0')
        arduino_due.port.write(b'5')
        arduino_due.port.write(b,speed)
        arduino_due.port.write(b'0')
        print("go of line", speed,  direct)
    elif state.get('stop') == "1":
        arduino_due.port.write(b'0')
        arduino_due.port.write(b'0')
        arduino_due.port.write(b'6')
        arduino_due.port.write(b,speed)
        arduino_due.port.write(b'0')
        print("stop!!!", speed, direct)
    else:
        print("wait")
    return json.dumps({'success': True})

@app.errorhandler(404)
def not_found(error):
    return render_template('eror404.html'), 404


if __name__ == '__main__':
    app.run(debud=True)