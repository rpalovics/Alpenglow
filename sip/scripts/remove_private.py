import sys

if __name__ == "__main__":
  in_class = False #in struct or in class
  private_part = False #private or protected declarations

  for line in sys.stdin:
    #sys.stderr.write("DEBUG: got line: " + line)
    class_first_line = "class" in line or "struct" in line
    if class_first_line:
      in_class = True
    private_first_line = "private" in line or "protected" in line
    if in_class and private_first_line:
      private_part = True
    end_of_class = "};" in line
    if end_of_class :
      in_class = False
      private_part = False
    override_method = "override" in line
    if not private_part or override_method :
      sys.stdout.write(line)
