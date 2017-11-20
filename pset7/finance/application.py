from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session, url_for
from flask_session import Session
from passlib.apps import custom_app_context as pwd_context
from tempfile import mkdtemp

from helpers import *

# configure application
app = Flask(__name__)

# ensure responses aren't cached
if app.config["DEBUG"]:
    @app.after_request
    def after_request(response):
        response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
        response.headers["Expires"] = 0
        response.headers["Pragma"] = "no-cache"
        return response

# custom filter
app.jinja_env.filters["usd"] = usd

# configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

@app.route("/")
@login_required
def index():
    entry = db.execute("SELECT * FROM users WHERE id=:id",
        id=session['user_id'])
    user = entry[0]['username']
    owned = db.execute("SELECT *, sum(shares) FROM transactions WHERE user=:user GROUP BY symbol HAVING SUM(shares) > 0",
        user=user)
    total_sum = 0

    for row in owned:
        query = lookup(row['symbol'])
        row['price'] = query['price']
        row['total'] = row['sum(shares)'] * row['price']
        total_sum += row['total']
        row['total'] = usd(row['total'])

    total_sum += entry[0]['cash']

    return render_template("index.html", stocks=owned, total=usd(total_sum), cash=usd(entry[0]['cash']))

@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock."""
    if request.method == "POST":

        sym = request.form.get("symbol").upper()
        shares = request.form.get("shares")
        row = db.execute("SELECT * FROM users WHERE id=:id",
            id=session['user_id'])

        if not sym:
            return apology("must provide stock's symbol")

        if not shares:
            return apology("must provide desired shares")

        query = lookup(sym)
        if not query:
            return apology("lookup failed, try again later")

        price = query['price']
        name = query['name']
        cash = row[0]['cash']
        user = row[0]['username']

        if cash < price * int(shares):
            return apology("Cannot afford " + shares + " shares of " + sym)
        else:
            db.execute("INSERT INTO transactions (id, user, symbol, name, price, shares) VALUES(NULL, :user, :symbol, :name, :price, :shares)",
                user=user, symbol=sym, name=name, price=price, shares=int(shares))
            db.execute("UPDATE users SET cash=:cash WHERE id = :id",
                cash=cash-price*int(shares), id=session['user_id'])
            return redirect(url_for("index"))
    else:
        return render_template("buy.html")

@app.route("/history")
@login_required
def history():
    """Show history of transactions."""

    entry = db.execute("SELECT * FROM users WHERE id=:id",
        id=session['user_id'])
    user = entry[0]['username']
    owned = db.execute("SELECT * FROM transactions WHERE user=:user ORDER BY date",
        user=user)

    return render_template("history.html", stocks = owned)

@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in."""

    # forget any user_id
    session.clear()

    # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username")

        # ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password")

        # query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username", username=request.form.get("username"))

        # ensure username exists and password is correct
        if len(rows) != 1 or not pwd_context.verify(request.form.get("password"), rows[0]["hash"]):
            return apology("invalid username and/or password")

        # remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # redirect user to home page
        return redirect(url_for("index"))

    # else if user reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")

@app.route("/logout")
def logout():
    """Log user out."""

    # forget any user_id
    session.clear()

    # redirect user to login form
    return redirect(url_for("login"))

@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "POST":

        sym = request.form.get("symbol")

        if not sym:
            return apology("must provide stock's symbol")

        result = lookup(sym)
        if not result:
            return apology("lookup failed")

        return render_template("quoted.html", name=result['name'], price=result['price'], symbol=result['symbol'])

    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user."""

    if request.method == "POST":

        if not request.form.get("username"):
            return apology("must provide username")

        elif not request.form.get("password"):
            return apology("must provide password")

        elif not request.form.get("repeat_password"):
            return apology("must repeat password")

        elif request.form.get("password") != request.form.get("repeat_password"):
            return apology("passwords do not match")

        pwhash = pwd_context.hash(request.form.get("password"))
        result = db.execute("INSERT INTO users (username, hash) VALUES(:username, :hsh)",
            username=request.form.get("username"), hsh=pwhash)

        if not result:
            return apology("username already exists")

        return redirect(url_for("login"))

    else:
        return render_template("register.html")

@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock."""

    if request.method == "POST":
        entry = db.execute("SELECT * FROM users WHERE id=:id",
            id=session['user_id'])
        user = entry[0]['username']
        owned = db.execute("SELECT * FROM transactions WHERE user=:user GROUP BY symbol HAVING SUM(shares) > 0",
            user=user)
        symbol = request.form.get("symbol")
        shares = int(request.form.get("shares"))

        if not symbol:
            return apology("please select a valid symbol")

        target_stock = db.execute("SELECT *, sum(shares) FROM transactions WHERE user=:user AND symbol=:symbol",
            user=user, symbol=symbol)
        print(target_stock)
        if not shares:
            return apology("must provide how many shares to sell")

        elif shares > target_stock[0]['sum(shares)'] or shares < 1:
            return apology("shares must be more than 0 and less than " + str(target_stock[0]['shares']))

        query = lookup(symbol)
        price = query['price']
        name = query['name']
        cash = entry[0]['cash']

        db.execute("INSERT INTO transactions (id, user, symbol, name, price, shares) VALUES(NULL, :user, :symbol, :name, :price, :shares)",
            user=user, symbol=symbol, name=target_stock[0]['name'], price=price, shares=-int(shares))
        db.execute("UPDATE users SET cash=:cash WHERE id = :id",
            cash=cash+price*shares, id=session['user_id'])

        return redirect(url_for("index"))

    else:
        entry = db.execute("SELECT * FROM users WHERE id=:id",
            id=session['user_id'])
        user = entry[0]['username']
        owned = db.execute("SELECT * FROM transactions WHERE user=:user GROUP BY symbol HAVING SUM(shares) > 0",
            user=user)

        return render_template("sell.html", stocks=owned)

@app.route("/deposit", methods=["GET", "POST"])
@login_required
def deposit():
    """Deposit more money into account."""

    if request.method == "POST":
        if not request.form.get("deposit"):
            return apology("Must enter amount to deposit")

        deposit = request.form.get("deposit")
        entry = db.execute("SELECT * FROM users WHERE id=:id",
            id=session['user_id'])
        user = entry[0]['username']
        cash = entry[0]['cash'] + float(deposit)

        db.execute("UPDATE users SET cash=:cash WHERE id = :id",
            cash=cash, id=session['user_id'])

        return redirect(url_for("index"))

    else:
        return render_template("deposit.html")