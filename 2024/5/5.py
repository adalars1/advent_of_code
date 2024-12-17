
import re
from math import floor


def validate_order(before, after, update : str):
  last_before_idx = update.rfind(before)
  first_after_idx = update.find(after)

  if last_before_idx == -1 or first_after_idx == -1:
    return True # line is not applicable for rule
  else:
    if last_before_idx < first_after_idx:
      return True # follows rule
    else:
      return False # breaks rule
     
def validate_all_orders(order_dict : dict, update : str):
  for key, after_list in order_dict.items():
    for after in after_list:
      res = validate_order(key, after, update)
      if not res:
        print("ERROR: update '" + update + "' breaks rule " + key + "|" + after)
      
  
order_rules = {}
updates = []
valid_updates = []
updates_to_fix = []

with open("./input", "r", encoding="utf-8") as data:
  for line in data:
    line = line.strip() # remove whitespaces
    if "|" in line: # rule line
      #print("order line: " + line)
      key, after_list = line.split("|") 
      if key in order_rules.keys():
        order_rules[key].append(after_list)
      else:
        order_rules[key] = list()
        order_rules[key].append(after_list)
    elif "," in line: #update line
      updates.append(line)


for update in updates:
  valid_update = True
  
  for before, after_list in order_rules.items():
    for after in after_list:
      if not validate_order(before, after, update):
        valid_update = False
        break
    
  if valid_update:
    valid_updates.append(update) 
  else:
    updates_to_fix.append(update) # save for later fixing
      

sum_middle_values = 0
for valid_update in valid_updates:
  valid_update_steps = valid_update.split(",")
  middle_index = floor(len(valid_update_steps)//2) # get middle index value
  sum_middle_values += int(valid_update_steps[middle_index])
    

print("Sum of middle values: " + str(sum_middle_values))

fixed_updates = []

for update in updates_to_fix:
  split_update = update.split(",")
  for before, after_list in order_rules.items():
    for after in after_list:
      if split_update.count(before) > 0 and split_update.count(after) > 0:
        first_index = split_update.index(before)
        second_index = split_update.index(after)

        if first_index > second_index:
          first_val = split_update[first_index] # get value to move
          split_update.remove(first_val) # remove old value
          split_update.insert(second_index, str(first_val)) # insert moved value where rule is followed
  # check that modified order is valid for this update
  validate_all_orders(order_rules, str(split_update).replace("[", "").replace("]" ,"").replace("'", "").replace(" ", ""))
  fixed_updates.append(str(split_update).replace("'",""))
  
sum_fixed_middle_values = 0
for fixed_update in fixed_updates:
  fixed_update_steps = fixed_update.split(",")
  middle_index = floor(len(fixed_update_steps)//2) # get middle index value
  sum_fixed_middle_values += int(fixed_update_steps[middle_index])
  
print("Sum of fixed middle values: " + str(sum_fixed_middle_values))
