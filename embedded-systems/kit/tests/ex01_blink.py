SCENARIO = {"duration": 5000}
def check(tr, H):
    return H.mode_is(tr, 13, "OUTPUT") + H.periodic(tr, 13, 500, 500)
