import sys
from time import sleep 
from serial import Serial, SerialException

class Arduino():
    PORTS = [
        '/dev/ttyACM0',
        '/dev/ttyACM1',
        '/dev/ttyACM2',
        '/dev/ttyUSB0',
        '/dev/ttyUSB1',
        '/dev/ttyUSB2',
        '/dev/ttyUSB3',
        '/dev/ttyUSB4',
    ]

    DECODE = {
        "cmd" : {
            "1001": "TEST",
            "1": "FORWARD",
            "2": "BACK",
            "3": "LEFT",
            "4": "RIGHT",
            "5": "ON_LINE",
            "0": "STOP",
        },
        "state": {
            0: "ON",
            1: "OFF",
        },
        "platform_state": {
            0: "INIT",
            1: "READY",
            2: "FORWARD",
            3: "STOP",
            4: "ROT_LFT",
            5: "ROT_RGT",
            6: "FOLLOW_LINE",
            7: "BACK",
        },
        "follow_state": {
            0: "START",
            1: "CALIBRATE",
            2: "CALIBRATED",
            3: "FOLLOW",
            4: "FAIL",
        }
    }

    COMMAND_KEY_ORDER  = ["cmd", "lft_pwm", "rgt_pwm", ]
    RESPONCE_KEY_ORDER = [
        (str, "last_cmd", "cmd"), 
        (int, "platform_state", "platform_state"), 
        (int, "lft_target", 0), 
        (int, "rgt_target", 0), 
        (int, "lft_avg", 0), 
        (int, "rgt_avg", 0), 
        (int, "lft_pwm", 0), 
        (int, "rgt_pwm", 0), 
        (int, "s_center", 0), 
        (int, "s_right", 0), 
        (int, "s_left", 0), 
        (int, "S_cnt_avg", 0),
        (int, "S_rgt_avg", 0),
        (int, "S_lft_avg", 0),
        (int, "follow_state", "follow_state"),
        (float, "VCC", 0), 
        (float, "I", 0), 
        (int, "motors_state", "state"),
        (int, "sensor_state", "state"),
    ]
    TEST = 1001
    FORWARD = 1
    BACK = 2
    LEFT = 3
    RIGHT = 4
    ON_LINE = 5
    STOP = 0

    def __init__(self):
        self.port = None
        for port in self.PORTS:
            try:
                self.port = Serial(port = port, baudrate = 9600, timeout = 1)
                sleep(2)
                if self.test(): break
               
            except SerialException as e:
                print(port, 'failed with %s' % e)
            
        if self.port == None:
            raise SerialException('Port is not found')
    
    def __del__(self):
        try:
            self.port.close()
        except AttributeError:
            print('Can not close port %s' % self.port, file = sys.stderr)

    def __str__(self):
        return str(self.port)

    def test(self):
        command = {
            "cmd": self.TEST,
            "lft_pwm": 0,
            "rgt_pwm": 0,
        }
        self.__send_command_dict(command)
        return self.__read_responce_dict()

    def forward(self, lft_pwm, rgt_pwm):
        command = {
            "cmd": self.FORWARD,
            "lft_pwm": lft_pwm,
            "rgt_pwm": rgt_pwm,
        }
        self.__send_command_dict(command)
        return self.__read_responce_dict()

    def follow_line(self, lft_pwm, rgt_pwm):
        command = {
            "cmd": self.ON_LINE,
            "lft_pwm": lft_pwm,
            "rgt_pwm": rgt_pwm,
        }
        self.__send_command_dict(command)
        return self.__read_responce_dict()

    def stop(self):
        command = {
            "cmd": self.STOP,
            "lft_pwm": 0,
            "rgt_pwm": 0,
        }
        self.__send_command_dict(command)
        return self.__read_responce_dict()

    def __send_command_dict(self, command):
        to_send = ""
        for key in self.COMMAND_KEY_ORDER:
            to_send += "%s " % str(command[key])
        self.port.write(bytes(to_send.strip(), "ascii"))

    def __read_responce_dict(self):
        rez = {}
        try:
            for _type, key, f in self.RESPONCE_KEY_ORDER:
                x = self.port.readline().decode().strip()
                x = _type(x)
                try:
                    rez.update({key:self.DECODE[f][x]})
                except KeyError:
                    rez.update({key:x})
        except ValueError:
            rez = None
        return rez

if __name__ == '__main__':
    log = []
    from pprint import pprint
    ino = Arduino()
    pprint(ino.follow_line(6,6))
    for i in range(30):
        log += [ino.test()]
        pprint(log[-1])
        sleep(1)
    pprint(ino.stop())
    from json import dump
    dump(log, open('log.json','w'), indent = 2)
