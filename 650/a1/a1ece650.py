import sys
import re


# assumptions based on assignment_1 FAQ

# code for class Point, Class Line and func intersect taken from ECE650 repo

class Point(object):
    def __init__(self, x, y):
        self.x = float(x)
        self.y = float(y)

    def __str__(self):
        return '(' + str(self.x) + ',' + str(self.y) + ')'


class Line(object):
    def __init__(self, src, dst):
        self.src = src
        self.dst = dst

    def __str__(self):
        return str(self.src) + '-->' + str(self.dst)


def intersect(l1, l2):
    x1, y1 = l1.src.x, l1.src.y
    x2, y2 = l1.dst.x, l1.dst.y
    x3, y3 = l2.src.x, l2.src.y
    x4, y4 = l2.dst.x, l2.dst.y

    # Formula for "line intersect given two points on a segment taken from
    # https://en.wikipedia.org/wiki/Line%E2%80%93line_intersection

    tu_den = ((x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4))

    # checking if tu_den is zero for parallel and co-linear points
    if tu_den != 0:
        t = ((x1 - x3) * (y3 - y4) - (y1 - y3) * (x3 - x4)) / tu_den
        u = ((x1 - x3) * (y1 - y2) - (y1 - y3) * (x1 - x2)) / tu_den
        if 0.0 <= t <= 1.0 and 0.0 <= u <= 1.0:  # check if intersection occurs within end points of line segment
            x, y = (x1 + t * (x2 - x1), y1 + t * (y2 - y1))
            return Point(x, y)
        else:
            # print("The Segments do not intersect")
            return None
    # if denominator is zero, check for intersection of points of collinear lines
    elif collinearLineInt(l1, l2) is not None:
        # print('found collinear intersection')
        return collinearLineInt(l1, l2)
    else:
        # print('no intersection found after checking collinear')
        return None


def collinearLineInt(l1, l2):
    x1, y1 = l1.src.x, l1.src.y
    x2, y2 = l1.dst.x, l1.dst.y
    x3, y3 = l2.src.x, l2.src.y
    x4, y4 = l2.dst.x, l2.dst.y

    if (x1, y1) == (x3, y3):
        return Point(x1, y1)
    elif (x1, y1) == (x4, y4):
        return Point(x1, y1)
    elif (x2, y2) == (x3, y3):
        return Point(x2, y2)
    elif (x2, y2) == (x4, y4):
        return Point(x2, y2)
    else:
        return None


def getIntersections(lines_dict):
    # this takes in street/lines dictionary and returns all valid intersections as dict {line:[int]}
    key_list = list(lines_dict.keys())
    # unused - inter_list = []
    lines_int_dict = {}
    for i in range(len(key_list) - 1):
        for j in range(i + 1, len(key_list)):
            for line1 in lines_dict[key_list[i]]:
                for line2 in lines_dict[key_list[j]]:
                    # print('intersect of {} and {}'.format(line1, line2))
                    # print(intersect(line1, line2))
                    int_point = intersect(line1, line2)
                    if int_point is not None:
                        # unused - inter_list.extend([int_point, line1.src, line1.dst, line2.src, line2.dst])
                        # unused - above makes a list of int_point and the two intersections line src and dst points

                        # below makes a dictionary whose keys are lines, and values are all intersection on that line

                        if line1 in lines_int_dict:
                            if (int_point.x, int_point.y) not in lines_int_dict[line1]:
                                # checks if the int point already exists for that key
                                lines_int_dict[line1].append((int_point.x, int_point.y))
                        else:
                            lines_int_dict[line1] = [(int_point.x, int_point.y)]

                        if line2 in lines_int_dict:
                            if (int_point.x, int_point.y) not in lines_int_dict[line2]:
                                lines_int_dict[line2].append((int_point.x, int_point.y))
                        else:
                            lines_int_dict[line2] = [(int_point.x, int_point.y)]

    # for line in lines_int_dict:
    #     print(line)
    #     print(lines_int_dict[line])
    # for i in range(len(inter_list)):
    #     print(inter_list[i])
    return lines_int_dict


