#Anonymous lambda functions ----------------
names = ['Ma', 'Adams', 'Zandusky', 'diMeola']
names.sort(key = lambda s: s.lower())
print(names)

add_numbers = lambda a, b: a + b
print(add_numbers(2, 3))

#invoking an anonymous lambda function directly
print((lambda x, y: x * y)(5, 6))

#Some named lamda functions ------------------
currency = lambda n: f"${n:,.2f}"
print(currency(99))

percent = lambda n: f"{n:.2%}"
print(percent(0.065))
print(percent(0.5))

maximum = lambda a, b: a if a > b else b
print(maximum(15, 13))

min_value = lambda a, b, c: a if (a <= b) and (b <= c) else (b if (b <= a) and (b <= c) else c)
print(min_value(9, 5, 8))

L = [2, 8, 12, -5, -10]
L3 = list(map(lambda t: t * 2, L))
print("L3 =", L3) 

from functools import reduce
my_list = [2, 3, 4, 5]

product = reduce(lambda x, y: x * y, my_list)
print("Product of elements:", product)  # Output: 120



