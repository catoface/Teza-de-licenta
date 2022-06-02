
import csv
from logging import exception

class SaveData:

    def __init__(self):
        self.data = []

    def row_append(self, row):
        self.row = row
        self.data.append(self.row)

    def _save_data(self):
        try:
            print('save data')
            with open('static/data.csv', 'w', newline='', encoding='UTF8') as f:
                writer = csv.writer(f)
                writer.writerows(self.data)
        except exception as e:
            print(e)

    def clear_buffer(self):
        self.data.clear()
