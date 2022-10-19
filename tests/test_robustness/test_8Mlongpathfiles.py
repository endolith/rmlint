#!/usr/bin/env python3
# encoding: utf-8
from nose.plugins.attrib import attr
from nose import with_setup
from tests.utils import *
import sys

NUMPAIRS = 1024+1

def branch_tree(current_path, remaining_depth):
    if (remaining_depth > 0):
        for i in range(2):
            next_path = current_path + "long" * 4 + str(i) + "/"
            create_dirs (next_path)
            branch_tree (next_path, remaining_depth - 1)
    else:
        for i in range(NUMPAIRS):
            create_file(str(i).zfill(1 + i), f'{current_path}a{str(i).zfill(7)}')
            create_file(str(i).zfill(1 + i), f'{current_path}b{str(i).zfill(7)}')
            create_link(
                f'{current_path}a{str(i).zfill(7)}',
                f'{current_path}c{str(i).zfill(7)}',
            )

            create_link(
                f'{current_path}a{str(i).zfill(7)}',
                f'{current_path}d{str(i).zfill(7)}',
            )

            create_link(
                f'{current_path}a{str(i).zfill(7)}',
                f'{current_path}e{str(i).zfill(7)}',
            )

            create_link(
                f'{current_path}a{str(i).zfill(7)}',
                f'{current_path}f{str(i).zfill(7)}',
            )

            create_link(
                f'{current_path}b{str(i).zfill(7)}',
                f'{current_path}g{str(i).zfill(7)}',
            )

            create_link(
                f'{current_path}b{str(i).zfill(7)}',
                f'{current_path}h{str(i).zfill(7)}',
            )


@attr('slow')
@with_setup(usual_setup_func, usual_teardown_func)
def test_manylongpathfiles():
    max_depth = 10 # will give 8M files total if NUMPAIRS = 1024+1
    branch_tree ("", max_depth)

    head, *data, footer = run_rmlint('-c json:no_body')
    assert footer['duplicates'] + footer['duplicate_sets'] == NUMPAIRS * 2 ** max_depth * 8
