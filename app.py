from flask import Flask, render_template, jsonify, request
from flask_mqtt import Mqtt

app = Flask(__name__)
app.config['MQTT_BROKER_URL'] = 'test.mosquitto.org'  # use the free broker from HIVEMQ
app.config['MQTT_BROKER_PORT'] = 1883  # default port for non-tls connection
mqtt = Mqtt(app)

@app.route('/', methods=['POST', 'GET'])
def index():
    if request.method == 'POST':
        mqtt.publish('test/track/led',  "10;2")

    return render_template('index.html')

@app.route('/button', methods=['POST'])
def test():

    if request.method == 'POST':
        mqtt.publish('test/track/led',  "10;2")
        return "yes it works"
    else:
        return "else"
