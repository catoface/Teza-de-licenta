
from flask import Flask, request
from save_data import SaveData
import datetime

app = Flask(__name__)

sd = SaveData()

@app.route('/', methods=['POST'])
def receive_data_from_arduino():
    row = []
    ts = datetime.datetime.now()
    print(f'timestamp:{ts}')
    row.append(ts)
    temperature = request.args.get('temperature')
    row.append(temperature)
    print(f'temperature: {temperature}')
    humidity = request.args.get('humidity')
    print(f'humidity: {humidity}')
    row.append(humidity)
    sd.row_append(row)
    return 'OK'

@app.route('/', methods=['GET'])
def main_page():
  sd._save_data()
  sd.clear_buffer()
  return '''
    <html>
      <head>
        <link rel="stylesheet" href="/static/css/app.css">
      </head>
      <body>
        <a class="button" href="/"">Save</a>
        <a class="button" href="/static/data.csv" download="data.csv">Download</a>
      </body>
    </html>
  '''


if __name__ == '__main__':
    app.run(host="192.168.1.6", port="5000")
