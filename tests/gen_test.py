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
            webpage_list.append(WebPage(url=line['url'],size=line['size']))
    return webpage_list


def gen_random_test(args, webpages):
    if 0 < args.webpage_limit < len(webpages):
        webpages = webpages[:args.webpage_limit]

    if args.output_file:
        output_filename = args.output_file
    else:
        output_filename = 'random_%d.txt' % args.test_length
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

    with open(output_filename, 'w') as output_file:
        for i in range(args.test_length):
            pos = random.randint(0, len(webpages)-1)
            output_file.write('%s\n' % webpages[pos].url)
    print('Test Created: %s' % output_filename)


def gen_test(args):
    webpages = get_webpage_list(args)


    if args.test_type == 'random':
        gen_random_test(args, webpages)
    elif args.test_type == 'freq':
        raise NotImplementedError
    elif args.test_type == 'size':
        raise NotImplementedError


if __name__ == "__main__":
    args = parse_args()
    gen_test(args)

