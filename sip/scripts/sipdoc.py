import sys
import re

if __name__ == "__main__":
  doc = False
  for line in sys.stdin:
    #sys.stderr.write("DEBUG: got line: " + line)
    start_doc = re.match("\s*/\*\*", line)
    if start_doc:
      #sys.stderr.write("DEBUG: start doc\n")
      doc = True
    end_doc = doc and re.match("\s*\*/", line)
    if end_doc:
      #sys.stderr.write("DEBUG: end doc\n")
      doc = False
    if start_doc :
      sys.stdout.write("  %Docstring\n") 
    elif end_doc :
      sys.stdout.write("  %End\n") 
    else :
      sys.stdout.write(line)
