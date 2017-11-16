import sys
import crypt
def main():
    if len(sys.argv) != 2:
        print('ERROR: program limited to one alpha command-line argument')
        return 1

    seedchars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"
    hashedPW = sys.argv[1]
    salt = hashedPW[:2]
    l = len(seedchars)
    isMatch = False
    for i in range(l):
        g = seedchars[i]
        result = crypt.crypt(g, salt)
        if result == hashedPW:
            i = 1000
            isMatch = True
            break
    if (not isMatch):
        for i in range(l):
            for j in range(l):
                g = seedchars[i] + seedchars[j]
                result = crypt.crypt(g, salt)
                if result == hashedPW:
                    isMatch = True
                    break
            if isMatch:
                break
    if (not isMatch):
        for i in range(l):
            for j in range(l):
                for m in range(l):
                    g = seedchars[i] + seedchars[j] + seedchars[m]
                    result = crypt.crypt(g, salt)
                    if result == hashedPW:
                        isMatch = True
                        break
                if isMatch:
                    break
            if isMatch:
                break
    if (not isMatch):
        for i in range(l):
            for j in range(l):
                for m in range(l):
                    for n in range(l):
                        g = seedchars[i] + seedchars[j] + seedchars[m] + seedchars[n]
                        result = crypt.crypt(g, salt)
                        if result == hashedPW:
                            isMatch = True
                            break
                    if isMatch:
                        break
                if isMatch:
                    break
            if isMatch:
                break
    print(g)
    return 0

if __name__ == "__main__":
    main()
