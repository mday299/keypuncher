class Elf:
    name = "undefined"
    calories = 0
    
    def __init__(self, n):
        self.name = n

    def get_calories(self):
        return self.calories

    def set_calories(self, cal):
        self.calories = cal