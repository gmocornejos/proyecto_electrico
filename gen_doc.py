import re
import sys
from pathlib import Path 

func_init = re.compile('\s*\w+\s+\w+\s*\(')
func_end = re.compile('\);')
doc_flag = re.compile('\s*//@\s+\w+')

class documentation():
    def __init__(self, name, docs):
        self.name = name
        self.docs = docs

class fuction(documentation):
    def __init__(self, name, docs):
        documentation.__init__(self, name, docs)

class datastruct(documentation):
    def __init__(self, name, docs):
        documentation.__init__(self, name, docs)

class source_file(documentation):
    def __init__(self, path, name, docs):
        documentation.__init__(self, name, docs)
        self.functions = list()
        self.datastrucs = list()

def build_tree(dirs):
    tree = dict()
    for d in dirs:
        l = [s for s in d.iterdir() if s.is_dir()]
        tree.update({d:build_tree(l)})
    return tree

def print_tree(dirs, level):
    for k, v in dirs.items():
        print('-'*level + k.name)
        print_tree(v, level+1)

def main():
    directories_tree = build_tree( [Path(x) for x in sys.argv[1:] ])
    print_tree(directories_tree, 0)
'''    
    file = open(sys.argv[1], 'r')

    for line in file:
        if doc_flag.search(line) != None:
            line = line.strip().split()
            print(' '.join(line[2:]) )
            print('\n')
            print(''.join(func_collect(file)))
'''
def func_collect(file):
    function = None
    for line in file:
        if func_init.search(line) != None:
            function = [line]
            if func_end.search(line) != None:
                return function
            else :
                break
    for line in file:
        function.append(line)
        if func_end.search(line) != None:
            break
    return function

if __name__ == "__main__":
    main()
