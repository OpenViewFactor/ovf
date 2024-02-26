def logStart(testLog, message):
    testLog.write(''.join(['\n< ----- ', message, ' ----- >']))
    print(''.join(['< ----- ', message, ' ----- >']))

def logUpdate(testLog, message):    
    testLog.write(''.join(['\n',message]))
    print(message)
    
def logEnd(testLog):
    testLog.write('\n< ----- LOG BREAK ----- >\n')
    print('< ----- LOG BREAK ----- >')