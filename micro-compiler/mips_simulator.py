import sys

class Simulator():
    def __init__(self, filename):
        self.filename = filename
        self.operations = []
        self.memory = {}
        