#!/usr/bin/env python3

from flask import Flask
app = Flask(__name__)

import db
database = db.DB()

import journal
logger = journal.JournalLogger()

import controller

if __name__ == '__main__':
    app.run(debug=True)
