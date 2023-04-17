import sys

if len(sys.argv) == 2:
    if sys.argv[1] == '-':
        prompt = ""
    else:
        prompt = sys.argv[1]
else:
    prompt = "jsh: "

print(sys.argv)
print(prompt, end='', flush=True)
for line in sys.stdin:
    line = line.strip('\n')
    if not line:
        print(prompt, end='', flush=True)
        continue
    if line == "exit":
        sys.exit(0)
    line = line.replace('\t', ' ')
    args = line.split()
    print(*args, sep='\n')
    print(prompt, end='', flush=True)
