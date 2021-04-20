#!/usr/bin/python
'''
tctl add [idx] [name]
tctl del [idx]
tctl swp [old_idx] [new_idx]
tctl ins [old_idx] [new_idx]
'''

import collections
import os
import re
import subprocess
import sys
import tempfile

EDITOR = '/usr/bin/vi'
DIR = '.'
VALID_FILE_REGEX = re.compile('^0?\d+ \w+\.(in|out)$')
SPLIT_FILE_REGEX = re.compile('(\ |\.)')


def usage(command='all'):
    cmdlines = ['tctl.py add [name]                --- create test at next free slot',
                'tctl.py add [idx] [name]          --- create at specified slot, shift the rest if necessary',
                'tctl.py del [idx]                 --- delete test',
                'tctl.py ins [src_idx] [dest_idx]  --- insert test from src_idx at new_idx, shift the rest',
                'tctl.py swp [old_idx] [new_idx]   --- swap tests at new_idx and old_idx']
    print('Usage:')
    for cmdline in cmdlines:
        if command in cmdline or command == 'all':
            print('\t' + cmdline)
    exit(1)


def check_cmdline(argv):
    if len(argv) == 3:
        if argv[1] != 'del' and argv[1] != 'add':
            usage()
        elif argv[1] == 'del' and not argv[2].isnumeric():
            usage('del')
    elif len(argv) == 4:
        if argv[1] != 'add' and argv[1] != 'swp' and argv[1] != 'ins':
            usage()
        elif argv[1] == 'add' and not argv[2].isnumeric():
            usage('add')
        elif argv[1] == 'swp' and not (argv[2].isnumeric() and argv[3].isnumeric()):
            usage('swp')
        elif argv[1] == 'ins' and not (argv[2].isnumeric() and argv[3].isnumeric()):
            usage('ins')
    else:
        usage()


def validate(file_list):
    for filename in file_list:
        (name, ext) = filename.split('.')
        test_ext = 'out' if ext == 'in' else 'in'
        test_filename = name + '.' + test_ext
        if test_filename not in file_list:
            print('Error: missing file\n\t\t' + test_filename)
            exit(1)
    return


def load_files(dir, with_regex):
    files = [str for str in sorted(os.listdir(dir)) if os.path.isfile(str)]
    filtered = [str for str in files if re.match(with_regex, str)]
    rejected = [str for str in files if str not in set(filtered)]
    if rejected != []:
        print('Rejected files:')
        for filename in rejected:
            print('    ' + filename)
    validate(filtered)
    return filtered


def mkindexed(file_list, regex):
    indexed = collections.OrderedDict()
    for name in file_list:
        (idx, title, ext) = re.split(regex, name)[::2]
        indexed[idx] = title
    return indexed


def mkkey(idx):
    return ('0' if idx < 10 else '') + str(idx)


def mktmpidx(indexed):
    return len(indexed) + 1000


def mkfilenames(idx, name):
    return [f'{mkkey(idx)} {name}.in', f'{mkkey(idx)} {name}.out']


def find_free_idx(indexed):
    # check if dir is empty
    if not indexed:
        ret = 0
    else:
        idx_list = list(map(lambda key: int(key), list(indexed)))
        last_idx = int(idx_list[0])
        for idx in idx_list:
            if (idx > last_idx + 1):
                break
            last_idx = idx
        ret = last_idx + 1
    return ret


def create_test(idx, name):
    for filename in mkfilenames(idx, name):
        print(f'create {filename}')
        with open(filename, 'x'):
            pass
        subprocess.call([EDITOR, filename])


def delete_test(idx, name):
    for filename in mkfilenames(idx, name):
        print(f'rm {filename}')
        os.remove(filename)


