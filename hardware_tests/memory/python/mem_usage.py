def get_things(n: int):
    for i in range(n):
        yield Thing(i % 3 + 100000)

class Thing:
    def __init__(self, qty: int):
        self.qty_ = qty

things = get_things(5)
# things = get_things(10000000)

# print sum of all qtys
quantities = [thing.qty_ for thing in things]

print(quantities)
