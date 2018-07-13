import sys
import re

if __name__ == "__main__":
  sip_code = False
  for line in sys.stdin:
    #sys.stderr.write("DEBUG: got line: " + line)
    start_sip_code = re.match("\s*/\* SIP_CODE", line)
    if start_sip_code:
      #sys.stderr.write("DEBUG: start sip_code\n")
      sip_code = True
    end_sip_code = sip_code and re.match("\s*\*/", line)
    if end_sip_code:
      #sys.stderr.write("DEBUG: end sip_code\n")
      sip_code = False
    if not start_sip_code and not end_sip_code :
      sys.stdout.write(line)
