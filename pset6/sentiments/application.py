from flask import Flask, redirect, render_template, request, url_for

import os
import sys
import helpers
from analyzer import Analyzer
from nltk.tokenize import TweetTokenizer

app = Flask(__name__)

@app.route("/")
def index():
    return render_template("index.html")

@app.route("/search")
def search():

    # validate screen_name
    screen_name = request.args.get("screen_name", "")
    if not screen_name:
        return redirect(url_for("index"))

    # get screen_name's tweets
    try:
        tweets = helpers.get_user_timeline(screen_name, 100)
    except RuntimeError as e:
        print(e)
        sys.exit(1)
    if tweets == None:
        print("Requested tweets not found")
        sys.exit(2)

    positives = os.path.join(sys.path[0], "positive-words.txt")
    negatives = os.path.join(sys.path[0], "negative-words.txt")

    analyzer = Analyzer(positives, negatives)
    chartDict = {}

    for tweet in tweets:
        score = analyzer.analyze(tweet)
        if score > 0.0:
            chartDict[tweet] = 'positive'
        elif score < 0.0:
            chartDict[tweet] = 'negative'
        else:
            chartDict[tweet] = 'neutral'

    positive = sum(1 for x in chartDict.values() if x == 'positive')
    negative = sum(1 for x in chartDict.values() if x == 'negative')
    neutral = sum(1 for x in chartDict.values() if x == 'neutral')

    # generate chart
    chart = helpers.chart(positive, negative, neutral)

    # render results
    return render_template("search.html", chart=chart, screen_name=screen_name)
