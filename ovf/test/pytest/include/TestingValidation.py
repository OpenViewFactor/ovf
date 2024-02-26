def PercentDiff(result, expected):
    percentDifference = (result - expected) / expected * 100
    return percentDifference

def ValidateNear(result, expected, tolerance):
    percentDifference = PercentDiff(result, expected)
    if abs(percentDifference) > tolerance:
        return False
    else:
        return True