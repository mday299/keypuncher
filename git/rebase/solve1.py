import Elf

elf_l = []

i = 1
calories = 0
with open('input1-1.txt') as in_file:
    for line in in_file:
        if line == "\n":
            #Record calories; Start the next elf
            elf_l.append(Elf.Elf("elf" + str(i)))
            elf_l[i-1].set_calories(calories)
            i = i+1
            calories = 0
        else:
            calories += int(line)

greatest_calories = 0
for next_elf in elf_l:
    if next_elf.get_calories() > greatest_calories:
        greatest_calories = next_elf.get_calories()

second_greatest = 0
for next_elf in elf_l:
    if next_elf.get_calories() > second_greatest and \
        next_elf.get_calories() != greatest_calories:
        second_greatest = next_elf.get_calories()

third_greatest = 0 
for next_elf in elf_l:
    if next_elf.get_calories() > third_greatest and \
        next_elf.get_calories() != greatest_calories and \
        next_elf.get_calories() != second_greatest:
        third_greatest = next_elf.get_calories()

print(greatest_calories)
print(greatest_calories + second_greatest + third_greatest)
