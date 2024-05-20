import random

rounds = [
    100,
    500,
    1000,
    5000,
    10000,
    50000,
    100000,
    500000,
]


for round in rounds:
    with open(f"{round}_input.txt", "a") as f:
        f.write(str(round) + "\n")
        for i in range(round):
            f.write(str(random.randint(1, round + 1)) + "\n")
