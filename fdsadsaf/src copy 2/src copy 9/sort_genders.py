import re

filename = "genders.hpp"

with open(filename, "r", encoding="utf-8") as f:
    text = f.read()

# najde všechno uvnitř { ... }
match = re.search(r"\{(.*)\}", text, re.S)
if not match:
    print("Nenalezen vector block")
    exit()

block = match.group(1)

# vytáhne stringy "..."
items = re.findall(r"\"(.*?)\"", block, re.S)

# sort
items_sorted = sorted(items, key=lambda s: s.lower())

# rebuild
new_block = ",\n    ".join(f"\"{x}\"" for x in items_sorted)

new_text = re.sub(
    r"\{.*\}",
    "{\n    " + new_block + "\n}",
    text,
    flags=re.S
)

with open(filename, "w", encoding="utf-8") as f:
    f.write(new_text)

print("Sorted genders.hpp successfully")