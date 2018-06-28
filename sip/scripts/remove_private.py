import sys

if __name__ == "__main__":
  state_in = False #in struct or in class

  for line in sys.stdin:
    #sys.stderr.write("DEBUG: got line: " + line)
    class_first_line = "class" in line
    struct_first_line = "struct" in line
    if class_first_line:
      state_in = True
      private_part = True
      private_type = "private:"
      private_written = True
    if struct_first_line:
      state_in = True
      private_part = False
    if state_in and "public" in line:
      private_part = False
    if state_in and "protected" in line:
      private_part = True
      private_type = "protected:"
      private_written = False
    if state_in and "private" in line:
      private_part = True
      private_type = "private:"
      private_written = False
    end_of_class = "};" in line
    if end_of_class :
      state_in = False
    override_method = "override" in line
    if not private_part or override_method :
      sys.stdout.write(line)
#TODO: bonyolultabb kiiras
