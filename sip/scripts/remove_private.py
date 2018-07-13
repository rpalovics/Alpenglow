import sys
import re

if __name__ == "__main__":
  state_in = False #in struct or in class
  nonc_part = False #sip methodcode or docstring

  for line in sys.stdin:
    #sys.stderr.write("DEBUG: got line: " + line)
    if "%Docstring" in line or "MethodCode" in line:
      nonc_part = True
    class_header = re.match("\s*class", line)
    if class_header:
      state_in = True
      private_part = True
      private_type = "  private:\n"
      private_written = True
    struct_first_line = "struct" in line
    if struct_first_line:
      state_in = True
      private_part = False
    if state_in and "public" in line:
      private_part = False
    if state_in and "protected" in line:
      private_part = True
      private_type = "  protected:\n"
      private_written = False
    if state_in and "private" in line:
      private_part = True
      private_type = "  private:\n"
      private_written = False
    end_of_class = "};" in line
    if end_of_class :
      state_in = False
    override_method = "override" in line or "virtual" in line
    #sys.stderr.write("DEBUG: state_in=" +str(state_in)+", private_part="+str(private_part)+"\n\n")
    decision = False
    if not state_in or class_header or nonc_part or not private_part :
      decision = True
    elif override_method :
      if not private_written :
        sys.stdout.write(private_type)
        private_written = True
      decision = True
    if decision :
      sys.stdout.write(line)
    if "%End" in line:
      nonc_part = False
