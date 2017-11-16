import nltk
from nltk.tokenize import TweetTokenizer

class Analyzer():
    """Implements sentiment analysis."""

    def __init__(self, positives, negatives):
        """Initialize Analyzer."""
        self.positives = set()
        self.negatives = set()

        with open(positives, 'r') as file:
            for line in file:
                strippedLine = line.strip()
                if (not line.startswith(';') and not strippedLine == ''):
                    self.positives.add(strippedLine)

        with open(negatives, 'r') as file:
            for line in file:
                strippedLine = line.strip()
                if (not line.startswith(';') and not strippedLine == ''):
                    self.negatives.add(strippedLine)

    def analyze(self, text):
        """Analyze text for sentiment, returning its score."""
        count = 0
        tknzr = TweetTokenizer()
        tknzdTxt = tknzr.tokenize(text)
        for word in tknzdTxt:
            query = word.strip().lower()
            if query in self.positives:
                count += 1
            elif query in self.negatives:
                count -= 1
        return count
