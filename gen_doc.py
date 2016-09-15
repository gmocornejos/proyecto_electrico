import re
import sys

func_init = re.compile('\s*\w+\s+\w+\s*\(')
func_end = re.compile('[\s\w]*\);\s*')
func_flag = re.compile('\s*//@\s+\w+')

def main():
    file = open(sys.argv[1], 'r')

    for line in file:
        if func_flag.search(line) != None:
            line = line.strip().split()
            print(' '.join(line[2:]) )
            print('\n')
            print(''.join(func_collect(file)))

def func_collect(file):
    function = None
    for line in file:
        if func_init.search(line) != None:
            function = [line]
            break
    for line in file:
        function.append(line)
        if func_end.search(line) != None:
            break
    return function

if __name__ == "__main__":
    main()
