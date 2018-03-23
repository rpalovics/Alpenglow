import sys
import re

if __name__ == "__main__":
  in_constr = False
  class_name = "InitialInvalidClassName"

  for line in sys.stdin:
    #sys.stderr.write("DEBUG: got line: " + line)
    class_first_line = "class" in line and not "friend" in line
    if class_first_line:
      class_name = re.search(r'\s*class\s+(\w+)[\s:{]', line).group(1)
    #erre vadaszunk: MyClass(MyClassParameters* params) : x_(params->x), y_(params->y) {}
    constructor_decl_start = class_name+"("+class_name+"Parameters" in line
    if constructor_decl_start:
      in_constr = True
      state = "constr_decl"
    if in_constr :
      out_line = ""
      for char in line :
        if state == "constr_decl" :
          if char==":" :
            state = "del"
          elif char=="{" or char==";":
            state = "keep_further"
            in_constr = False
            out_line+=char
          else:
            out_line+=char
        elif state == "del" :
          if char=="{":
            state = "keep_further"
            in_constr = False
            out_line+=char
        elif state == "keep_further":
          out_line+=char
      sys.stdout.write(out_line)
    else :
      sys.stdout.write(line)
