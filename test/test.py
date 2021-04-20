#!/usr/bin/python
'''
Invoke:
test.py executable directory

test executable using input-output pairs found in (optional) subdirectories:

./directory/subdirectory/*.in  <-- questions
./directory/subdirectory/*.out <-- answers

syntax of *.IN:
# case description
actual input
EOF
'''

from _collections import OrderedDict
from colorama import Fore, Back, Style, init
import os
import signal
import subprocess
import sys


# colorama init ensures terminal colors compatibility on windows
init()

INFILE_EXT = 'in'
OUTFILE_EXT = 'out'
if len(sys.argv) != 3:
    print('Usage:\n\ttest.py executable directory\n')
    quit()
EXECUCTABLE = os.path.abspath(sys.argv[1])
BASE_DIR = os.path.abspath(sys.argv[2])


class Error:
    def __init__(self, infile_path, test_name, return_code, output):
        self.infile_path = infile_path
        self.test_name = test_name
        self.return_code = return_code
        self.output = output

    def __str__(self):
        return (f'Error({self.infile_path}, {self.test_name}, {self.return_code},' +
                f'{self.ouput})')

    def __repr__(self):
        return str(self)


class Fail:
    def __init__(self, infile_path, test_name, input, output_expected, output_received):
        self.infile_path = infile_path
        self.test_name = test_name
        self.input = input
        self.output_expected = output_expected
        self.output_received = output_received

    def __str__(self):
        return f'Fail({self.infile_path}, {self.test_name}, {self.output_expected}, ' +\
            f'{self.output_received}'

    def __repr__(self):
        return str(self)


def get_relpath(path):
    '''
    Returns part of directory path relative to BASE_DIR 
    '''
    return str(path).removeprefix(BASE_DIR + '/')


def get_test_sets(base):
    '''
    Extracts test from directory tree starting from base
    returns them in format
    [path -> infiles_list]
    Expect directory tree nodes to contain either only branches or only leaves
    '''
    sets = {}
    for root, dirs, files in os.walk(base):
        # empty dirs means root contain only leaves i.e. files
        infiles = list(filter(
            lambda filename: filename.endswith(INFILE_EXT), files
        ))
        if not dirs and infiles:
            sets[get_relpath(root)] = sorted(infiles)
    return OrderedDict(sorted(sets.items(), key=lambda t: t[0]))


def get_counter_max_str(test_dict):
    '''
    Sum of lengths of all test sets
    '''
    ret = 0
    for _, list in dict(test_dict).items():
        ret += len(list)
    return str(ret)


def get_counter_str(counter, sets_dict):
    '''
    Create counter string enumerating list
    '''
    counter_max_str = get_counter_max_str(sets_dict)
    return '(' + str(counter + 1).zfill(len(counter_max_str)) + '/' +\
        counter_max_str + ')'


def get_test_content(infile_path):
    '''
    Read test name and input from infile if is in format
    # test name
    test content
    EOF
    else return file name as name
    '''
    with open(infile_path) as infile:
        header = infile.readline()
        input = infile.read()
        if header and header[0] == '#':
            name = header[1:].strip()
        else:
            _, name = os.path.split(infile_path)
            input = header + input
        return (name, input)


def print_result(result, counter_str, test_name):
    result_strings = {
        'OK': ' ' + Fore.GREEN +
              'OK' + Style.RESET_ALL + '    ',
        'FAIL': ' ' + Fore.RED +
                'FAIL' + Style.RESET_ALL + '  ',
        'ERROR': ' ' + Style.BRIGHT + Fore.YELLOW +
                 'ERR' + Style.RESET_ALL + '   '
    }
    print(result_strings[result] + ' ' + counter_str + '   ' + test_name)


def run_test(executable, infile_path, counter_str, results):
    (test_name, test_input) = get_test_content(infile_path)
    completed_proc = subprocess.run(executable, stdin=open(infile_path),
                                    stdout=subprocess.PIPE,
                                    stderr=subprocess.STDOUT)
    try:
        output_received = completed_proc.stdout.decode()
    except:
        print('Decode output error!')
        output_received = completed_proc.stdout

    if completed_proc.returncode != 0:
        results['ERROR'].append(
            Error(str(infile_path), test_name, completed_proc.returncode,
                  output_received))
        result_str = 'ERROR'
    else:
        outfile_path = os.path.normpath(
            str(infile_path).replace('.in', '.out', -2))
        with open(outfile_path) as fd:
            output_expected = fd.read()
            if output_expected == output_received:
                result_str = 'OK'
            else:
                results['FAIL'].append(
                    Fail(str(infile_path), test_name, test_input,
                         output_expected, output_received))
                result_str = 'FAIL'
    print_result(result_str, counter_str, test_name)


def print_results_summary(results):
    if not results['ERROR'] and not results['FAIL']:
        print('\nAll tests OK!')
    if results['ERROR']:
        print(f'\n\n\tErrors: {len(results["ERROR"])}')
        print('***********************************')
        for error in results['ERROR']:
            relpath = get_relpath(error.infile_path)
            if error.return_code != 0:
                signal_name = signal.Signals(-error.return_code).name
                print(f'\n{Style.BRIGHT}{relpath}: ' +
                      f'caught {signal_name}{Style.RESET_ALL}' +
                      (f' with output:\n{error.output}' if error.output else ''))
            else:
                print(f'{Style.BRIGHT}{relpath}: ' +
                      f'returned {error.return_code}{Style.RESET_ALL}' +
                      (f' with output:\n{error.output}' if error.output else ''))
    if results['FAIL']:
        print(f'\n\n\tFailed: {len(results["FAIL"])}')
        print('***********************************')
        for fail in results['FAIL']:
            relpath = get_relpath(fail.infile_path)
            print(f'\n{Style.BRIGHT}{relpath}: failed{Style.RESET_ALL}')
            print(f'\tinput   : {repr(fail.input)}')
            print(f'\texpected: {repr(fail.output_expected)}')
            print(f'\treceived: {repr(fail.output_received)}')
    print('***********************************')


if not os.path.exists(EXECUCTABLE):
    print(f'executable {EXECUCTABLE} not found')
    quit(127)

if not os.path.exists(BASE_DIR):
    print(f'directory {BASE_DIR} not found')
    quit(2)

TEST_SETS = get_test_sets(BASE_DIR)

print(f'\nTESTING: {EXECUCTABLE}')
print(f'AGAINST: {BASE_DIR}')
print('***********************************\n')

RESULTS = {'FAIL': [], 'ERROR': []}

COUNTER_BASE = 0
for root, infile_set in TEST_SETS.items():
    if root != BASE_DIR:
        print('SUBDIR:  ' + root)
    counter_max = len(infile_set)
    for counter, infile in enumerate(infile_set):
        dir = BASE_DIR + '/' + root if BASE_DIR != root else BASE_DIR
        infile_path = os.path.normpath(dir + '/' + infile)
        counter_str = get_counter_str(COUNTER_BASE + counter, TEST_SETS)
        run_test(EXECUCTABLE, infile_path, counter_str, RESULTS)
    COUNTER_BASE = COUNTER_BASE + len(infile_set)


print_results_summary(RESULTS)

print('\n')
