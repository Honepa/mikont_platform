#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Fri Aug 14 18:20:31 2020

@author: honepa
"""
from tkinter import Tk
from arduino import Arduino

class WASD:
    def __init__():
        root = Tk()
        arduino_due = Arduino()
        root.bind('w', event_w)
        root.bind('a', event_a)
        root.bind('d', event_d)
        root.mainloop()
    def event_w(self, event):
        lft_pwm = 100
        rgt_pwm = 100
        arduino_due.forward(lft_pwm, rgt_pwm)
    def event_a(self, event):
        lft_pwm = 110
        rgt_pwm = 90
        arduino_due.forward(lft_pwm, rgt_pwm)
    def event_d(self, event):
        lft_pwm = 90
        rgt_pwm = 110
        arduino_due.forward(lft_pwm, rgt_pwm)

if __name__ == '__main__':
    platform = WASD()
    