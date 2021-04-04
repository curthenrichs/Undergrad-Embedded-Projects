import sys
import csv

if __name__ == "__main__":
    # argument one is text file to encode
    # argument two is text file to store
    # argument three is delay string size
    if len(sys.argv) > 3:
        filepath_in = sys.argv[1]
        filepath_out = sys.argv[2]
        delay_cnt = int(sys.argv[3])
        if delay_cnt < 0:
            print('Delay count must be a positive or zero integer.')
            quit()
    else:
        print('All input parameters must be supplied.')
        quit()

    # iterate list of integers in text file
    data_in = []
    try:
        with open(filepath_in) as in_f:
            data_in = [int(x) for x in in_f]
    except IOError:
        print('Error occured while attempting to read from input file {}.'.format(filepath_in))
        quit()
    except ValueError:
        print('Values must be integers seperated by newline characters.')
        quit()

    # generate encoding on list
    data_out = []
    for i in data_in:
        if i < 0:
            print('Warning: integer {} is less than 0, using least significant bits.'.format(i))
            i = (i & 0x7F)
        elif i > 127:
            print('Warning: integer {} is greater than 127, using least significant bits.'.format(i))
            i = (i & 0x7F)

        # MSB always logic-1
        data_out.append(1)
        data_out.append(0)

        # 7 data bits
        for n in range(1, 8):
            if ((i >> (7 - n)) & 0x1) == 1:
                data_out.append(1)
                data_out.append(0)
            else:
                data_out.append(0)
                data_out.append(0)

    # generate delay zeros
    for d in range(0,delay_cnt):
        data_out.append(0)
        data_out.append(0)

    # store as CSV file
    try:
        with open(filepath_out, 'w', newline='') as out_f:
            writer = csv.writer(out_f, delimiter=' ', quoting=csv.QUOTE_NONE)
            for e in data_out:
                writer.writerow([e])
    except IOError:
        print('Error occured while attempting to write to output file {}.'.format(filepath_out))
        quit()
