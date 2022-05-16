from flask import Flask, request
import csv
import datetime

app = Flask(__name__)
data = []

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
    data.append(row)
    return 'OK'

@app.route('/', methods=['GET'])
def write_data_to_csv():
    with open('data.csv', 'w', newline='', encoding='UTF8') as f:
        writer = csv.writer(f)
        writer.writerows(data)
    return '<p>write data to csv</p>'


if __name__ == '__main__':
    app.run(host="192.168.1.6", port="5000")
