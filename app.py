import struct

from flask import Flask, render_template, jsonify, request
from flask_mqtt import Mqtt

from utils import hex_to_rgb

app = Flask(__name__)
app.config.from_pyfile('config.py')
mqtt = Mqtt(app)
print(app.config['FLASK_ENV'])

distances = ['100m', '200m', '300m', '400m', '500m', '600m', '700m', '800m', '1600m', '3200m']
LAP_LENGTH = 200

@app.route('/', methods=['POST', 'GET'])
def index():
    if request.method == 'POST':
        lapseconds = int(request.form['laptime'])
        distance = request.form['distance']
        ledcolor = request.form['ledcolor'][1:]
        ledrgb = hex_to_rgb(ledcolor)
        ledgbr = (ledrgb[1], ledrgb[2], ledrgb[0])
        # mqtt.publish('test/track/led',  f"{laptime};{numlaps};{ledgbr[0]};{ledgbr[1]};{ledgbr[2]}", retain=True)
        numlaps = int(distance[:-1]) / LAP_LENGTH
        mqtt.publish('test/track/led', struct.pack('5i', lapseconds, numlaps, *ledgbr))
        print(lapseconds)

    return render_template('index.html', distances=distances)

@app.route('/button', methods=['POST'])
def test():

    if request.method == 'POST':
        mqtt.publish('test/track/led',  "10;2")
        return "yes it works"
    else:
        return "else"
