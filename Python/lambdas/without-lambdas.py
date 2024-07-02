def lowercaseof(anystring):
    return anystring.lower()

def currency(n, w=15):
    s = f"${n:,.2f}"
    #Pad left of output with spaces = to w
    return s.rjust(w)

def multiply_by_2(t):
    return t * 2

names = ['Ma', 'Adams', 'Zandusky', 'diMeola']
names.sort(key=lowercaseof)
print(names)

print(currency(21.99, 14))
print(currency(21.997, 20))

L = [2, 8, 12, -5, -10]
L2 = list(map(multiply_by_2, L))
print("L2 =", L2)


