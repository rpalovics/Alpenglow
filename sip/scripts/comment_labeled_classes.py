import sys
import re

# TODO kikupalni ezt a barkacsmunkat. Ejnye. Legalabb a zarojeleket lehetne szamolni, a kommentek es stringek figyelembevetelevel.

if __name__ == "__main__":
  commenting = False
  for line in sys.stdin:
    #sys.stderr.write("DEBUG: got line: " + line)
    start_commenting = "COMMENTOUTTHISCLASS" in line #TODO remove this word
    if start_commenting:
      #sys.stderr.write("DEBUG: start commenting\n")
      commenting = True
    if commenting :
      sys.stdout.write("//"+line) #TODO if already commented, dont comment twice
    else :
      sys.stdout.write(line)
    end_commenting = line == "};\n"
    if end_commenting :
      #sys.stderr.write("DEBUG: end commenting\n")
      commenting = False
