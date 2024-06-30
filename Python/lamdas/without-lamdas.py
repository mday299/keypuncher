def lowercaseof(anystring):
    return anystring.lower()

names = ['Ma', 'Adams', 'Zandusky', 'diMeola']
names.sort(key=lowercaseof)

print(names)