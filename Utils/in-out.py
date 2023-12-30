def printInOut(_inlist, _outlist, _output_file):
    for i in range(0, len(_inlist)):
        sort_in = sorted(_inlist[i])
        sort_out = sorted(_outlist[i])
        if sort_in == sort_out:
            _output_file.write(f"BOTH IN AND OUT ARE EQUAL\n")
        _output_file.write(f"IN[{i}] =\t\t\t{str(sort_in)}\nOUT[{i}] =\t\t{str(sort_out)}\n\n")


def reaching_definitions(_predecessors, _gen, _kill, _output_file):
    if len(_predecessors) != len(_gen) or len(_predecessors) != len(_kill) or len(_gen) != len(_kill):
        raise Exception("The length of the lists must be the same")

    inlist = []
    outlist = []
    size = len(_predecessors)

    # Initialize the lists
    for i in range(0, size):
        inlist.append([])
        outlist.append([])
        outlist[i] = _gen[i]

    change = True
    iteration = 1
    while change:
        change = False
        _output_file.write(f"*-*-*-*-* START ITERATION {iteration} *-*-*-*-*\n")
        for i in range(0, size):
            for j in range(0, len(_predecessors[i])):
                inlist[i] = list(set(inlist[i] + outlist[_predecessors[i][j]]))
            oldout = outlist[i]
            outlist[i] = list(set(gen[i] + list(set(inlist[i]) - set(kill[i]))))
            if oldout != outlist[i]:
                change = True
        printInOut(inlist, outlist, _output_file)
        _output_file.write(f"*-*-*-*-* END ITERATION {iteration} *-*-*-*-*\n\n")
        iteration += 1

    return inlist, outlist


def predecessors_intersection(_predecessors):
    if len(_predecessors) == 0:
        return []
    if len(_predecessors) == 1:
        return _predecessors[0]
    else:
        intersection = _predecessors[0]
        for i in range(1, len(_predecessors)):
            intersection = list(set(intersection) & set(_predecessors[i]))
        return intersection


def generate_universe_set(_gen):
    universe_set = []
    for i in range(0, len(_gen)):
        for j in range(0, len(_gen[i])):
            if _gen[i][j] not in universe_set:
                universe_set.append(_gen[i][j])

    return universe_set


def data_flux_analysis(_predecessors, _gen, _kill, _output_file):
    if len(_predecessors) != len(_gen) or len(_predecessors) != len(_kill) or len(_gen) != len(_kill):
        raise Exception("The length of the lists must be the same")

    inlist = []
    outlist = []
    size = len(_predecessors)

    universe = generate_universe_set(_gen)
    for i in range(0, size):
        inlist.append([])
        outlist.append([])
        outlist[i] = list(set(universe) - set(_kill[i]))

    change = True
    iteration = 1
    while change:
        change = False
        _output_file.write(f"*-*-*-*-*-*-*-*-*-* START ITERATION {iteration} *-*-*-*-*-*-*-*-*-*\n")
        for i in range(0, size):
            inlist[i] = predecessors_intersection([outlist[j] for j in predecessor[i]])
            oldout = outlist[i]
            outlist[i] = list(set(_gen[i] + list(set(inlist[i]) - set(_kill[i]))))
            if oldout != outlist[i]:
                change = True
        printInOut(inlist, outlist, _output_file)
        _output_file.write(f"*-*-*-*-*-*-*-*-*-* END ITERATION {iteration} *-*-*-*-*-*-*-*-*-*\n\n")
        iteration += 1

    return inlist, outlist


def liveness_analysis(_predecessors, _use, _def, _output_file):
    if len(_predecessors) != len(_use) or len(_predecessors) != len(_def) or len(_use) != len(_def):
        raise Exception("The length of the lists must be the same")

    inlist = []
    outlist = []
    size = len(_predecessors)

    for i in range(0, size):
        inlist.append([])
        outlist.append([])

    change = True
    iteration = 1

    # liveness analysis is backwards
    while change:
        change = False
        _output_file.write(f"*-*-*-*-*-*-*-*-*-* START ITERATION {iteration} *-*-*-*-*-*-*-*-*-*\n")
        for i in range(0, size):
            current_index = size - i - 1
            oldout = outlist[current_index]

            outlist[current_index] = []
            for j in range(0, len(_predecessors[current_index])):
                outlist[current_index] = list(set(outlist[current_index] + inlist[_predecessors[current_index][j]]))

            oldin = inlist[current_index]
            inlist[current_index] = list(
                set(_use[current_index] + list(set(outlist[current_index]) - set(_def[current_index]))))

            if oldout != outlist[current_index] or oldin != inlist[current_index]:
                change = True
        printInOut(inlist, outlist, _output_file)
        _output_file.write(f"*-*-*-*-*-*-*-*-*-* END ITERATION {iteration} *-*-*-*-*-*-*-*-*-*\n\n")
        iteration += 1

    return inlist, outlist


if __name__ == '__main__':
    gen = [[],
           [1, 2],
           [3, 4],
           [5, 6],
           [7, 8],
           [9]]

    kill = [[],
            [],
            [],
            [5, 6],
            [7, 8],
            [2]]

    predecessor = [[],
                   [0],
                   [1, 5],
                   [2],
                   [2],
                   [3, 4]]

    reaching_def_file = open("reaching_def.txt", "w")
    inlist, outlist = reaching_definitions(predecessor, gen, kill, reaching_def_file)
    reaching_def_file.close()
    #
    # data_flux_file = open("data_flux.txt", "w")
    # data_flux_analysis(predecessor, gen, kill, data_flux_file)
    # data_flux_file.close()

    # liveness_analysis_file = open("liveness_analysis.txt", "w")
    # liveness_analysis(predecessor, gen, kill, liveness_analysis_file)
    # liveness_analysis_file.close()
