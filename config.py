from os import environ
from dotenv import load_dotenv

FLASK_ENV = 'development'
TESTING = True
DEBUG = True
MQTT_BROKER_URL = 'test.mosquitto.org'  
MQTT_BROKER_PORT = 1883  # default port for non-tls connection