def move_test(idx1, idx2, name1, name2=None):
    if name2 == None:
        name2 = name1
    oldfilein, oldfileout = mkfilenames(idx1, name1)
    newfilein, newfileout = mkfilenames(idx2, name2)
    print(f'move {oldfilein} -> {newfilein}')
    os.rename(oldfilein, newfilein)
    print(f'move {oldfileout} -> {newfileout}')
    os.rename(oldfileout, newfileout)


def swap_tests(idx1, idx2, name1, name2):
    assert(name1 != name2)
    move_test(idx1, idx2, name1, name1)
    move_test(idx2, idx1, name2, name2)


def shift(from_idx, indexed, key_op, to_idx=None):
    if to_idx == None:
        to_idx = int(list(indexed.keys())[-1])
    direction = 'up' if key_op(1) > 1 else 'down'
    print(f'** Shift {direction} from idx=' + str(from_idx))
    for key, name in indexed.copy().items():
        idx = int(key)
        if idx >= from_idx and idx <= to_idx:
            idx_new = key_op(idx)
            assert(idx_new != idx)
            oldfilein, oldfileout = mkfilenames(idx, name)
            newfilein, newfileout = mkfilenames(idx_new, name)
            move_test(idx, key_op(idx), name)
            indexed.pop(key)
            indexed[mkkey(idx_new)] = name


def shift_up_from(from_idx, indexed):
    def key_op(k): return k + 1

    shift(from_idx, indexed, key_op)


def shift_down_to(to_idx, indexed):
    def key_op(k): return k - 1

    shift(to_idx + 1, indexed, key_op)


def execute_add(args, indexed):
    if len(args) == 1:
        new_idx = find_free_idx(indexed)
        name = args[0]
    else:
        new_idx = int(args[0])
        name = args[1]
        if new_idx < len(list(indexed.keys())):
            shift_up_from(new_idx, indexed)
    print(f'** Add: {name} @ {new_idx}')
    create_test(new_idx, name)


def execute_del(args, indexed):
    idx = int(args[0])
    name = indexed[mkkey(idx)]
    print(f'** Delete: {name} @ {idx}')
    delete_test(idx, name)
    if idx < len(indexed):
        shift_down_to(idx, indexed)


def execute_swp(args, indexed):
    idx1 = int(args[0])
    idx2 = int(args[1])
    name1 = indexed[mkkey(idx1)]
    name2 = indexed[mkkey(idx2)]
    print(f'** Swap: {name1} @ {idx1} <-> {name2} @ {idx2}')
    swap_tests(idx1, idx2, name1, name2)


def execute_ins(args, indexed):
    idx_src = int(args[0])
    idx_dest = int(args[1])
    name_src = indexed[mkkey(idx_src)]
    if mkkey(idx_dest) in indexed:
        idx_tmp = mktmpidx(indexed)
        name_tmp = name_src
        print(f'** Backup: {name_src} @ {idx_src} -> {name_tmp} @ {idx_tmp}')
        move_test(idx_src, idx_tmp, name_src, name_tmp)
        indexed.pop(mkkey(idx_src))
        shift_down_to(idx_src, indexed)
        shift_up_from(idx_dest, indexed)
        print(f'** Restore: {name_tmp} @ {idx_tmp} -> {name_tmp} @ {idx_dest}')
        move_test(idx_tmp, idx_dest, name_tmp)
    else:
        print(f'** Moving: {name_src} @ {idx_src} -> @ {idx_dest}')
        move_test(idx_src, idx_dest, name_src)
        shift_down_to(idx_src, indexed)


def execute_command(args, indexed):
    if args[0] == 'add':
        execute_add(args[1:], indexed)
    elif args[0] == 'del':
        execute_del(args[1:], indexed)
    elif args[0] == 'swp':
        execute_swp(args[1:], indexed)
    elif args[0] == 'ins':
        execute_ins(args[1:], indexed)


check_cmdline(sys.argv)
FILES = load_files(DIR, VALID_FILE_REGEX)
INDEXED = mkindexed(FILES, SPLIT_FILE_REGEX)
execute_command(sys.argv[1:], INDEXED)
