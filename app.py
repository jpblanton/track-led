import struct

from flask import Flask, render_template, jsonify, request
from flask_mqtt import Mqtt

from utils import hex_to_rgb

app = Flask(__name__)
app.config.from_pyfile('config.py')
mqtt = Mqtt(app)
print(app.config['FLASK_ENV'])

distances = ['100m', '200m', '300m', '400m', '500m', '600m', '700m', '800m', '1600m', '3200m']

@app.route('/', methods=['POST', 'GET'])
def index():
    if request.method == 'POST':
        laptime = request.form['laptime']
        numlaps = request.form['numlaps']
        ledcolor = request.form['ledcolor'][1:]
        if laptime.strip().isdigit():
            lapseconds = laptime
        elif ':' in laptime:
            tmp = laptime.split(':')
            if len(tmp) == 2:
                lapseconds = (60 * tmp[0]) + tmp[1]
            elif len(tmp) == 3:
                lapseconds = (3600 * tmp[0]) + (60 * tmp[1]) + tmp[2]
            else:
                raise TypeError('needs either hh:mm:ss or mm:ss')
        else:
            raise TypeError('needs to be an integer or form [hh:]mm:ss')
        ledrgb = hex_to_rgb(ledcolor)
        ledgbr = (ledrgb[1], ledrgb[2], ledrgb[0])
        # mqtt.publish('test/track/led',  f"{laptime};{numlaps};{ledgbr[0]};{ledgbr[1]};{ledgbr[2]}", retain=True)
        mqtt.publish('test/track/led', struct.pack('5i', int(lapseconds), int(numlaps), *ledgbr))
        print(lapseconds)
        print(numlaps)

    return render_template('index.html', distances=distances)

@app.route('/button', methods=['POST'])
def test():

    if request.method == 'POST':
        mqtt.publish('test/track/led',  "10;2")
        return "yes it works"
    else:
        return "else"
