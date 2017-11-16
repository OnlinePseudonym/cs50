print("card number: ", end='')

while True:
    cardString = input()
    d = cardString[0]
    twoD = cardString[:2]
    l = len(cardString)

    try:
        cardNumber = int(cardString)
    except ValueError:
        print('error: please enter a valid credit card number')
        print('card number: ', end='')
        continue
    if l < 17:
        break
    else:
        print('error: please enter a valid credit card number2')

s = 0
d = int(d)
twoD = int(twoD)
print(l)
print(d)

for i in range(l):
    print(i)
    curDigit = (cardNumber % (10**(i + 1))) // (10 ** i)
    if i % 2 == 0:
        s += curDigit
    else:
        if curDigit * 2 > 9:
            s += (curDigit * 2) % 10
            s += 1
        else:
            s += curDigit * 2
    print(s)

if (d == 4 and
        s % 10 == 0 and
        (l == 16 or l == 13)):
    print('VISA')
elif ((twoD == 34 or twoD == 37) and
        s % 10 == 0 and
        l == 15):
    print('AMEX')
elif ((twoD > 50 and twoD < 56) and
        s % 10 == 0 and
        l == 16):
    print('MASTERCARD')
else:
    print('INVALID')
