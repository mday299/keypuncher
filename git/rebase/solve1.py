import Elf

elf_list = []

i = 1
calories = 0
with open('input1-1.txt') as in_file:
    for line in in_file:
        if line == "\n":
            #Record calories; Start the next elf
            elf_list.append(Elf.Elf("elf" + str(i)))
            elf_list[i-1].set_calories(calories)
            i = i+1
            calories = 0
        else:
            calories += int(line)

greatest_calories = 0
for next_elf in elf_list:
    if next_elf.get_calories() > greatest_calories:
        greatest_calories = next_elf.get_calories()

print(greatest_calories)