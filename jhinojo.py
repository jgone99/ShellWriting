import sys

if len(sys.argv) == 2:
    if sys.argv[1] == '-':
        prompt = ""
    else:
        prompt = sys.argv[1]
else:
    prompt = "jsh: "

print(prompt, end='', flush=True)
for line in sys.stdin:
    line = line.strip('\n')
    if line == "exit":
        sys.exit(0)
    line = line.replace('\t', ' ')
    words = line.split()
    print(*words, sep='\n')
    print(prompt, end='', flush=True)
