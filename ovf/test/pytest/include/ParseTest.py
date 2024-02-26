import xml.etree.ElementTree as ET

#TODO refactor this code to make use of dictionaries instead of just a list
#! makes it easier and clearer to access elements by name instead of by index

def XMLParse(root, testName, testOutputsDir):
    INFORMATION = root.findall('TEST-DESCRIPTION')
    CONFIG = INFORMATION[0][0].text

    TARGETS = root.findall('TARGETS')
    ANALYTIC = float(TARGETS[0][0].text)
    EXPECTED = float(TARGETS[0][1].text)
    TOLERANCE = float(TARGETS[0][2].text)

    NUMERICS = [ANALYTIC, EXPECTED, TOLERANCE]
    
    MESHES = root.findall('MESHES')
    EMITTER = MESHES[0][0].text
    RECEIVER = MESHES[0][1].text
    BLOCKERS = MESHES[0][2:]

    SURFACES = [EMITTER, RECEIVER]
    
    SETTINGS = root.findall('SETTINGS')
    SELFINT = SETTINGS[0][0].text
    METHOD = SETTINGS[0][1].text
    PRECISION = SETTINGS[0][2].text

    ARGS = [SELFINT, METHOD, PRECISION]

    data = [NUMERICS, SURFACES, ARGS]
    return data, BLOCKERS