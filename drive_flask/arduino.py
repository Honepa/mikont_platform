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

    COMMAND_KEY_ORDER  = ["cmd", "lft_pwm", "rgt_pwm", ]
    RESPONCE_KEY_ORDER = [
        (str, "last_cmd"), 
        (int, "lft_pwm"), 
        (int, "rgt_pwm"), 
        (int, "lft_avg"), 
        (int, "rgt_avg"), 
        (float, "a0"), 
        (float, "a1"), 
        (float, "a2"), 
        (float, "VCC"), 
        (float, "I"), 
        (int, "motors_state"), 
        (int, "sensor_state"), 
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
            for _type, key in self.RESPONCE_KEY_ORDER:
                x = self.port.readline().decode().strip()
                x = _type(x)
                rez.update({key:x})
        except ValueError:
            rez = None
        return rez

if __name__ == '__main__':
    from pprint import pprint
    ino = Arduino()
    pprint(ino.test())
    sleep(1)
    pprint(ino.forward(20,20))
    for i in range(10):
        pprint(ino.test())
        sleep(1)
    pprint(ino.follow_line(50,50))
    for i in range(10):
        pprint(ino.test())
        sleep(1)
    pprint(ino.stop())
    pprint(ino.test())
        
