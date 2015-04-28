__author__ = 'tjd08a'
import random as rand
import sys

def write_file(ramps, N):
    print "Making course file"
    name = "course.txt"

    file = None
    try:
        file = open(name, 'w')
    except:
        print "File write error\n"
        sys.exit(0)

    first_line = "%d\n" % N
    file.write(first_line)
    for ramp in ramps:
        line = "%d\n" % ramp
        file.write(line)



ramp_locations = []
N = 8
min_ramps = 3
rand.seed()
ramps = rand.randint(min_ramps, N)
max_locations = N * 3 - 1

for i in xrange(ramps):
    location = rand.randint(0, max_locations)
    while location in ramp_locations:
        location = rand.randint(0, max_locations)

    ramp_locations.append(location)

ramp_locations.sort()
write_file(ramp_locations, N)
print ramp_locations

