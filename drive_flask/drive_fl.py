#export FLASK_APP=drive_fl.py
#flask run --host 0.0.0.0 --port 8080

from flask import Flask, render_template, request
from json import dumps
from arduino import Arduino
arduino_due = Arduino()

def change_dir(speed, direct):
    x = int(direct * (speed / 255))
    if x > 0:
        rgt_pwm = speed - x
        lft_pwm = speed
    else:
        rgt_pwm = speed
        lft_pwm = speed - abs(x)
    return lft_pwm, rgt_pwm

app = Flask(__name__)

@app.route('/', methods=['GET','POST'])
def index(**qwargs):
    return render_template('index.html')

@app.route('/state', methods=['GET','POST'])
def drive_mod(**qwargs):
    state = request.form
    speed = int(state.get('speed'))
    direct = int(state.get('dir'))
    lft_pwm, rgt_pwm = change_dir(speed, direct)
    #print(state.get('speed'))
    #print(state.get('dir'))
    responce = {}
    if state.get('forward') == "1":
        responce = arduino_due.forward(lft_pwm, rgt_pwm)
        print("go forward", speed, direct)
    elif state.get('back') == "1":
        print("go back", speed, direct)
    elif state.get('left') == "1":
        responce = arduino_due.forward(0, rgt_pwm)        
        print("go left", speed, direct)
    elif state.get('right') == "1":
        responce = arduino_due.forward(lft_pwm, 0)
        print("go right", speed, direct)
    elif state.get('on_line') == "1":
        responce = arduino_due.follow_line(lft_pwm, rgt_pwm)
        print("go of line", speed,  direct)
    else:
        responce = arduino_due.stop()
        lft_pwm = 0
        rgt_pwm = 0
        print("stop!!!", speed, direct)
    try:
        pass
    except Exception as e:
        print(e)
    return dumps(responce, indent=2)

@app.errorhandler(404)
def not_found(error):
    return render_template('eror404.html'), 404

if __name__ == '__main__':
    app.run(debud=True)