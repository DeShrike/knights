from itertools import permutations

TAKE = 2
SIZE = 1024
FILENAME = "generate.sh"
OUTFOLDER = "./generated"

pieces = ["Knight", "Dromedary", "Alfil", "Antilope", "Dabbaba", "Wazir", "Zebra", "Ferz", "Mantis", "Sipius", "Xoch"]

with open(FILENAME, "w") as fo:
   fo.write("#!/bin/bash\n")
   fo.write("\n")
   fo.write(f"mkdir -p {OUTFOLDER}\n")
   fo.write("\n")

   for l in permutations(pieces, TAKE):
      opponents = "-".join(l)
      fo.write(f"./knights {opponents} {SIZE} {OUTFOLDER}/{opponents}.png\n")

   fo.write("\n")
   fo.write("echo Done.\n")
   fo.write(f"echo Generated files are in the folder {OUTFOLDER}\n")

print(f"Generated {FILENAME}")
print(f"To run do")
print(f"$ chmod +x {FILENAME}")
print(f"$ ./{FILENAME}")

