from itertools import product

def evaluate_expression(target, numbers):
    """
    Kontrollera om det finns en kombination av addition och multiplikation
    mellan talen som ger resultatet target, där uträkningen alltid görs från vänster till höger.

    :param target: Målvärdet (ett heltal).
    :param numbers: En lista av tal som ska kombineras.
    :return: True om en giltig kombination hittas, annars False.
    """
    num_count = len(numbers)
    operators = ['+', '*']

    # Generera alla möjliga kombinationer av operatorer
    for ops in product(operators, repeat=num_count - 1):
        # Utför beräkningar från vänster till höger
        result = numbers[0]
        for i in range(num_count - 1):
            if ops[i] == '+':
                result += numbers[i + 1]
            elif ops[i] == '*':
                result *= numbers[i + 1]

        # Jämför resultatet med målvärdet
        if result == target:
            return True

    return False

def process_list(lines):
    """
    Bearbeta en lista av rader och kontrollera om målet kan uppnås för varje rad.

    :param lines: En lista av strängar där varje rad innehåller målvärdet och talen.
    :return: En lista med resultat (True/False) för varje rad.
    """
    results = []
    for line in lines:
        # Dela upp raden i målvärde och talen
        target_str, numbers_str = line.split(":")
        target = int(target_str.strip())
        numbers = list(map(int, numbers_str.strip().split()))

        # Kontrollera om uttrycket kan ge målvärdet
        results.append(evaluate_expression(target, numbers))

    return results


lines = []

with open("./input", "r", encoding="utf-8") as data:
  for line in data:
    lines.append(line)
    
results = process_list(lines)
sum_true_results = 0
for line, result in zip(lines, results):
    if result:
      sum_true_results += int(line.split(": ")[0])

print("Sum of all true expressions: " + str(sum_true_results))