def getVerticesAndEdges(line_and_intersections):
    # input is of form {line1 : [(intersection 1),(intersection2)...], line2: [(intersection 1),(intersection2)...]
    # we change it to [[line1src, int1, int2..., line1dst],[line2src, int1, int2..., line2dst]]
    # we can then sort the sublists inside, this will give all edges between linesrc and line dst
    # similarly we can flatten the whole list and set() it to get all vertices

    line_and_intersections_list = []
    sublists_flattened = []
    sublists_flattened_2dp_sorted = []

    for a_line in line_and_intersections:
        line_and_intersections_list.append(
            [[(a_line.src.x, a_line.src.y)], line_and_intersections[a_line], [(a_line.dst.x, a_line.dst.y)]])
        # in this list, the src, intersections and dst are a list of their own, we need to flatten it

    for element in line_and_intersections_list:
        # this list comprehension to flatten a list taken from
        # https://stackoverflow.com/questions/952914/how-to-make-a-flat-list-out-of-a-list-of-lists?rq=1
        sublists_flattened.append([item for sublist in element for item in sublist])

    # THIS STEP IS TO MAKE SURE ALL POINTS ARE ROUNDED TO 2.D.P, sorted in ascending order, duplicates removed
    for sublist in sublists_flattened:
        sublists_flattened_2dp_sorted.append(sorted(list(set(
            [(float("{:.2f}".format(point[0])), float("{:.2f}".format(point[1]))) for point in sublist]))))

    # print (sublists_flattened_2dp_sorted)
    # we will use sublists_flattened_2dp_SORTED to show vertices and edges
    # FINDING VERTICES: COMPLETELY FLATTENED THE LIST - SET
    vertex_list = list(set([item for sublist in sublists_flattened_2dp_sorted for item in sublist]))

    print('V = {', file=sys.stdout)
    for i in range(0, len(vertex_list)):
        print('{}: {}'.format(i + 1, vertex_list[i]), file=sys.stdout)
    print('}', file=sys.stdout)

    # WE MAP THE INDICES OF OUR VERTEX_LIST ONTO SUBLISTS_FLATTENED_2DP_SORTED - THIS LIST ESSENTIALLY HAS ALL EDGES
    # GOING FROM POINT TO POINT, SORTED IN ASCENDING ORDER. WE THEN PRINT OUT THE MAPPED INDICES, YOU CAN DO THIS WITH
    edges_list = []
    for sublist in sublists_flattened_2dp_sorted:
        # pass every point in sublist to vertex_list.index() to map vertex indices to points in sublist
        edges_list.append([vertex_list.index(item) + 1 for item in sublist])

    # PRINTING THE EDGES
    # note - we made this weird printing program below just to make sure the last edge set does not have comma after it
    print('E = {', file=sys.stdout)
    for sublist in edges_list:
        if sublist is edges_list[-1]:
            for i in range(0, len(sublist) - 1):
                print('<{},{}>'.format(sublist[i], sublist[i + 1]), file=sys.stdout, end="")
                if i < len(sublist) - 2:
                    print(',', file=sys.stdout)
        else:
            for i in range(0, len(sublist) - 1):
                print('<{},{}>,'.format(sublist[i], sublist[i + 1]), file=sys.stdout)
    if not vertex_list:
        print('}', file=sys.stdout)
    else:
        print('\n}', file=sys.stdout)


def getStreetLines(streets_dict):
    # this function takes in dictionary of street names and their points and returns
    # street names and their line segments as dictionary, value are of type object line
    street_lines = {}
    key_list = list(streets_dict.keys())
    for i in range(len(key_list)):
        point_list = list(streets_dict[key_list[i]])
        line_list = []
        for j in range(len(point_list) - 1):
            L = Line(Point(point_list[j][0], point_list[j][1]), Point(point_list[j + 1][0], point_list[j + 1][1]))
            line_list.append(L)
        street_lines.update({key_list[i]: line_list})
    return street_lines


def getStreetDets(aString):
    aStringList = aString.lower().strip().split('"')
    # aStringList[0] gives add/mod/rm/gg
    # aStringList[1] gives name of street
    # aStringList[2] gives coordinates as string
    return aStringList


def getCoordinatesFromStr(aString):
    list_of_numbers = re.findall('-?\d+', aString)
    # this coordinate extraction algorithm was taken from
    # https://stackoverflow.com/questions/53990075/convert-list-into-list-of-tuples-of-every-two-elements
    coordinates_list = [(int(list_of_numbers[i]), int(list_of_numbers[i + 1])) for i in
                        range(0, len(list_of_numbers), 2)]
    # coordinates_list = [ast.literal_eval(x) for x in coordinates_str_list] - doesn't work with leading zero int
    return coordinates_list


def main():
    streets = {}
    re_add = re.compile('^ *add +"[a-zA-Z ]+" +(\( *-?\d+ *, *-?\d+ *\) +)+\( *-?\d+ *, *-?\d+ *\) *$')
    re_mod = re.compile('^ *mod +"[a-zA-Z ]+" +(\( *-?\d+ *, *-?\d+ *\) +)+\( *-?\d+ *, *-?\d+ *\) *$')
    re_rm = re.compile('^ *rm +"[a-zA-Z ]+" *$')
    re_gg = re.compile('^ *gg *$')
    re_showStreets = re.compile('^ *streets *$')

    while True:
        try:
            line = sys.stdin.readline()
            if line == "":
                break

            elif line.strip() == "":
                print("Error: Command cannot be empty", file=sys.stderr)

            #elif re_showStreets.findall(line.strip()):
                #print("Streets/coordinates currently in DB: {}".format(streets))

            elif re_add.findall(line.strip()):
                street_name = getStreetDets(line)[1]
                coordinates = getCoordinatesFromStr(getStreetDets(line)[2])
                if street_name not in streets.keys():
                    streets.update({street_name: coordinates})
                else:
                    print("Error: Street already exists", file=sys.stderr)

            elif re_mod.findall(line.strip()):
                # print('mod command accepted')
                street_name = getStreetDets(line)[1]
                coordinates = getCoordinatesFromStr(getStreetDets(line)[2])
                if street_name in streets.keys():
                    streets.update({street_name: coordinates})
                else:
                    print("Error: Cannot modify, Street does not exist", file=sys.stderr)


            elif re_rm.findall(line.strip()):
                street_name = getStreetDets(line)[1]
                if street_name in streets.keys():
                    streets.pop(street_name)
                else:
                    print("Error: Cannot remove, Street does not exist", file=sys.stderr)

            elif re_gg.findall(line.strip()):
                # print('gg command accepted')
                line_int_dict = {}
                street_lines_dict = getStreetLines(streets)  # takes streets and give lines dict
                ints_and_endpoints = getIntersections(street_lines_dict)
                getVerticesAndEdges(ints_and_endpoints)

            else:
                print('Error: Invalid command format. Please check your syntax and re-enter:', file=sys.stderr)

        except Exception as e:
            print("Program Error: {}".format(e), file=sys.stderr)
            pass

    # print("Finished reading input", file=sys.stdout)
    # # return exit code 0 on successful termination
    sys.exit(0)


if __name__ == "__main__":
    main()
