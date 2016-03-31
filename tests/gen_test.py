#! /usr/bin/python
import csv
import sys
import argparse
import random
import os


class WebPage:
    def __init__(self, url, freq=0, size=0):
        self.url = url
        self.freq = freq
        self.size = size


def parse_args():
    parser = argparse.ArgumentParser(description='Generate webpage test file')

    parser.add_argument('test_type', type=str, choices=('random', 'freq', 'size'),
                        help='Type of test file to generate')

    parser.add_argument('test_length', type=int, help='Length of test file to generate')

    parser.add_argument('--webpage_list_file', type=str, required=False, default='similarweb_list.csv',
                        help='User trace file')

    parser.add_argument('--webpage_limit', type=int, required=False, default=0,
                        help='Limit to top N pages')

    parser.add_argument('--output_file', type=str, required=False, help='Name of output test file')
    return parser.parse_args()


def get_webpage_list(args):
    webpage_list = []
    with open(args.webpage_list_file) as page_file:
        csvreader = csv.DictReader(page_file)
        for line in csvreader:
            webpage_list.append(WebPage(url=line['url'],size=int(line['size'])))
    return webpage_list


def gen_random_test(args, webpages, output_filename):
    if 0 < args.webpage_limit < len(webpages):
        webpages = webpages[:args.webpage_limit]

    working_set = {}
    with open(output_filename, 'w') as output_file:
        for i in range(args.test_length):
            pos = random.randint(0, len(webpages)-1)
            page = webpages[pos]
            output_file.write('%s\n' % page.url)
            working_set[page.url] = page.size
    print('Working Set Size: %d' % (sum(working_set.values())))
    print('Number of Unique Pages: %d' % len(working_set))


def gen_freq_test(args, webpages, output_filename):
    if 0 < args.webpage_limit < len(webpages):
        webpages = webpages[:args.webpage_limit]

    probs = [.6, .25, .1, .05]
    cumsum = [sum(probs[:x]) for x in range(1, len(probs)+1)]
    limits = [10, 20, 25]
    page_distr = [[]]*len(probs)
    for i in range(3):
        page_distr[i] = webpages[sum(limits[:i]):sum(limits[:i+1])]
        assert len(page_distr[i]) == limits[i]
    page_distr[3] = webpages[sum(limits):]
    assert len(page_distr) > 0

    working_set = {}
    with open(output_filename, 'w') as output_file:
        for i in range(args.test_length):
            p = random.random()
            index = 0
            while p > cumsum[index]:
                index += 1

            subset = page_distr[index]
            assert len(subset) > 0
            pos = random.randint(0, len(subset)-1)
            page = subset[pos]
            output_file.write('%s\n' % page.url)
            working_set[page.url] = page.size
    print('Working Set Size: %d' % (sum(working_set.values())))
    print('Number of Unique Pages: %d' % len(working_set))


def gen_size_test(args, webpages, output_filename):
    if 0 < args.webpage_limit < len(webpages):
        webpages = webpages[:args.webpage_limit]

    sorted(webpages, key=lambda x: x.size)
    gen_freq_test(args, webpages, output_filename)


def gen_test(args):
    webpages = get_webpage_list(args)

    if args.output_file:
        output_filename = args.output_file
    else:
        output_filename = '%s_%d.txt' % (args.test_type, args.test_length)
        if os.path.exists(output_filename):
            print("Default output file \"%s\" already exists" % output_filename)
            overwrite = raw_input("Overwrite existing file [Y/N]: ")
            if overwrite == 'Y' or overwrite == 'y':
                pass
            elif overwrite == 'N' or overwrite == 'n':
                print('Not generating test.')
                return
            else:
                print("Invalid input")
                return

    if args.test_type == 'random':
        gen_random_test(args, webpages, output_filename)
    elif args.test_type == 'freq':
        gen_freq_test(args, webpages, output_filename)
    elif args.test_type == 'size':
        gen_size_test(args, webpages, output_filename)
    print('Test Created: %s' % output_filename)


if __name__ == "__main__":
    command_args = parse_args()
    gen_test(command_args)

